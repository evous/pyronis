#pragma once
#include <game/renderer/renderer_context.hpp>
#include <utils/math.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include <windows.h>
#include <psapi.h>

namespace gt {
	void setup();

	renderer_context_d3d9_t* get_renderer() noexcept;

	inline std::uintptr_t base_address = 0;
	inline std::uintptr_t end_address = 0;
	inline HWND hwnd = nullptr;
	inline vec2i_t window_size = { 0, 0 };

	inline HRESULT(*end_scene)(IDirect3DDevice9* _this) = nullptr;

	inline renderer_context_d3d9_t** renderer = nullptr;
}