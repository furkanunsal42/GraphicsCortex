#pragma once

#include <cstdint>
#include <string>
#include "vec2.hpp"
#include "vec4.hpp"
#include "Newsletter.h"
#include <memory>
#include <filesystem>
#include <array>

class Image;
class Monitor;

struct WindowDescription;

class Window {
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

	Window(
		const WindowDescription& description
	);

	Window(
		glm::ivec2 window_resolution,
		const std::string& window_name,
		Monitor& full_screen_target_monitor,
		GraphicsAPI api = OpenGL, uint32_t context_version_major = 4, uint32_t context_version_minor = 6

	);
	Window(
		glm::ivec2 window_resolution,
		const std::string& window_name,
		Window& shared_context,
		GraphicsAPI api = OpenGL, uint32_t context_version_major = 4, uint32_t context_version_minor = 6
	);

	Window(
		glm::ivec2 window_resolution,
		const std::string& window_name,
		Monitor& full_screen_target_monitor,
		Window& shared_context,
		GraphicsAPI api = OpenGL, uint32_t context_version_major = 4, uint32_t context_version_minor = 6

	);
	Window(
		glm::ivec2 window_resolution,
		const std::string& window_name,
		GraphicsAPI api = OpenGL, uint32_t context_version_major = 4, uint32_t context_version_minor = 6
	);

	static std::shared_ptr<Window> create_from_current();

	Window(const Window& other) = delete;
	Window(Window&& other);
	Window& operator=(Window& other) = delete;
	Window& operator=(Window&& other);
	~Window();

	void release();

	// events
	bool should_close();
	void set_should_close(bool value);

	void swap_buffers();
	void poll_events();
	void wait_events();
	void wait_events_timeout(double timeout_seconds);
	void post_empty_event();
	double handle_events(bool print_performance = true);
	void* get_handle();

	double get_and_reset_deltatime();

	// graphics context
	void context_make_current();

	uint32_t get_context_version_major();
	uint32_t get_context_version_minor();
	GraphicsAPI get_context_api();
	bool get_context_forward_compatibility();
	//bool get_context_debug_mode();
	OpenGLProfile get_context_profile();

	// default framebuffer
	glm::ivec2 get_framebuffer_resolution();
	glm::ivec4 get_framebuffer_color_bits();
	glm::ivec2 get_framebuffer_depth_stencil_bits();
	int32_t get_framebuffer_multisample_count();

	void set_framebuffer_swap_interval(int32_t value);

	// window
	Monitor get_fullscreen_monitor();
	void set_fullscreen_monitor(const Monitor& monitor);
	void set_fullscreen_monitor(const Monitor& monitor, glm::ivec2 position, glm::ivec2 resolution, int32_t refresh_rate);
	void disable_fullscreen();

	glm::ivec2 get_window_resolution();
	//glm::ivec2 get_window_resolution_undecorated();
	void set_window_resolution(glm::ivec2 window_resolution);

	glm::vec2 get_window_content_scale();
	void set_window_resolution_limits(glm::ivec2 min_resolution, glm::ivec2 max_resolution);
	void set_aspect_ratio(int numerator, int denumerator);

	void set_window_position(glm::ivec2 position);
	glm::ivec2 get_window_position();

	std::string get_window_name();
	void set_window_name(const std::string window_name);
	void set_window_icon(Image& icon);
	void set_default_window_icon();

	bool is_window_minimized();
	bool is_window_maximized();
	bool is_window_restored();
	void window_minimize();
	void window_maximize();
	void window_restore();

	bool is_window_visible();
	void set_window_visibility(bool value);

	bool is_window_focused();
	void window_focus();
	void window_request_attention();

	void set_window_opacity(float opacity);
	float get_window_opacity();

	bool is_window_resizable();
	void set_window_resizable(bool value);

	bool is_window_always_on_top();
	void set_window_always_on_top(bool value);

	bool is_window_auto_iconify_when_fullscreen();
	void set_window_auto_iconify_when_fullscreen(bool value);

	bool is_window_focus_on_show();
	void set_window_focus_on_show(bool value);

	bool is_window_decorated();
	void set_window_decorated(bool value);

	// input handling

	// newsletters
	struct NewslettersBlock;
	NewslettersBlock* newsletters = nullptr;

	enum class IconifyResult {
		ICONIFIED,
		RESTORED,
	};
	enum class MaximizeResult {
		MAXIMIZED,
		RESTORED,
	};
	enum class FocusResult {
		FOCUS_GAINED,
		FOCUS_LOST,
	};
	enum class CursorEnterResult {
		CURSOR_ENTERED,
		CURSOR_EXITED,
	};
	//enum class JoystickConnectionResult {
	//	JOYSTICK_CONNECTED,
	//	JOYSTICK_DISCONNECTED,
	//};

	enum class Key {
		SPACE,
		APOSTROPHE,
		COMMA,
		MINUS,
		PERIOD,
		SLASH,
		NUM_0,
		NUM_1,
		NUM_2,
		NUM_3,
		NUM_4,
		NUM_5,
		NUM_6,
		NUM_7,
		NUM_8,
		NUM_9,
		SEMICOLON,
		EQUAL,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		LEFT_BRACKET,
		BACKSLASH,
		RIGHT_BRACKET,
		GRAVE_ACCENT,
		WORLD_1,
		WORLD_2,
		ESCAPE,
		ENTER,
		TAB,
		BACKSPACE,
		INSERT,
		DELETE,
		RIGHT,
		LEFT,
		DOWN,
		UP,
		PAGE_UP,
		PAGE_DOWN,
		HOME,
		END,
		CAPS_LOCK,
		SCROLL_LOCK,
		NUM_LOCK,
		PRINT_SCREEN,
		PAUSE,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		F16,
		F17,
		F18,
		F19,
		F20,
		F21,
		F22,
		F23,
		F24,
		F25,
		KP_0,
		KP_1,
		KP_2,
		KP_3,
		KP_4,
		KP_5,
		KP_6,
		KP_7,
		KP_8,
		KP_9,
		KP_DECIMAL,
		KP_DIVIDE,
		KP_MULTIPLY,
		KP_SUBTRACT,
		KP_ADD,
		KP_ENTER,
		KP_EQUAL,
		LEFT_SHIFT,
		LEFT_CONTROL,
		LEFT_ALT,
		LEFT_SUPER,
		RIGHT_SHIFT,
		RIGHT_CONTROL,
		RIGHT_ALT,
		RIGHT_SUPER,
		MENU,

		KEY_COUNT,
	};

	static int32_t Key_to_GLFW(Key key);
	static Key GLFW_to_KEY(int32_t glfw_key);

	enum class MouseButton {
		BUTTON1 = 0,
		LEFT = MouseButton::BUTTON1,
		BUTTON2 = 1,
		RIGHT = MouseButton::BUTTON2,
		BUTTON3 = 2,
		BUTTON4 = 3,
		MIDDLE = MouseButton::BUTTON3,
		BUTTON5 = 4,
		BUTTON6 = 5,
		BUTTON7 = 6,
		BUTTON8 = 7,

		BUTTON_COUNT = 8,
	}; 

	enum class PressAction {
		RELEASE		= 0,
		PRESS		= 1,
		REPEAT		= 2,
	};

	enum class KeyMods {
		SHIFT		= 0x0001,
		CONTROL		= 0x0002,
		ALT			= 0x0004,
		SUPER		= 0x0008,
		CAPS_LOCK	= 0x0010,
		NUM_LOCK	= 0x0020,

		MOD_COUNT	= 6,
	};

	enum class CursorMode {
		NORMAL		= 0x00034001,
		HIDDEN		= 0x00034002,
		DISABLED	= 0x00034003,
		CAPTURED	= 0x00034004,
	};

	struct KeyPressResult {
		Key key;
		int32_t scancode;
		PressAction action;
		KeyMods mods;
	};

	struct MousePressResult{
		MouseButton button;
		PressAction action;
		KeyMods mods;
	};

	void set_sticky_keys(bool value);
	bool get_sticky_keys();
	PressAction get_key(Key key);
	int32_t get_key_scancode(Key key);
	std::u8string get_key_name(Key key);
	
	bool is_raw_mouse_movement_supported();
	bool get_raw_mouse_movement();
	void set_raw_mouse_movement(bool value);
	glm::dvec2 get_cursor_position();
	void set_cursor_position(glm::dvec2 position);
	void set_cursor_mode(CursorMode mode);
	CursorMode get_cursor_mode();

	void set_sticky_mouse_buttons(bool value);
	bool get_sticky_mouse_buttons();
	PressAction get_mouse_button(MouseButton mouse_button);

	bool get_key_press_inpulse(Key key);
	bool get_key_release_inpulse(Key key);
	bool get_mouse_press_inpulse(MouseButton button);
	bool get_mouse_release_inpulse(MouseButton button);


	// TODO : custom and default cursor images

protected:
	Window() = default;
	Window(void* handle);

	struct NewslettersBlock {

		NewslettersBlock();

		Newsletter<>							on_should_close_events;
		Newsletter<const glm::ivec2&>			on_framebuffer_resolution_events;
		Newsletter<const glm::ivec2&>			on_window_resolution_events;
		Newsletter<const glm::vec2&>			on_window_content_scale_events;
		Newsletter<const glm::ivec2&>			on_window_position_events;
		Newsletter<const IconifyResult&>		on_window_iconify_events;
		Newsletter<const MaximizeResult&>		on_window_maximize_events;
		Newsletter<const FocusResult&>			on_window_focus_events;
		Newsletter<>							on_window_refresh_events;

		Newsletter<const KeyPressResult&>		on_key_events;
		Newsletter<const uint32_t&>				on_char_events;
		Newsletter<const glm::dvec2&>			on_cursor_position_events;
		Newsletter<const CursorEnterResult&>	on_cursor_enter_or_leave_events;
		Newsletter<const MousePressResult&>		on_mouse_key_events;
		Newsletter<const glm::dvec2&>			on_mouse_scroll_events;

		Newsletter<const std::vector<std::filesystem::path>&>	on_filepath_drop_events;

		void clear_tables();

		std::array<bool, (size_t)Key::KEY_COUNT>			key_press_table;
		std::array<bool, (size_t)Key::KEY_COUNT>			key_release_table;
		std::array<bool, (size_t)MouseButton::BUTTON_COUNT> mouse_press_table;
		std::array<bool, (size_t)MouseButton::BUTTON_COUNT> mouse_release_table;

		bool is_key_press_table_dirty		= true;
		bool is_key_release_table_dirty		= true;
		bool is_mouse_press_table_dirty		= true;
		bool is_mouse_release_table_dirty	= true;
	};

	void _initialize(const WindowDescription& description);
	constexpr static std::chrono::system_clock::time_point invalid_time = std::chrono::system_clock::time_point::max();

	void* handle = nullptr;
	std::string window_name;
	std::chrono::system_clock::time_point last_handle_events_time = invalid_time;
	std::chrono::system_clock::time_point last_perforamnce_print_time = invalid_time;
	uint32_t frame_count_since_performance_print = 0;
};


struct WindowDescription {
	// graphics context
	void* context_shared = nullptr;
	uint32_t ctx_version_major = 4;
	uint32_t ctx_version_minor = 6;
	Window::GraphicsAPI ctx_api = Window::OpenGL;
	bool ctx_forward_compatibility = false;
	bool ctx_debug_mode = true;
	Window::OpenGLProfile ctx_profile = Window::Compatibility;
	bool ctx_enable_debug_callback_print = false;

	// default framebuffer
	glm::ivec2 f_resolution = glm::ivec2(0, 0);
	glm::ivec4 f_color_bits = glm::ivec4(8, 8, 8, 8);
	glm::ivec2 f_depth_stencil_bits = glm::ivec2(24, 8);
	int32_t f_multisample_count = 0;
	bool f_srgb_enabled = false;
	bool f_double_buffered = true;
	int32_t f_swap_interval = 0;

	// windowing
	glm::ivec2 w_resolution = glm::ivec2(512, 512);
	std::string w_name = "GraphicsCortex Window";
	Monitor* w_fullscreen_monitor_ptr = nullptr;
	bool w_resizable = true;
	bool w_visible = true;
	bool w_decorated = true;
	bool w_auto_iconify_when_fullscreen = true;
	bool w_always_on_top = false;
	bool w_transparent = false;
	bool w_scale_window_size = true;
	bool w_scale_framebuffer_size = true;

	bool w_have_initial_position = false;
	glm::ivec2 w_initial_position = glm::ivec2(0);
	bool w_maximized_on_create = false;
	bool w_focus_on_create = false;
	bool w_focus_on_show = true;
	int32_t w_fullscreen_refresh_rate = 0;

	// input handling
	bool i_mouse_passthrough = false;
	bool i_center_cursor_on_create = false;

	// newsletter
	bool n_create_newsletters = true;
};