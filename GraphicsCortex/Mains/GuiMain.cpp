// #include "GraphicsCortex.h"
// 
// int main() {
// 	Frame frame(1920, 1080, "GraphicsCortex", 16, 0, false, true, false, false);
// 	Scene scene;
// 	Ui ui(frame);
// 
// 	Style toggle;
// 	toggle.color = Vec3<float>(0.2f, 0.2f, 0.2f);
// 	toggle.corner_rounding = Vec4<float>(50, 50, 50, 50);
// 	toggle.margin = Vec4<float>(20, 20, 20, 20);
// 	toggle.padding = Vec4<float>(50, 0, 0, 0);
// 	toggle.on_hover.cursor_type = Frame::Hand;
// 	toggle.on_active.color = Vec3<float>(0.3, 0.3, 0.3);
// 	
// 	Style background;
// 	background.color = Vec3<float>(0.6, 0.6, 0.6);
// 
// 	while (frame.is_running()) {
// 		double frame_time = frame.handle_window();
// 		frame.clear_window(1, 1, 1, 1);
// 		frame.display_performance(180);
// 		ui.new_frame();
// 
// 		scene.render();
// 		
// 		ui.begin(background, Layout::Horizional);
// 		ui.box("box1", Vec2<float>(200, 200), toggle);
// 		ui.end();
// 	}
// }

#include "GraphicsCortex.h"


namespace defualt_gui_styles {
	void default_theme() {
		auto ColorFromBytes = [](uint8_t r, uint8_t g, uint8_t b)
		{
			return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
		};

		auto& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		const ImVec4 bgColor = ColorFromBytes(37, 37, 38);
		const ImVec4 lightBgColor = ColorFromBytes(82, 82, 85);
		const ImVec4 veryLightBgColor = ColorFromBytes(90, 90, 95);

		const ImVec4 panelColor = ColorFromBytes(51, 51, 55);
		const ImVec4 panelHoverColor = ColorFromBytes(29, 151, 236);
		const ImVec4 panelActiveColor = ColorFromBytes(0, 119, 200);

		const ImVec4 textColor = ColorFromBytes(255, 255, 255);
		const ImVec4 textDisabledColor = ColorFromBytes(151, 151, 151);
		const ImVec4 borderColor = ColorFromBytes(78, 78, 78);

		colors[ImGuiCol_Text] = textColor;
		colors[ImGuiCol_TextDisabled] = textDisabledColor;
		colors[ImGuiCol_TextSelectedBg] = panelActiveColor;
		colors[ImGuiCol_WindowBg] = bgColor;
		colors[ImGuiCol_ChildBg] = bgColor;
		colors[ImGuiCol_PopupBg] = bgColor;
		colors[ImGuiCol_Border] = borderColor;
		colors[ImGuiCol_BorderShadow] = borderColor;
		colors[ImGuiCol_FrameBg] = panelColor;
		colors[ImGuiCol_FrameBgHovered] = panelHoverColor;
		colors[ImGuiCol_FrameBgActive] = panelActiveColor;
		colors[ImGuiCol_TitleBg] = bgColor;
		colors[ImGuiCol_TitleBgActive] = bgColor;
		colors[ImGuiCol_TitleBgCollapsed] = bgColor;
		colors[ImGuiCol_MenuBarBg] = panelColor;
		colors[ImGuiCol_ScrollbarBg] = panelColor;
		colors[ImGuiCol_ScrollbarGrab] = lightBgColor;
		colors[ImGuiCol_ScrollbarGrabHovered] = veryLightBgColor;
		colors[ImGuiCol_ScrollbarGrabActive] = veryLightBgColor;
		colors[ImGuiCol_CheckMark] = panelActiveColor;
		colors[ImGuiCol_SliderGrab] = panelHoverColor;
		colors[ImGuiCol_SliderGrabActive] = panelActiveColor;
		colors[ImGuiCol_Button] = panelColor;
		colors[ImGuiCol_ButtonHovered] = panelHoverColor;
		colors[ImGuiCol_ButtonActive] = panelHoverColor;
		colors[ImGuiCol_Header] = panelColor;
		colors[ImGuiCol_HeaderHovered] = panelHoverColor;
		colors[ImGuiCol_HeaderActive] = panelActiveColor;
		colors[ImGuiCol_Separator] = borderColor;
		colors[ImGuiCol_SeparatorHovered] = borderColor;
		colors[ImGuiCol_SeparatorActive] = borderColor;
		colors[ImGuiCol_ResizeGrip] = bgColor;
		colors[ImGuiCol_ResizeGripHovered] = panelColor;
		colors[ImGuiCol_ResizeGripActive] = lightBgColor;
		colors[ImGuiCol_PlotLines] = panelActiveColor;
		colors[ImGuiCol_PlotLinesHovered] = panelHoverColor;
		colors[ImGuiCol_PlotHistogram] = panelActiveColor;
		colors[ImGuiCol_PlotHistogramHovered] = panelHoverColor;
		//colors[ImGuiCol_ModalWindowDarkening] = bgColor;
		colors[ImGuiCol_DragDropTarget] = bgColor;
		colors[ImGuiCol_NavHighlight] = bgColor;
		//colors[ImGuiCol_DockingPreview] = panelActiveColor;
		colors[ImGuiCol_Tab] = bgColor;
		colors[ImGuiCol_TabActive] = panelActiveColor;
		colors[ImGuiCol_TabUnfocused] = bgColor;
		colors[ImGuiCol_TabUnfocusedActive] = panelActiveColor;
		colors[ImGuiCol_TabHovered] = panelHoverColor;

		style.WindowRounding = 0.0f;
		style.ChildRounding = 0.0f;
		style.FrameRounding = 0.0f;
		style.GrabRounding = 0.0f;
		style.PopupRounding = 0.0f;
		style.ScrollbarRounding = 0.0f;
		style.TabRounding = 0.0f;
	}
}

struct _ui_object_data {
	std::string name;
	bool selected;
};

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 0, 1, true, false, true);

	defualt_gui_styles::default_theme();
	ImGuiIO& io = ImGui::GetIO(); // (void)io;
	ImFont* font = io.Fonts->AddFontFromFileTTF("Roboto-Thin.ttf", 12);

	std::vector<_ui_object_data> objects{ {"Cube1"}, {"Cube2"}, {"Cube3"}, {"Sphere1"} };
	char* objects_search_text = new char[60]{ "" };

	std::vector<std::string> insertable{ "Graphic", "Physics", "Object", "Light", "Shader", "Vehicle", "Framebuffer" };
	char* insert_search_text = new char[60]{ "" };

	while (frame.is_running()) {
		double frametime = frame.handle_window();


		frame.clear_window();

		ImGui::PushFont(font);
		// scene viewer
		ImGui::Begin("##Scene Viewer", 0, ImGuiWindowFlags_NoTitleBar);
		ImGui::TextColored(ImColor(255, 255, 255), "Scene Viewer");
		ImGui::BeginChild("##Object List", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 25), true);
		ImGui::InputText("Search", objects_search_text, 59);

		for (_ui_object_data& object : objects) {
			if (ImGui::Selectable(object.name.c_str(), &object.selected)) {

				if (!glfwGetKey(frame.window, GLFW_KEY_LEFT_CONTROL)) {
					for (auto& object2 : objects)
						object2.selected = false;
				}
				object.selected = true;

			}
			if (object.selected) {
				ImGui::SetItemDefaultFocus();
			}

		}
		ImGui::EndChild();
		ImGui::Button("Import", ImVec2(60, 20));
		ImGui::SameLine();
		ImGui::Button("Delete", ImVec2(60, 20));
		ImGui::End();


		// objcet properties
		ImGui::Begin("##Properties", 0, ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("Properties");
		ImGui::BeginChild("##Object Properties", ImGui::GetContentRegionAvail(), true);
		ImGui::EndChild();
		ImGui::End();



		// Insert Object
		ImGui::Begin("##Insert", 0, ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("Insert");
		ImGui::BeginChild("##Insert Object", ImGui::GetContentRegionAvail(), true);
		ImGui::InputText("Search", insert_search_text, 59);

		for (std::string& option : insertable) {
			bool selected = false;
			ImGui::Selectable(option.c_str(), &selected, 0, ImVec2(ImGui::GetContentRegionAvail().x, 30));
		}
		ImGui::EndChild();
		ImGui::End();


		// File Explorer
		ImGui::Begin("##Explorer", 0, ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("File Explorer");
		ImGui::BeginChild("##File Explorer", ImGui::GetContentRegionAvail(), true);
		ImGui::EndChild();
		ImGui::End();


		// Control Menu
		ImGui::Begin("##Control", 0, ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("Control Menu");
		ImGui::Button("Play", ImVec2(60, 30));
		ImGui::SameLine(0, 20);
		ImGui::Button("Pause", ImVec2(60, 30));
		ImGui::SameLine(0, 20);
		ImGui::Button("Restart", ImVec2(60, 30));
		ImGui::End();


		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New")) {
				}
				if (ImGui::MenuItem("Open Recent")) {
				}
				if (ImGui::MenuItem("Save")) {
				}
				if (ImGui::MenuItem("Save as")) {
				}
				ImGui::EndMenu();

			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::MenuItem("Undo");
				ImGui::MenuItem("Redo");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				ImGui::MenuItem("Control Menu");
				ImGui::MenuItem("File Explorer");
				ImGui::MenuItem("Insert");
				ImGui::MenuItem("Properties");
				ImGui::MenuItem("Scene Viewer");
				ImGui::EndMenu();
			}



			ImGui::EndMainMenuBar();
		}
		ImGui::PopFont();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}