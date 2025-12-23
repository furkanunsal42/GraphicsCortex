#pragma once

#include <filesystem>
#include "Window.h"

class ImGuiLayer {
public:
	static void init(Window& window, const std::filesystem::path& layout_file_path = "", const std::string& layout_file_name = "imgui.ini");
	static void begin_dockspace(glm::ivec2 window_size);
	static void begin_dockspace(glm::ivec2 dockspace_begin, glm::ivec2 dockspace_end);
	static void end_dockspace();
	static void start_new_frame();
	static void render_new_frame();
	static void release();
};
