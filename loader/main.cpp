#include <iostream>
#include <array>
#include <windows.h>
#include <shlobj.h>
#include <filesystem>

typedef NTSTATUS(_stdcall* _NtSetInformationProcess)(
	HANDLE           ProcessHandle,
	PROCESS_INFORMATION_CLASS ProcessInformationClass,
	PVOID            ProcessInformation,
	ULONG            ProcessInformationLength
	);

BOOL SetRemoteProcessMitigationPolicy (
	DWORD targetPid,
	PROCESS_MITIGATION_POLICY MitigationPolicy,
	PVOID lpBuffer,
	SIZE_T dwLength) {

	BOOL result = FALSE;
	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_SET_INFORMATION, FALSE, targetPid);

	if (proc != NULL) {
		HMODULE ntdll_module = GetModuleHandleA("ntdll.dll");
		if (ntdll_module == NULL) {
			std::cerr << "failed to get module handle for ntdll.dll\n";
			std::cin.get();
			return 1;
		}

		_NtSetInformationProcess NtSetInformationProcess = (_NtSetInformationProcess)GetProcAddress(ntdll_module, "NtSetInformationProcess");

		uint64_t policy = *(DWORD*)lpBuffer;
		policy = policy << 32;
		policy += (DWORD)MitigationPolicy;

		NTSTATUS ret = NtSetInformationProcess(
			proc,
			(PROCESS_INFORMATION_CLASS)0x34,
			&policy,
			sizeof(policy)
		);

		if (ret == 0)
			result = TRUE;

		CloseHandle(proc);
	}

	return result;
}

BOOL EnableDebugPrivilege() {
	HANDLE token;
	TOKEN_PRIVILEGES tp;
	BOOL result;
	LUID luid;

	result = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token);
	if (!result) {
		return FALSE;
	}

	result = LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);
	if (!result) {
		CloseHandle(token);
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	result = AdjustTokenPrivileges(token, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	if (!result) {
		CloseHandle(token);
		return FALSE;
	}

	CloseHandle(token);
	return TRUE;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "usage: drop dll on top of this executable\n";
		std::cin.get();
		return 1;
	}

	if (!EnableDebugPrivilege()) {
		std::cerr << "failed to enable debug privilege\n";
		std::cin.get();
		return 1;
	}

	char path[MAX_PATH];

	std::filesystem::path localAppDataPath = std::filesystem::path(getenv("LOCALAPPDATA"));

	std::filesystem::path filePath = localAppDataPath / "Growtopia" / "Growtopia.exe";

	std::strcpy(path, filePath.string().c_str());

	STARTUPINFO startup_info = {};
	PROCESS_INFORMATION process_info = {};
	if (!CreateProcess(path, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startup_info, &process_info)) {
		std::cerr << "failed to start growtopia.exe\n";
		std::cin.get();
		return 1;
	}

	WaitForInputIdle(process_info.hProcess, INFINITE);

	HWND hwnd = FindWindow(NULL, "Growtopia");
	if (!hwnd) {
		std::cerr << "failed to find growtopia window\n";
		std::cin.get();
		return 1;
	}

	DWORD process_id;
	GetWindowThreadProcessId(hwnd, &process_id);

	PROCESS_MITIGATION_DYNAMIC_CODE_POLICY policy = {};
	policy.ProhibitDynamicCode = FALSE;
	if (!SetRemoteProcessMitigationPolicy(
		process_id,
		ProcessDynamicCodePolicy,
		&policy,
		sizeof(policy)
	)) {
		std::cerr << "failed to disable BlockDynamicCode mitigation policy\n";
		std::cin.get();
		return 1;
	}

	const char* dll_path = argv[1];
	std::size_t dll_path_len = std::strlen(dll_path);

	HMODULE module_handle = LoadLibrary(dll_path);
	if (module_handle == NULL) {
		std::cerr << "failed to load dll\n";
		std::cin.get();
		return 1;
	}

	HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);

	// restore NtProtectVirtualMemory original bytes
	HMODULE ntdll_module = GetModuleHandleA("ntdll.dll");
	if (!ntdll_module) {
		std::cerr << "failed to get module handle for ntdll.dll\n";
		std::cin.get();
		return 1;
	}

	LPVOID nt_protect_virtual_memory_addr = GetProcAddress(ntdll_module, "NtProtectVirtualMemory");
	if (!nt_protect_virtual_memory_addr) {
		std::cerr << "failed to get address of nt_protect_virtual_memory\n";
		std::cin.get();
		return 1;
	}

	constexpr std::array<std::uint8_t, 5> bytes = { 0x4C, 0x8B, 0xD1, 0xB8, 0x50 };

	if (!WriteProcessMemory(
		process_handle,
		nt_protect_virtual_memory_addr,
		bytes.data(),
		bytes.size(),
		nullptr
	)) {
		std::cerr << "failed to restore nt_protect_virtual_memory original bytes\n";
		std::cin.get();
		return 1;
	}

	// inject dll
	LPVOID remote_address = VirtualAllocEx(process_handle, NULL, dll_path_len + 1, MEM_COMMIT, PAGE_READWRITE);
	if (!remote_address) {
		std::cerr << "failed to allocate memory in remote process\n";
		std::cin.get();
		return 1;
	}

	WriteProcessMemory(process_handle, remote_address, dll_path, dll_path_len + 1, NULL);

	HMODULE kernel32_module = GetModuleHandleA("kernel32.dll");
	if (!kernel32_module) {
		std::cerr << "failed to get module handle for kernel32.dll\n";
		std::cin.get();
		return 1;
	}

	LPTHREAD_START_ROUTINE thread_start_routine = (LPTHREAD_START_ROUTINE)GetProcAddress(kernel32_module, "LoadLibraryA");

	HANDLE remote_thread = CreateRemoteThread(process_handle, NULL, 0, thread_start_routine, remote_address, 0, NULL);
	if (remote_thread) {
		WaitForSingleObject(remote_thread, INFINITE);
		CloseHandle(remote_thread);
	}

	if (remote_address) {
		VirtualFreeEx(process_handle, remote_address, 0, MEM_RELEASE);
	}

	CloseHandle(process_handle);
	CloseHandle(process_info.hThread);
	return 0;
}
