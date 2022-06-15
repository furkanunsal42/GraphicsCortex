#include "GL\glew.h"
#include "Frame.h"
#include "Debuger.h"
#include <iostream>

namespace frame {
	long old_time = time(nullptr);
	int fps = 0;
	bool is_glew_initialized = false;
	bool is_glfw_initialized = false;

	GLFWwindow* create_window(int width, int height, std::string name, int swapinterval) {
		if (!is_glfw_initialized) {
			glfwInit();
			is_glfw_initialized = true;
		}

		GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(swapinterval);

		if (!is_glew_initialized) {
			glewInit();
			is_glew_initialized = true;
		}

		return window;
	}

	void clear_window(float red, float green, float blue, float alpha) {
		if (!is_glfw_initialized || !is_glew_initialized)
			return;

		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glClearColor(red, green, blue, alpha));
	}

	void display_fps() {
		fps++;
		long now = std::time(nullptr);
		if (now - old_time >= 1) {
			std::cout << fps << std::endl;
			fps = 0;
			old_time = now;
		}
	}
}