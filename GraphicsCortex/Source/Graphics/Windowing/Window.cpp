#include "GL/glew.h"
#include "Window.h"
#include "GLFW/glfw3.h"
#include "Library.h"

#include <iostream>
#include "glm.hpp"
#include "Image.h"
#include "Monitor.h"

Window::Window(const WindowDescription& description)
{
	_initialize(description);
}

Window::Window(glm::ivec2 window_resolution, const std::string& window_name, Monitor& full_screen_target_monitor)
{
	WindowDescription description;
	description.w_resolution = window_resolution;
	description.w_name = window_name;
	description.w_fullscreen_monitor_ptr = &full_screen_target_monitor;

	_initialize(description);
}

Window::Window(glm::ivec2 window_resolution, const std::string& window_name)
{
	WindowDescription description;
	description.w_resolution = window_resolution;
	description.w_name = window_name;

	_initialize(description);
}

void Window::_initialize(const WindowDescription& description)
{
	OpenGLBackend::_init_glfw();

	// context
	//glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	//glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	
	glfwWindowHint(GLFW_RESIZABLE,					description.w_resizable ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE,					description.w_visible ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED,					description.w_decorated ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_AUTO_ICONIFY,				description.w_auto_iconify_when_fullscreen ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING,					description.w_always_on_top ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,	description.w_transparent ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR,			description.w_scale_window_size ? GLFW_TRUE : GLFW_FALSE);
	//glfwWindowHint(GLFW_SCALE_FRAMEBUFFER,		description.w_scale_framebuffer_size ? GLFW_TRUE : GLFW_FALSE);
	if (description.w_have_initial_position) {
	//	glfwWindowHint(GLFW_POSITION_X,				description.w_initial_position.x);
	//	glfwWindowHint(GLFW_POSITION_Y,				description.w_initial_position.y);
	}
	glfwWindowHint(GLFW_MAXIMIZED,					description.w_maximized_on_create ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_FOCUSED,					description.w_focus_on_create ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_FOCUS_ON_SHOW,				description.w_focus_on_show ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_REFRESH_RATE,				description.w_fullscreen_refresh_rate);
	//glfwWindowHint(GLFW_MOUSE_PASSTHROUGH,		description.i_mouse_passthrough ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_CENTER_CURSOR,				description.i_center_cursor_on_create ? GLFW_TRUE : GLFW_FALSE);

	glm::ivec2 clipped_resolution = description.w_resolution;
	clipped_resolution = glm::max(clipped_resolution, glm::ivec2(1, 1));

	void* monitor_ptr = nullptr;
	if (description.w_fullscreen_monitor_ptr != nullptr)
		monitor_ptr = (GLFWmonitor*)(description.w_fullscreen_monitor_ptr->monitor_ptr);

	handle = glfwCreateWindow(clipped_resolution.x, clipped_resolution.y, description.w_name.c_str(), (GLFWmonitor*)monitor_ptr, nullptr);
	glfwMakeContextCurrent((GLFWwindow*)handle); // ?

	if (description.n_create_newsletters) {
		newsletters = new NewslettersBlock();
		glfwSetWindowUserPointer((GLFWwindow*)handle, this);

		glfwSetWindowCloseCallback((GLFWwindow*)handle, [](GLFWwindow* window) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_should_close_events.publish();
			}
			});

		glfwSetWindowSizeCallback((GLFWwindow*)handle, [](GLFWwindow* window, int width, int height) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_resolution_events.publish(glm::ivec2(width, height));
			}
			});
		glfwSetWindowContentScaleCallback((GLFWwindow*)handle, [](GLFWwindow* window, float xscale, float yscale) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_content_scale_events.publish(glm::vec2(xscale, yscale));
			}
			});
		glfwSetWindowPosCallback((GLFWwindow*)handle, [](GLFWwindow* window, int xpos, int ypos) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_position_events.publish(glm::ivec2(xpos, ypos));
			}
			});
		glfwSetWindowIconifyCallback((GLFWwindow*)handle, [](GLFWwindow* window, int iconified) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_iconify_events.publish(iconified == GLFW_TRUE ? IconifyResult::ICONIFIED : IconifyResult::RESTORED);
			}
			});
		glfwSetWindowMaximizeCallback((GLFWwindow*)handle, [](GLFWwindow* window, int maximized) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_maximize_events.publish(maximized == GLFW_TRUE ? MaximizeResult::MAXIMIZED : MaximizeResult::RESTORED);
			}
			});
		glfwSetWindowFocusCallback((GLFWwindow*)handle, [](GLFWwindow* window, int focused) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_focus_events.publish(focused == GLFW_TRUE ? FocusResult::FOCUS_GAINED : FocusResult::FOCUS_LOST);
			}
			});
		glfwSetWindowRefreshCallback((GLFWwindow*)handle, [](GLFWwindow* window) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_refresh_events.publish();
			}
			});

		glfwSetKeyCallback((GLFWwindow*)handle, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				Key key_enum = Key(key);
				PressAction press_action =
					action == GLFW_PRESS ? PressAction::PRESS :
					action == GLFW_RELEASE ? PressAction::RELEASE :
					action == GLFW_REPEAT ? PressAction::REPEAT : PressAction::PRESS;
				KeyMods mods_enum = KeyMods(mods);

				newsletters->on_key_events.publish(KeyPressResult{ .key = key_enum, .scancode = scancode, .action = press_action, .mods = mods_enum });
			}
			});
		glfwSetCharCallback((GLFWwindow*)handle, [](GLFWwindow* window, uint32_t codepoint) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_char_events.publish(codepoint);
			}
			});
		glfwSetCursorPosCallback((GLFWwindow*)handle, [](GLFWwindow* window, double xpos, double ypos) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_cursor_position_events.publish(glm::dvec2(xpos, ypos));
			}
			});
		glfwSetCursorEnterCallback((GLFWwindow*)handle, [](GLFWwindow* window, int32_t entered) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_cursor_enter_or_leave_events.publish(entered == GLFW_TRUE ? CursorEnterResult::CURSOR_ENTERED : CursorEnterResult::CURSOR_EXITED);
			}
			});
		glfwSetMouseButtonCallback((GLFWwindow*)handle, [](GLFWwindow* window, int32_t button, int32_t action, int32_t mods) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				MouseButton button_enum = MouseButton(button);
				PressAction press_action =
					action == GLFW_PRESS ? PressAction::PRESS :
					action == GLFW_RELEASE ? PressAction::RELEASE :
					action == GLFW_REPEAT ? PressAction::REPEAT : PressAction::PRESS;
				KeyMods mods_enum = KeyMods(mods);
				newsletters->on_mouse_key_events.publish(MousePressResult{ .button = button_enum, .action = press_action, .mods = mods_enum });
			}
			});
		glfwSetScrollCallback((GLFWwindow*)handle, [](GLFWwindow* window, double xoffset, double yoffset) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_mouse_scroll_events.publish(glm::dvec2(xoffset, yoffset));
			}
			});
		glfwSetDropCallback((GLFWwindow*)handle, [](GLFWwindow* window, int path_count, const char* paths[]) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				std::vector<std::u8string> paths_v;
				for (int i = 0; i < path_count; i++)
					paths_v.push_back((const char8_t*)paths[i]);
				newsletters->on_filepath_drop_events.publish(paths_v);
			}
			});
	}

	// input handling
	glfwSetInputMode((GLFWwindow*)handle, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

	OpenGLBackend::_init_glew();
}

Window::~Window()
{
	release();
}

void Window::release()
{
	if (newsletters != nullptr) {
		delete newsletters;
		newsletters = nullptr;
	}

	if (handle != nullptr) {
		glfwDestroyWindow((GLFWwindow*)handle);
		handle = nullptr;
	}
}

// event handling

bool Window::should_close()
{
	return glfwWindowShouldClose((GLFWwindow*)handle) == GLFW_TRUE;
}

void Window::set_should_close(bool value)
{
	glfwSetWindowShouldClose((GLFWwindow*)handle, value ? GLFW_TRUE : GLFW_FALSE);
}

void Window::swap_buffers()
{
	glfwSwapBuffers((GLFWwindow*)handle);
}

void Window::poll_events()
{
	glfwPollEvents();
}

void Window::wait_events()
{
	glfwWaitEvents();
}

void Window::wait_events_timeout(double timeout_seconds)
{
	glfwWaitEventsTimeout(timeout_seconds);
}

void Window::post_empty_event() {
	glfwPostEmptyEvent();
}

double Window::handle_events(bool print_performances) {

	glfwPollEvents();
	glfwSwapBuffers((GLFWwindow*)handle);

	return 0;
}

// window

Monitor Window::get_fullscreen_monitor()
{
	GLFWmonitor* monitor_ptr = glfwGetWindowMonitor((GLFWwindow*)handle);
	Monitor monitor((void*)monitor_ptr);
	return monitor;
}

void Window::set_fullscreen_monitor(const Monitor& monitor)
{
	set_fullscreen_monitor(monitor, glm::ivec2(0, 0), monitor.get_resolution(), monitor.get_refresh_rate());
}

void Window::set_fullscreen_monitor(const Monitor& monitor, glm::ivec2 position, glm::ivec2 resolution, int32_t refresh_rate)
{
	glfwSetWindowMonitor((GLFWwindow*)handle, (GLFWmonitor*)monitor.monitor_ptr, position.x, position.y, resolution.x, resolution.y, refresh_rate);
}

void Window::disable_fullscreen()
{
	Monitor monitor = get_fullscreen_monitor();
	if (monitor.monitor_ptr == nullptr) return;

	glfwSetWindowMonitor((GLFWwindow*)handle,
		nullptr,
		get_window_position().x, get_window_position().y,
		get_window_resolution().x, get_window_resolution().y,
		0);
}

glm::ivec2 Window::get_window_resolution()
{
	glm::ivec2 resolution;
	glfwGetWindowSize((GLFWwindow*)handle, &resolution.x, &resolution.y);
	return resolution;
}

//glm::ivec2 Window::get_window_resolution_undecorated()
//{
//	//int left, top, right, bottom;
//	//glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);
//}

void Window::set_window_resolution(glm::ivec2 window_resolution)
{
	glfwSetWindowSize((GLFWwindow*)handle, window_resolution.x, window_resolution.y);
}

glm::vec2 Window::get_window_content_scale()
{
	glm::vec2 scale;
	glfwGetWindowContentScale((GLFWwindow*)handle, &scale.x, &scale.y);
	return scale;
}

void Window::set_window_resolution_limits(glm::ivec2 min_resolution, glm::ivec2 max_resolution)
{
	glfwSetWindowSizeLimits((GLFWwindow*)handle, min_resolution.x, min_resolution.y, max_resolution.x, max_resolution.y);
}

void Window::set_aspect_ratio(int numerator, int denumerator)
{
	glfwSetWindowAspectRatio((GLFWwindow*)handle, numerator, denumerator);
}

void Window::set_window_position(glm::ivec2 position)
{
	glfwSetWindowPos((GLFWwindow*)handle, position.x, position.y);
}

glm::ivec2 Window::get_window_position()
{
	glm::ivec2 position;
	glfwGetWindowPos((GLFWwindow*)handle, &position.x, &position.y);
	return position;
}

//std::string Window::get_window_name()
//{
//	const char* title = glfwGetWindowTitle((GLFWwindow*)handle);
//	std::string title_str(title);
//	return title_str;
//}

void Window::set_window_name(const std::string window_name)
{
	glfwSetWindowTitle((GLFWwindow*)handle, window_name.c_str());
}

void Window::set_window_icon(Image& icon)
{
	if (icon.get_channel_count() != 4 || icon.get_byte_per_channel() != 1) {
		std::cout << "[Windowing Error] Window::set_window_icon() is called with incorrect image format. image must be 8bit/channel RGBA" << std::endl;
		__debugbreak();
	}

	GLFWimage image;
	image.width = icon.get_width();
	image.height = icon.get_height();
	image.pixels = icon.get_image_data();

	glfwSetWindowIcon((GLFWwindow*)handle, 1, &image);
}

void Window::set_default_window_icon()
{
	glfwSetWindowIcon((GLFWwindow*)handle, 0, NULL);
}

bool Window::is_window_minimized()
{
	int iconified = glfwGetWindowAttrib((GLFWwindow*)handle, GLFW_ICONIFIED);
	return iconified == GLFW_TRUE;
}

bool Window::is_window_maximized()
{
	int maximized = glfwGetWindowAttrib((GLFWwindow*)handle, GLFW_MAXIMIZED);
	return maximized == GLFW_TRUE;
}

bool Window::is_window_restored()
{
	return !(is_window_minimized() || is_window_maximized());
}

void Window::window_minimize()
{
	glfwIconifyWindow((GLFWwindow*)handle);
}

void Window::window_maximize()
{
	glfwMaximizeWindow((GLFWwindow*)handle);
}

void Window::window_restore()
{
	glfwRestoreWindow((GLFWwindow*)handle);
}

bool Window::is_window_visible()
{
	int visible = glfwGetWindowAttrib((GLFWwindow*)handle, GLFW_VISIBLE);
	return visible == GLFW_TRUE;
}

void Window::set_window_visibility(bool value)
{
	if (value)	glfwShowWindow((GLFWwindow*)handle);
	else		glfwHideWindow((GLFWwindow*)handle);
}

bool Window::is_window_focused()
{
	int focused = glfwGetWindowAttrib((GLFWwindow*)handle, GLFW_FOCUSED);
	return focused == GLFW_TRUE;
}

void Window::window_focus()
{
	glfwFocusWindow((GLFWwindow*)handle);
}

void Window::window_request_attention()
{
	glfwRequestWindowAttention((GLFWwindow*)handle);
}

void Window::set_window_opacity(float opacity)
{
	glfwSetWindowOpacity((GLFWwindow*)handle, opacity);
}

float Window::get_window_opacity()
{
	float opacity = glfwGetWindowOpacity((GLFWwindow*)handle);
	return opacity;
}

bool Window::is_window_resizable()
{
	int value = glfwGetWindowAttrib((GLFWwindow*)handle, GLFW_RESIZABLE);
	return value == GLFW_TRUE;
}

void Window::set_window_resizable(bool value)
{
	glfwSetWindowAttrib((GLFWwindow*)handle, GLFW_RESIZABLE, value ? GLFW_TRUE : GLFW_FALSE);
}

bool Window::is_window_always_on_top()
{
	int value = glfwGetWindowAttrib((GLFWwindow*)handle, GLFW_FLOATING);
	return value == GLFW_TRUE;
}

void Window::set_window_always_on_top(bool value)
{
	glfwSetWindowAttrib((GLFWwindow*)handle, GLFW_FLOATING, value ? GLFW_TRUE : GLFW_FALSE);
}

bool Window::is_window_auto_iconify_when_fullscreen()
{
	int value = glfwGetWindowAttrib((GLFWwindow*)handle, GLFW_AUTO_ICONIFY);
	return value == GLFW_TRUE;
}

void Window::set_window_auto_iconify_when_fullscreen(bool value)
{
	glfwSetWindowAttrib((GLFWwindow*)handle, GLFW_AUTO_ICONIFY, value ? GLFW_TRUE : GLFW_FALSE);
}

bool Window::is_window_focus_on_show()
{
	int value = glfwGetWindowAttrib((GLFWwindow*)handle, GLFW_FOCUS_ON_SHOW);
	return value == GLFW_TRUE;
}

void Window::set_window_focus_on_show(bool value)
{
	glfwSetWindowAttrib((GLFWwindow*)handle, GLFW_FOCUS_ON_SHOW, value ? GLFW_TRUE : GLFW_FALSE);
}

bool Window::is_window_decorated()
{
	int value = glfwGetWindowAttrib((GLFWwindow*)handle, GLFW_DECORATED);
	return value == GLFW_TRUE;
}

void Window::set_window_decorated(bool value)
{
	glfwSetWindowAttrib((GLFWwindow*)handle, GLFW_DECORATED, value ? GLFW_TRUE : GLFW_FALSE);
}

// input handling

void Window::set_sticky_keys(bool value)
{
	glfwSetInputMode((GLFWwindow*)handle, GLFW_STICKY_KEYS, value ? GLFW_TRUE : GLFW_FALSE);
}

bool Window::get_sticky_keys()
{
	int sticky_keys = glfwGetInputMode((GLFWwindow*)handle, GLFW_STICKY_KEYS);
	return sticky_keys == GLFW_TRUE;
}

Window::PressAction Window::get_key(Key key)
{
	int action = glfwGetKey((GLFWwindow*)handle, int32_t(key));

	return	action == GLFW_PRESS ? Window::PressAction::PRESS :
		action == GLFW_RELEASE ? Window::PressAction::RELEASE :
		action == GLFW_REPEAT ? Window::PressAction::REPEAT : Window::PressAction::PRESS;

}

int32_t Window::get_key_scancode(Key key)
{
	return glfwGetKeyScancode(int32_t(key));
}

std::u8string Window::get_key_name(Key key)
{
	return std::u8string((const char8_t*)glfwGetKeyName(int32_t(key), get_key_scancode(key)));
}

bool Window::is_raw_mouse_movement_supported()
{
	return glfwRawMouseMotionSupported();
}

bool Window::get_raw_mouse_movement()
{
	return glfwGetInputMode((GLFWwindow*)handle, GLFW_RAW_MOUSE_MOTION) == GLFW_TRUE;
}

void Window::set_raw_mouse_movement(bool value)
{
	glfwSetInputMode((GLFWwindow*)handle, GLFW_RAW_MOUSE_MOTION, value ? GLFW_TRUE : GLFW_FALSE);
}

glm::dvec2 Window::get_cursor_position()
{
	glm::dvec2 position;
	glfwGetCursorPos((GLFWwindow*)handle, &position.x, &position.y);
	return position;
}

void Window::set_cursor_mode(CursorMode mode)
{
	glfwSetInputMode((GLFWwindow*)handle, GLFW_CURSOR, int32_t(mode));
}

Window::CursorMode Window::get_cursor_mode()
{
	return Window::CursorMode(glfwGetInputMode((GLFWwindow*)handle, GLFW_CURSOR));
}

void Window::set_sticky_mouse_buttons(bool value)
{
	glfwSetInputMode((GLFWwindow*)handle, GLFW_STICKY_MOUSE_BUTTONS, value ? GLFW_TRUE : GLFW_FALSE);
}

bool Window::get_sticky_mouse_buttons()
{
	return glfwGetInputMode((GLFWwindow*)handle, GLFW_STICKY_MOUSE_BUTTONS) == GLFW_TRUE;

}

Window::PressAction Window::get_mouse_button(MouseButton mouse_button)
{
	int state = glfwGetMouseButton((GLFWwindow*)handle, int32_t(mouse_button));
	return Window::PressAction(state);
}
