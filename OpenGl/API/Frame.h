#pragma once
#include <string>
#include <GLFW\glfw3.h>
namespace frame {
	extern bool is_glew_initialized;
	extern bool is_glfw_initialized;

	GLFWwindow* create_window(int width, int height, std::string name, int swapinterval = 1);

	void clear_window(float red = 0, float green = 0, float blue = 0, float alpha = 1);
}