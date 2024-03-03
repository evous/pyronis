#include <menu/menu.hpp>
#include <game/gt.hpp>
#include <utils/console.hpp>

#include <sstream>
#include <format>
#include <utils/memory.hpp>

void menu::setup() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	ImGui_ImplDX9_Init(gt::get_renderer()->device);
	ImGui_ImplWin32_Init(gt::hwnd);

	console::print_good("initialized imgui");
}

void debug_button(const char* name, auto address) {
	std::uintptr_t addr = reinterpret_cast<std::uintptr_t>(address);

	if (ImGui::Button(std::format("copy##{}", name).c_str()))
		ImGui::SetClipboardText(std::format("{:x}", addr).c_str());

	ImGui::SameLine();

	ImGui::Text("%s: 0x%llx", name, addr);
}

void menu::render() noexcept {
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (show) {
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetNextWindowPos({ 10.f, 10.f }, ImGuiCond_Once);
		ImGui::SetNextWindowSize({ 500.f, 300.f }, ImGuiCond_Once);
		ImGui::Begin("pyronis", &show, ImGuiWindowFlags_NoCollapse);

		app_t* app = gt::get_app();
		enet_client_t* client = gt::get_client();
		renderer_context_d3d9_t* renderer = gt::get_renderer();

		ImGui::SeparatorText("debug test");

		debug_button("app", app);
		debug_button("renderer", renderer);
		debug_button("enet client", client);

		if (client->peer) {
			ImGui::SeparatorText("enet client test");
			ImGui::Text("server: %s:%u", client->server_ip, client->server_port);
			ImGui::Text("user: %d", client->user);
			ImGui::Text("token: %d", client->token);
			ImGui::Text("uuid token: %s", client->uuid_token);
		}

		ImGui::SeparatorText("app & baseapp test");

		if (app) {
			ImGui::Text("fps: %d", app->game_timer.fps);
			ImGui::Checkbox("show/hide fps", &app->fps_visible);
		}

		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}