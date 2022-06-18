#pragma once
#include <string>
#include <GLFW\glfw3.h>
namespace frame {
	extern long old_time;
	extern int fps;
	extern bool is_glew_initialized;
	extern bool is_glfw_initialized;
	extern GLFWwindow* create_window(int width, int height, std::string name, int swapinterval = 1, bool depth_test = false, bool blend = false);
	extern void clear_window(float red = 0, float green = 0, float blue = 0, float alpha = 1);
	extern void display_fps();
}