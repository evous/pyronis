#include <hooks/hooks.hpp>
#include <menu/menu.hpp>
#include <utils/console.hpp>
#include <minhook/include/MinHook.h>

#include <stdexcept>
#include <format>
#include <sstream>
#include <regex>

void hook_function(auto& function, const auto& hook_function) {
	void* address = function;
	void* original = nullptr;

	if (MH_CreateHook(address, hook_function, &original) != MH_OK)
		throw std::runtime_error("failed to create hook");

	if (MH_EnableHook(address) != MH_OK)
		throw std::runtime_error("failed to enable hook");

	function = reinterpret_cast<decltype(function)>(original);
}

void hooks::install() {
	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize minhook");

	hook_function(gt::end_scene, end_scene_hook);
	original_wnd_proc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(gt::hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&wnd_proc_hook)));

	console::print_good("installed hooks");
}

HRESULT hooks::end_scene_hook(IDirect3DDevice9* _this) {
	menu::render();
	return gt::end_scene(_this);
}

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

LRESULT hooks::wnd_proc_hook(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
        return TRUE;

    if (msg == WM_KEYUP && wparam == VK_INSERT) {
        menu::show = !menu::show;
        return TRUE;
    }

    const ImGuiIO& io = ImGui::GetIO();

    bool mouse =
        msg == WM_LBUTTONDOWN ||
        msg == WM_RBUTTONDOWN ||
        msg == WM_LBUTTONDBLCLK ||
        msg == WM_RBUTTONDBLCLK ||
        msg == WM_MOUSEWHEEL;

    if (io.WantTextInput || (io.WantCaptureMouse && mouse))
        return TRUE;

    return CallWindowProc(original_wnd_proc, hwnd, msg, wparam, lparam);
}
