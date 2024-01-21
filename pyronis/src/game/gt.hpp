#pragma once
#include <game/network/enet_client.hpp>
#include <game/renderer/renderer_context.hpp>
#include <game/app/app.hpp>
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

	inline app_t* (*get_app)() = nullptr;
	inline enet_client_t* (*get_client)() = nullptr;
	inline void (*set_fps_limit)(base_app_t* _this, float fps) = nullptr;

	inline renderer_context_d3d9_t** renderer = nullptr;

	inline std::uintptr_t enable_pasting_address = 0;
}