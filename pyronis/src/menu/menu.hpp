#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_dx9.h>
#include <imgui/imgui_impl_win32.h>

#include <cstdint>
#include <string_view>

namespace menu {
	void setup();
	void render() noexcept;

	inline bool show = true;
}