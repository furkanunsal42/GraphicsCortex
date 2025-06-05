#include "GL/glew.h"
#include "Window.h"
#include "GLFW/glfw3.h"
#include "Library.h"
#include <filesystem>
#include <iostream>
#include "glm.hpp"
#include "Image.h"
#include "Monitor.h"

#include "WindowBoundGlobalResources.h"

Window::Window(const WindowDescription& description)
{
	_initialize(description);
}

Window::Window(glm::ivec2 window_resolution, const std::string& window_name, Monitor& full_screen_target_monitor, GraphicsAPI api, uint32_t context_version_major, uint32_t context_version_minor)
{
	WindowDescription description;
	description.w_resolution = window_resolution;
	description.f_resolution = window_resolution;
	description.w_name = window_name;
	description.w_fullscreen_monitor_ptr = &full_screen_target_monitor;
	description.ctx_api = api;
	description.ctx_version_major = context_version_major;
	description.ctx_version_minor = context_version_minor;

	_initialize(description);
}

Window::Window(glm::ivec2 window_resolution, const std::string& window_name, Window& shared_context, GraphicsAPI api, uint32_t context_version_major, uint32_t context_version_minor)
{
	WindowDescription description;
	description.w_resolution = window_resolution;
	description.f_resolution = window_resolution;
	description.w_name = window_name;
	description.context_shared = shared_context.handle;
	description.ctx_api = api;
	description.ctx_version_major = context_version_major;
	description.ctx_version_minor = context_version_minor;

	_initialize(description);
}

Window::Window(glm::ivec2 window_resolution, const std::string& window_name, Monitor& full_screen_target_monitor, Window& shared_context, GraphicsAPI api, uint32_t context_version_major, uint32_t context_version_minor)
{
	WindowDescription description;
	description.w_resolution = window_resolution;
	description.f_resolution = window_resolution;
	description.w_name = window_name;
	description.w_fullscreen_monitor_ptr = &full_screen_target_monitor;
	description.context_shared = shared_context.handle;
	description.ctx_api = api;
	description.ctx_version_major = context_version_major;
	description.ctx_version_minor = context_version_minor;

	_initialize(description);
}

Window::Window(glm::ivec2 window_resolution, const std::string& window_name, GraphicsAPI api, uint32_t context_version_major, uint32_t context_version_minor)
{
	WindowDescription description;
	description.w_resolution = window_resolution;
	description.f_resolution = window_resolution;
	description.w_name = window_name;
	description.ctx_api = api;
	description.ctx_version_major = context_version_major;
	description.ctx_version_minor = context_version_minor;

	_initialize(description);
}

Window::Window(void* context)
{
	this->handle = context;
}

std::shared_ptr<Window> Window::create_from_current()
{
	GLFWwindow* current_handle = glfwGetCurrentContext();
	auto window = std::shared_ptr<Window>(new Window((void*)current_handle));
	window->context_make_current(); // to set global resources
	return window;
};

namespace {
	
	void error_callback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam
	) {
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}
}

void Window::_initialize(const WindowDescription& description)
{
	OpenGLBackend::_init_glfw();

	// context
	switch (description.ctx_api) {
	case OpenGL: 	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API); break;
	case OpenGL_ES:	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API); break;
	case Vulkan:	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); break;
	case None:		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); break;
	}

	// context
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, description.ctx_version_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, description.ctx_version_minor);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, description.ctx_forward_compatibility ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, description.ctx_debug_mode ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, description.ctx_profile == OpenGLProfile::Core ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
	//glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, );
	glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_ANY_RELEASE_BEHAVIOR);
	glfwWindowHint(GLFW_CONTEXT_NO_ERROR, GLFW_FALSE);

	// framebuffer
	glfwWindowHint(GLFW_RED_BITS, description.f_color_bits.r);
	glfwWindowHint(GLFW_GREEN_BITS, description.f_color_bits.g);
	glfwWindowHint(GLFW_BLUE_BITS, description.f_color_bits.b);
	glfwWindowHint(GLFW_ALPHA_BITS, description.f_color_bits.a);
	glfwWindowHint(GLFW_DEPTH_BITS, description.f_depth_stencil_bits.x);
	glfwWindowHint(GLFW_STENCIL_BITS, description.f_depth_stencil_bits.y);
	glfwWindowHint(GLFW_SAMPLES, description.f_multisample_count);
	glfwWindowHint(GLFW_SRGB_CAPABLE, description.f_srgb_enabled ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, description.f_double_buffered ? GLFW_TRUE : GLFW_FALSE);

	// window
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
	if (glm::any(glm::lessThanEqual(description.w_resolution, glm::ivec2(0, 0))))
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	void* monitor_ptr = nullptr;
	if (description.w_fullscreen_monitor_ptr != nullptr)
		monitor_ptr = (GLFWmonitor*)(description.w_fullscreen_monitor_ptr->monitor_ptr);

	handle = glfwCreateWindow(clipped_resolution.x, clipped_resolution.y, description.w_name.c_str(), (GLFWmonitor*)monitor_ptr, (GLFWwindow*)description.context_shared);

	// global resoureces
	// if (description.context_shared != nullptr) {
	// even if context is shared a unique GlobalResources() is created for each new context
	// }
	
	context_make_current();
	glfwSwapInterval(description.f_swap_interval);

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

		glfwSetFramebufferSizeCallback((GLFWwindow*)handle, [](GLFWwindow* window, int width, int height) {
			Window* context = (Window*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_framebuffer_resolution_events.publish(glm::ivec2(width, height));
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
				std::vector<std::filesystem::path> paths_v;
				for (int i = 0; i < path_count; i++)
					paths_v.push_back(std::filesystem::path(paths[i]));
				newsletters->on_filepath_drop_events.publish(paths_v);
			}
			});
	}

	// input handling
	glfwSetInputMode((GLFWwindow*)handle, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

	OpenGLBackend::_init_glew();

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(error_callback, 0);
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

	GlobalResources* owned_global_resources = &(context_to_global_resources[handle]);
	context_to_global_resources.erase(handle);
	if (active_global_resources == owned_global_resources)
		active_global_resources = nullptr;

	if (handle != nullptr) {
		glfwDestroyWindow((GLFWwindow*)handle);
		handle = nullptr;
	}
}

void Window::context_make_current()
{
	glfwMakeContextCurrent((GLFWwindow*)handle);

	active_global_resources = &(context_to_global_resources[handle]);
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

	if (print_performances) {
		if (last_perforamnce_print_time == invalid_time) {
			last_perforamnce_print_time = std::chrono::system_clock::now();
		}

		double time_since_print = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - last_perforamnce_print_time).count();
		frame_count_since_performance_print++;

		if (time_since_print > 1000) {
			std::cout << "[Info] FPS : " << frame_count_since_performance_print << " frametime : " << time_since_print / frame_count_since_performance_print << std::endl;
			last_perforamnce_print_time = std::chrono::system_clock::now();
			frame_count_since_performance_print = 0;
		}
	}

	double deltatime_ms;
	if (last_handle_events_time == invalid_time) {
		last_handle_events_time = std::chrono::system_clock::now();
		deltatime_ms = 0;
	}
	else {
		std::chrono::system_clock::duration deltatime = std::chrono::system_clock::now() - last_handle_events_time;
		last_handle_events_time = std::chrono::system_clock::now();
		deltatime_ms = std::chrono::duration_cast<std::chrono::microseconds>(deltatime).count() / 1000.0f;
	}
	return deltatime_ms;

}

void* Window::get_handle() {
	return handle;
}

// context

uint32_t Window::get_context_version_major()
{
	GLint version_major = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &version_major);
	return version_major;
}


uint32_t Window::get_context_version_minor()
{
	GLint version_minor = 0;
	glGetIntegerv(GL_MINOR_VERSION, &version_minor);
	return version_minor;
}


Window::GraphicsAPI Window::get_context_api()
{
	const unsigned char* version_str = glGetString(GL_VERSION);
	std::cout << version_str << std::endl;
	return OpenGL;
}

bool Window::get_context_forward_compatibility()
{
	GLint flags = 0;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	return flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT;
}

//bool Window::get_context_debug_mode()
//{
//	__debugbreak();
//	return true;
//}

Window::OpenGLProfile Window::get_context_profile()
{
	GLint profile = 0;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

	if (profile & GL_CONTEXT_CORE_PROFILE_BIT) {
		return Window::OpenGLProfile::Core;
	}

	if (profile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT) {
		return Window::OpenGLProfile::Compatibility;
	}

	__debugbreak();
	return Window::OpenGLProfile::Compatibility;
}

// framebuffer

glm::ivec2 Window::get_framebuffer_resolution() {
	glm::ivec2 size;
	glfwGetFramebufferSize((GLFWwindow*)handle, &size.x, &size.y);
	return size;
}

glm::ivec4 Window::get_framebuffer_color_bits()
{
	glm::ivec4 color_bits;
	glGetNamedFramebufferAttachmentParameteriv(0, GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE, &color_bits.r);
	glGetNamedFramebufferAttachmentParameteriv(0, GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE, &color_bits.g);
	glGetNamedFramebufferAttachmentParameteriv(0, GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE, &color_bits.b);
	glGetNamedFramebufferAttachmentParameteriv(0, GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &color_bits.a);
	return color_bits;
}

glm::ivec2 Window::get_framebuffer_depth_stencil_bits()
{
	glm::ivec4 depth_stencil_bits;
	glGetNamedFramebufferAttachmentParameteriv(0, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &depth_stencil_bits.x);
	glGetNamedFramebufferAttachmentParameteriv(0, GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &depth_stencil_bits.y);
	return depth_stencil_bits;
}

int32_t Window::get_framebuffer_multisample_count()
{
	int32_t samples;
	glGetIntegerv(GL_SAMPLES, &samples);
	return samples;
}

void Window::set_framebuffer_swap_interval(int32_t value)
{
	glfwSwapInterval(value);
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

void Window::set_cursor_position(glm::dvec2 position)
{
	glfwSetCursorPos((GLFWwindow*)handle, position.x, position.y);
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
