#include "GraphicsCortex.h"


namespace defualt_gui_styles {
	void dark_theme() {
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize = ImVec2(350, 100);
	}
}

struct _ui_object_data {
	std::string name;
	bool selected;
};

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 0, 1, true, false, true);
	
	defualt_gui_styles::dark_theme();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	std::vector<_ui_object_data> objects{ {"Cube1"}, {"Cube2"}, {"Cube3"}, {"Sphere1"}};
	char* objects_search_text = new char[60]{""};
	
	std::vector<std::string> insertable{ "Graphic", "Physics", "Object", "Light", "Shader", "Vehicle", "Framebuffer" };
	char* insert_search_text = new char[60]{ "" };

	while (frame.is_running()) {
		double frametime = frame.handle_window();
		frame.clear_window();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
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
		ImGui::InputText("Search", objects_search_text, 59);

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

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}