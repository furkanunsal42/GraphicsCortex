#include "GL/glew.h"
#include "Window.h"
#include "GLFW/glfw3.h"
#include "Library.h"

#include <iostream>
#include "glm.hpp"

Window::Window(const WindowDescription& description)
{
	_initialize(description);
}

Window::Window(glm::ivec2 window_resolution, const std::string& window_name, const Monitor& full_screen_target_monitor)
{
	WindowDescription description;
	description.w_resolution = window_resolution;
	description.w_name = window_name;
	description.w_fullscreen_monitor_ptr = full_screen_target_monitor.monitor_ptr;

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

	window = glfwCreateWindow(clipped_resolution.x, clipped_resolution.y, "", (GLFWmonitor*)description.w_fullscreen_monitor_ptr, nullptr);
	glfwMakeContextCurrent((GLFWwindow*)window); // ?

	if (description.n_create_newsletters) {
		newsletters = new NewslettersBlock();
		glfwSetWindowUserPointer((GLFWwindow*)window, this);

		glfwSetWindowCloseCallback((GLFWwindow*)window, [](GLFWwindow* window) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_should_close_events.publish();
			}
			});

		glfwSetWindowSizeCallback((GLFWwindow*)window, [](GLFWwindow* window, int width, int height) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_resolution_events.publish(glm::ivec2(width, height));
			}
			});
		glfwSetWindowContentScaleCallback((GLFWwindow*)window, [](GLFWwindow* window, float xscale, float yscale) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_content_scale_events.publish(glm::vec2(xscale, yscale));
			}
			});
		glfwSetWindowPosCallback((GLFWwindow*)window, [](GLFWwindow* window, int xpos, int ypos) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_position_events.publish(glm::ivec2(xpos, ypos));
			}
			});
		glfwSetWindowIconifyCallback((GLFWwindow*)window, [](GLFWwindow* window, int iconified) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_iconify_events.publish(iconified == GLFW_TRUE ? IconifyResult::ICONIFIED : IconifyResult::RESTORED);
			}
			});
		glfwSetWindowMaximizeCallback((GLFWwindow*)window, [](GLFWwindow* window, int maximized) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_maximize_events.publish(maximized == GLFW_TRUE ? MaximizeResult::MAXIMIZED : MaximizeResult::RESTORED);
			}
			});
		glfwSetWindowFocusCallback((GLFWwindow*)window, [](GLFWwindow* window, int focused) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_focus_events.publish(focused == GLFW_TRUE ? FocusResult::FOCUS_GAINED : FocusResult::FOCUS_LOST);
			}
			});
		glfwSetWindowRefreshCallback((GLFWwindow*)window, [](GLFWwindow* window) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_refresh_events.publish();
			}
			});
		
		glfwSetKeyCallback((GLFWwindow*)window, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				Key key_enum = Key(key);
				PressAction press_action =
					action == GLFW_PRESS ? PressAction::PRESS :
					action == GLFW_RELEASE ? PressAction::RELEASE :
					action == GLFW_REPEAT ? PressAction::REPEAT : PressAction::PRESS;
				KeyMods mods_enum = KeyMods(mods);

				newsletters->on_key_events.publish(KeyPressResult{.key=key_enum, .scancode=scancode, .action=press_action, .mods=mods_enum});
			}
			});
		glfwSetCharCallback((GLFWwindow*)window, [](GLFWwindow* window, uint32_t codepoint) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_char_events.publish(codepoint);
			}
			});
		glfwSetCursorPosCallback((GLFWwindow*)window, [](GLFWwindow* window, double xpos, double ypos) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_cursor_position_events.publish(glm::dvec2(xpos, ypos));
			}
			});
		glfwSetCursorEnterCallback((GLFWwindow*)window, [](GLFWwindow* window, int32_t entered) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_cursor_enter_or_leave_events.publish(entered == GLFW_TRUE ? CursorEnterResult::CURSOR_ENTERED : CursorEnterResult::CURSOR_EXITED);
			}
			});
		glfwSetMouseButtonCallback((GLFWwindow*)window, [](GLFWwindow* window, int32_t button, int32_t action, int32_t mods) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				MouseButton button_enum = MouseButton(button);
				PressAction press_action =
					action == GLFW_PRESS ? PressAction::PRESS :
					action == GLFW_RELEASE ? PressAction::RELEASE :
					action == GLFW_REPEAT ? PressAction::REPEAT : PressAction::PRESS;
				KeyMods mods_enum = KeyMods(mods);
				newsletters->on_mouse_key_events.publish(MousePressResult{.button=button_enum, .action=press_action, .mods=mods_enum});
			}
			});
		glfwSetScrollCallback((GLFWwindow*)window, [](GLFWwindow* window, double xoffset, double yoffset) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_mouse_scroll_events.publish(glm::dvec2(xoffset, yoffset));
			}
			});
		glfwSetDropCallback((GLFWwindow*)window, [](GLFWwindow* window, int path_count, const char* paths[]) {
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

	if (window != nullptr) {
		glfwDestroyWindow((GLFWwindow*)window);
		window = nullptr;
	}
}

bool Window::should_close()
{
	return glfwWindowShouldClose((GLFWwindow*)window) == GLFW_TRUE;
}

void Window::set_should_close(bool value)
{
	glfwSetWindowShouldClose((GLFWwindow*)window, value ? GLFW_TRUE : GLFW_FALSE);
}

void Window::swap_buffers()
{
	glfwSwapBuffers((GLFWwindow*)window);
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
	glfwSwapBuffers((GLFWwindow*)window);

	return 0;
}

//Window::NewslettersBlock* Window::get_newsletters()
//{
//	return newsletters;
//}
