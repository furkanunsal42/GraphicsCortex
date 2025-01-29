#pragma once

#include <cstdint>
#include <string>
#include "vec2.hpp"

class Monitor;

class Window {

	Window(
		uint32_t window_width, uint32_t window_height,
		const std::string& window_name,
		const Monitor& full_screen_target_monitor
	);
	Window(
		uint32_t window_width, uint32_t window_height,
		const std::string& window_name
	);

	static Window create_from_current();

	Window(const Window& other) = delete;
	~Window();

	void release();

	// windowing
	bool should_close();
	void set_should_close(bool value);

	void swap_buffers();
	void poll_events();
	void wait_events();
	double handle_window_events(bool print_performance = true);

	glm::ivec2 get_window_size();
	glm::ivec2 get_window_size_decorated();

	void on_window_should_close();
	void on_window_size_change();


	// input handling



protected:
	void _initialize();
	bool _is_frame_initialized = false;
	void* glfw_window = nullptr;
	void* glfw_window_shared = nullptr;

	// windowing
	uint32_t w_width;
	uint32_t w_height;
	std::string w_name;
	void* w_fullscreen_monitor_ptr = nullptr;
	bool w_resizable = true;
	bool w_visible = true;
	bool w_decorated = true;
	bool w_focus_on_create = false;
	bool w_auto_iconify_when_fullscreen = true;
	bool w_floating = false;
	bool w_begin_maximized = false;
	bool w_transparent = false;
	bool w_focus_on_show = true;

	// input handling
	bool i_begin_cursor_centered = false;

};