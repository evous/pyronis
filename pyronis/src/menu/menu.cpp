#include <menu/menu.hpp>
#include <game/gt.hpp>
#include <utils/console.hpp>

#include <sstream>
#include <format>

void menu::setup() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

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

		renderer_context_d3d9_t* renderer = gt::get_renderer();
		enet_client_t* client = gt::get_client();

		debug_button("enet client", client);
		debug_button("renderer", renderer);

		ImGui::SeparatorText("test");

		if (client->peer) {
			ImGui::Text("server: %s:%d", client->server_ip.c_str(), client->server_port);
			ImGui::Text("user: %d", client->user);
			ImGui::Text("token: %d", client->token);
			ImGui::Text("uuid token: %s", client->uuid_token);
			ImGui::Text("door id: %s", client->door_id);
		}

		ImGui::Text("application average %.3f ms/frame (%.1f fps)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}