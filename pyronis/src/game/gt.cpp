#include <game/gt.hpp>
#include <utils/console.hpp>
#include <utils/memory.hpp>

#include <stdexcept>
#include <string>
#include <format>
#include <fstream>

enum class find_mode {
	normal,
	call,
	load,
	function_start
};

void find_address(auto& dest, std::string_view pattern, find_mode mode, std::intptr_t offset = 0) {
	std::uintptr_t address = memory::find_pattern(pattern, offset);

	switch (mode) {
	case find_mode::call:
		address = memory::get_address_from_call(address);
		break;

	case find_mode::load:
		address = memory::get_address_from_load(address);
		break;

	case find_mode::function_start:
		address = memory::find_function_start(address);
		break;

	default:
		break;
	}

	if (address == 0)
		throw std::runtime_error(std::format("pattern '{}' not found", pattern));

	dest = reinterpret_cast<decltype(dest)>(address);
}

void find_addresses() {
	find_address(gt::renderer, "48 8B 05 ? ? ? ? 41 83 ? ?", find_mode::load, 3);

	gt::end_scene = reinterpret_cast<decltype(gt::end_scene)>((*reinterpret_cast<void***>(gt::get_renderer()->device_ex))[42]);
}


void gt::setup() {
	using namespace console;

	MODULEINFO module_info = {};
	if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &module_info, sizeof(MODULEINFO)))
		throw std::runtime_error("failed to get module information");

	base_address = reinterpret_cast<std::uintptr_t>(module_info.lpBaseOfDll);
	end_address = base_address + module_info.SizeOfImage;
	if (base_address == 0 || end_address == 0)
		throw std::runtime_error("invalid module address");

	hwnd = FindWindow(nullptr, "Growtopia");
	if (hwnd == nullptr)
		throw std::runtime_error("growtopia window not found");

	RECT rect = {};
	if (!GetWindowRect(gt::hwnd, &rect))
		throw std::runtime_error("failed to get window size");

	window_size.x = rect.left - rect.right;
	window_size.y = rect.bottom - rect.top;

	find_addresses();
	print_good("found addresses");
}

renderer_context_d3d9_t* gt::get_renderer() noexcept {
	return *renderer;
}