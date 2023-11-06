#pragma once
#include "Config.h"

#include <string>
#include "CortexMath.h"

extern bool is_glew_initialized;
extern bool is_glfw_initialized;
extern int fps_counter_batch;
extern double frame_time_ms;
extern double frame_rate_fps;

struct GLFWwindow;

class Frame {
public:
	int multisample;
	int window_width;
	int window_height;
	std::string window_name;
	int swapinterval;
	bool depth_test, blend, face_culling, initialize_imgui;
	GLFWwindow* window;

	Frame(int width, int height, const std::string& name = "GraphicsCortex", int msaa = 0, int swapinterval = 1, bool depth_test = true, bool blend = false, bool face_culling = true, bool initialize_gui = true);
	~Frame();

	GLFWwindow* create_window(int width, int height, std::string name, int msaa = 0, int swapinterval = 1, bool depth_test = true, bool blend = false, bool face_culling = true);
	void clear_window(float red = 0, float green = 0, float blue = 0, float alpha = 1);
	void display_performance(int batch_size = NULL);
	double get_interval_ms();
	bool is_running();
	double handle_window();
	
	enum CursorType {
		Arrow = 0,
		IBeam,
		Crosshair,
		Hand,
		HResize,
		VResize,
		Disabled,
	};
	
	enum CursorState {
		LeftPressed,
		LeftReleased,
		RightPressed,
		RightReleased,
	};

	Vec2<int> get_cursor_position();
	void set_cursor_type(Frame::CursorType cursor_type = Frame::CursorType::Arrow);
	CursorType get_cursor_type();
	CursorState get_mouse_state();
	double get_time_sec();

private:

	CursorType _current_cursor_type;
	void _window_resize_callback(GLFWwindow* window, int width, int height);
};