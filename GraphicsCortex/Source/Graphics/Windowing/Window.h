#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <vec2.hpp>

class Monitor;
class GraphicsCortex;

class Frame2 {
public:

	enum GraphicsAPI {
		OpenGL,
		OpenGL_ES,
		Vulkan,
		None,
	};

	enum OpenGLProfile {
		Core,
		Compatibility,
	};

	Frame2(
		uint32_t window_width, uint32_t window_height,
		const std::string& window_name,
		const Monitor& full_screen_target_monitor,
		GraphicsAPI api = OpenGL, uint32_t context_version_major = 4, uint32_t context_version_minor = 6
	);
	Frame2(
		uint32_t window_width, uint32_t window_height,
		const std::string& window_name,
		const Frame2& shared_context,
		const Monitor& full_screen_target_monitor,
		GraphicsAPI api = OpenGL, uint32_t context_version_major = 4, uint32_t context_version_minor = 6
	);
	Frame2(
		uint32_t window_width, uint32_t window_height,
		const std::string& window_name,
		GraphicsAPI api = OpenGL, uint32_t context_version_major = 4, uint32_t context_version_minor = 6
	);
	Frame2(
		uint32_t window_width, uint32_t window_height,
		const std::string& window_name,
		const Frame2& shared_context,
		GraphicsAPI api = OpenGL, uint32_t context_version_major = 4, uint32_t context_version_minor = 6
	);

	static Frame2 create_from_current();

	Frame2(const Frame2& other) = delete;
	~Frame2();

	void release();

	// graphics context
	void context_make_current();

	uint32_t get_context_version_major();
	uint32_t get_context_version_minor();

	void set_context_forward_compatibility(bool value);
	void set_context_debug_mode(bool value);
	void set_context_profile(OpenGLProfile profile);


	// default framebuffer
	bool is_double_buffered();
	void set_double_buffer(bool value);


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


private:

	

	void _initialize();
	bool _is_frame_initialized = false;
	void* glfw_window = nullptr;
	void* glfw_window_shared = nullptr;


	// graphics context
	uint32_t ctx_version_major;
	uint32_t ctx_version_minor;
	GraphicsAPI ctx_api;
	bool ctx_forward_compatibility = false;
	bool ctx_debug_mode = true;
	OpenGLProfile ctx_profile = Compatibility;

	// default framebuffer
	uint32_t f_width;
	uint32_t f_height;


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


	friend GraphicsCortex;
};