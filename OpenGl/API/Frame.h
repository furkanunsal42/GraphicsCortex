#pragma once
#include "Config.h"

#include <string>
#include <GLFW\glfw3.h>

extern bool is_glew_initialized;
extern bool is_glfw_initialized;
extern int fps_counter_batch;
extern double frame_time_ms;
extern double frame_rate_fps;

class Frame {
public:
	int multisample;
	int window_width;
	int window_height;
	std::string window_name;
	int swapinterval;
	bool depth_test, blend, face_culling;
	GLFWwindow* window;

	Frame(int width, int height, const std::string& name = "GraphicsCortex", int msaa = 0, int swapinterval = 1, bool depth_test = false, bool blend = false, bool face_culling = true);
	~Frame();

	GLFWwindow* create_window(int width, int height, std::string name, int msaa = 0, int swapinterval = 1, bool depth_test = false, bool blend = false, bool face_culling = true);
	void clear_window(float red = 0, float green = 0, float blue = 0, float alpha = 1);
	void display_performance(int batch_size = NULL);
	double get_interval_ms();
private:
};