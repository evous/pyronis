#pragma once
#include <game/gt.hpp>

#include <string>
#include <cstdint>
#include <d3d9.h>

namespace hooks {
	void install();

	HRESULT end_scene_hook(IDirect3DDevice9* _this);
	LRESULT wnd_proc_hook(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	inline WNDPROC original_wnd_proc = nullptr;
}
