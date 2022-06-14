#include <string>

#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "Debuger.h"
namespace frame{
	bool is_glew_initialized = false;
	bool is_glfw_initialized = false;

	GLFWwindow* create_window(int width, int height, std::string name, int swapinterval=1) {
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

	void clear_window(float red = 0, float green = 0, float blue = 0, float alpha = 1) {
		if (!is_glfw_initialized || !is_glew_initialized)
			return;

		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		GLCall(glClearColor(red, green, blue, alpha));
	}
}