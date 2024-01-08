#include <game/gt.hpp>
#include <menu/menu.hpp>
#include <hooks/hooks.hpp>
#include <utils/console.hpp>

#include <stdexcept>
#include <windows.h>

void on_injected() noexcept {
	console::setup();
	console::print<console::color::red>("pyronis, growtopia internal [credits: teo]\n\n");

	try {
		gt::setup();
		menu::setup();
		hooks::install();
	}
	catch (const std::runtime_error& error) {
		console::print_error(error.what());
		return;
	}

	console::print<console::color::green>("\nloaded successfully!\n\n");
}

BOOL APIENTRY DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hmodule);
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(on_injected), nullptr, 0, nullptr);
	}
	return TRUE;
}

