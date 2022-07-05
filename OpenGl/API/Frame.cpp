#include "GL\glew.h"
#include "Frame.h"
#include "Debuger.h"
#include <iostream>

namespace frame {
	bool is_glew_initialized = false;
	bool is_glfw_initialized = false;
	int fps_counter_batch = 180;
	double frame_time_ms = 0;
	double frame_rate_fps = 0;
	int multisample = 0;
	// private
	double old_time = glfwGetTime();
	double old_time_accurate = 0;
	int fps_counter_index = 0;
	double seconds_total_batch = 0;

	GLFWwindow* create_window(int width, int height, std::string name, int msaa, int swapinterval, bool depth_test, bool blend, bool face_culling) {
		
		multisample = msaa;

		if (!is_glfw_initialized) {
			glfwInit();
			is_glfw_initialized = true;
		}
		//if(multisample)
			//glfwWindowHint(GLFW_SAMPLES, multisample);

		GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(swapinterval);

		if (!is_glew_initialized) {
			glewInit();
			is_glew_initialized = true;
		}
		

		if (face_culling)
			glEnable(GL_CULL_FACE);
		if (depth_test)
			glEnable(GL_DEPTH_TEST);
		if (blend) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		if (multisample) {
			glEnable(GL_MULTISAMPLE);
		}
		
		return window;
	}

	void clear_window(float red, float green, float blue, float alpha) {
		if (!is_glfw_initialized || !is_glew_initialized)
			return;

		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glClearColor(red, green, blue, alpha));
	}

	void display_performance(int batch_size) {
		if (batch_size != NULL && fps_counter_batch != batch_size)
			fps_counter_batch = batch_size;

		double now = glfwGetTime();
		double seconds_current = (now - old_time);
		if (fps_counter_index == fps_counter_batch) {
			frame_time_ms = seconds_total_batch / fps_counter_index * 1000;
			frame_rate_fps = 1 / ((seconds_total_batch / fps_counter_batch));
			std::string ms = std::to_string(frame_time_ms);
			std::string fps = std::to_string(frame_rate_fps);
			std::cout << "[Opengl Info] ms:" + ms.substr(0, ms.find(".") + 3) + " fps: " + fps.substr(0, fps.find(".") + 3) << std::endl;
			fps_counter_index = 0;
			seconds_total_batch = 0;
		}
		seconds_total_batch += seconds_current;
		fps_counter_index++;
		old_time = now;
	}

	
	double get_interval_ms() {
		if (old_time_accurate == 0)
			old_time_accurate = glfwGetTime();
		double now = glfwGetTime();
		double ms_current = (now - old_time_accurate) * 1000;
		old_time_accurate = now;
		return ms_current;
	}
}