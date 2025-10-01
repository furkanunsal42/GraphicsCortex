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
    OpenGLBackend::_init_glew();
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
	window_name = description.w_name;

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
				Key key_enum = Window::GLFW_to_KEY(key);
				if (key_enum == Key(-1))
					return;

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

		newsletters->on_key_events.subscribe([&](const Window::KeyPressResult& result) {
			if (result.action == PressAction::PRESS) {
				newsletters->key_press_table[(size_t)result.key] = true;
				newsletters->is_key_press_table_dirty = true;
			}
			else if (result.action == PressAction::RELEASE) {
				newsletters->key_release_table[(size_t)result.key] = true;
				newsletters->is_key_release_table_dirty = true;
			}
			});
		
		newsletters->on_mouse_key_events.subscribe([&](const Window::MousePressResult& result) {
			if (result.action == PressAction::PRESS) {
				newsletters->mouse_press_table[(size_t)result.button] = true;
				newsletters->is_mouse_press_table_dirty = true;
			}
			else if (result.action == PressAction::RELEASE) {
				newsletters->mouse_release_table[(size_t)result.button] = true;
				newsletters->is_mouse_release_table_dirty = true;
			}
			});


	}

	// input handling
	glfwSetInputMode((GLFWwindow*)handle, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

	OpenGLBackend::_init_glew();

	if (description.ctx_enable_debug_callback_print) {
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(error_callback, 0);
	}
}

Window::Window(Window&& other) :
	handle(std::move(other.handle)),
	newsletters(std::move(other.newsletters)),
	window_name(std::move(other.window_name)),
	last_handle_events_time(std::move(other.last_handle_events_time)),
	last_perforamnce_print_time(std::move(other.last_perforamnce_print_time)),
	frame_count_since_performance_print(std::move(other.frame_count_since_performance_print))
{
	if (glfwGetWindowUserPointer((GLFWwindow*)handle) == &other)
		glfwSetWindowUserPointer((GLFWwindow*)handle, this);

	if (active_window == &other) {
		active_window = this;
	}

	other.handle = nullptr;
	other.newsletters = nullptr;
}

Window& Window::operator=(Window&& other)
{
	release();

	if (this == &other)
		return *this;

	handle								= std::move(other.handle);
	newsletters							= std::move(other.newsletters);
	window_name							= std::move(other.window_name);
	last_handle_events_time				= std::move(other.last_handle_events_time);
	last_perforamnce_print_time			= std::move(other.last_perforamnce_print_time);
	frame_count_since_performance_print = std::move(other.frame_count_since_performance_print);

	if (glfwGetWindowUserPointer((GLFWwindow*)handle) == &other)
		glfwSetWindowUserPointer((GLFWwindow*)handle, this);

	if (active_window == &other)
		active_window = this;

	other.handle = nullptr;
	other.newsletters = nullptr;
	
	return *this;
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
	if (active_window == this)
		active_window = nullptr;

	if (handle != nullptr) {
		glfwDestroyWindow((GLFWwindow*)handle);
		handle = nullptr;
	}
}

void Window::context_make_current()
{
	glfwMakeContextCurrent((GLFWwindow*)handle);

	active_global_resources = &(context_to_global_resources[handle]);
	active_window = this;
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

	if (newsletters != nullptr) {
		newsletters->clear_tables();
	}

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

	return get_and_reset_deltatime();
}

void* Window::get_handle() {
	return handle;
}

double Window::get_and_reset_deltatime()
{
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

std::string Window::get_window_name()
{
	//const char* title = glfwGetWindowTitle((GLFWwindow*)handle);
	//std::string title_str(title);
	//return title_str;

	return window_name;
}

void Window::set_window_name(const std::string window_name)
{
	glfwSetWindowTitle((GLFWwindow*)handle, window_name.c_str());
	this->window_name = window_name;
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
	int32_t glfw_key = Key_to_GLFW(key);
	if (glfw_key == -1) {
		std::cout << "[IO Error] get_key() is called with invalid Key" << std::endl;
		ASSERT(false);
	}

	int32_t action = glfwGetKey((GLFWwindow*)handle, glfw_key);

	return	action == GLFW_PRESS ? Window::PressAction::PRESS :
		action == GLFW_RELEASE ? Window::PressAction::RELEASE :
		action == GLFW_REPEAT ? Window::PressAction::REPEAT : Window::PressAction::PRESS;

}

int32_t Window::get_key_scancode(Key key)
{
	int32_t glfw_key = Key_to_GLFW(key);
	if (glfw_key == -1) {
		std::cout << "[IO Error] get_key_scancode() is called with invalid Key" << std::endl;
		ASSERT(false);
	}

	return glfwGetKeyScancode(glfw_key);
}

std::u8string Window::get_key_name(Key key)
{
	int32_t glfw_key = Key_to_GLFW(key);
	if (glfw_key == -1) {
		std::cout << "[IO Error] get_key_name() is called with invalid Key" << std::endl;
		ASSERT(false);
	}

	return std::u8string((const char8_t*)glfwGetKeyName(glfw_key, get_key_scancode(key)));
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

bool Window::get_key_press_inpulse(Key key)
{
	if (newsletters == nullptr) {
		std::cout << "[IO Error] Window::get_key_press_impulse() is called but impulse queries are disabled when Window isn't initialized with Window::newsletters" << std::endl;
		ASSERT(false);
	}

	return newsletters->key_press_table[(size_t)key];
}

bool Window::get_key_release_inpulse(Key key)
{
	if (newsletters == nullptr) {
		std::cout << "[IO Error] Window::get_key_release_inpulse() is called but impulse queries are disabled when Window isn't initialized with Window::newsletters" << std::endl;
		ASSERT(false);
	}
	return newsletters->key_release_table[(size_t)key];
}

bool Window::get_mouse_press_inpulse(MouseButton button)
{
	if (newsletters == nullptr) {
		std::cout << "[IO Error] Window::get_mouse_press_inpulse() is called but impulse queries are disabled when Window isn't initialized with Window::newsletters" << std::endl;
		ASSERT(false);
	}
	return newsletters->mouse_press_table[(size_t)button];
}

bool Window::get_mouse_release_inpulse(MouseButton button)
{
	if (newsletters == nullptr) {
		std::cout << "[IO Error] Window::get_mouse_release_inpulse() is called but impulse queries are disabled when Window isn't initialized with Window::newsletters" << std::endl;
		ASSERT(false);
	}
	return newsletters->mouse_release_table[(size_t)button];
}

Window::NewslettersBlock::NewslettersBlock()
{
	clear_tables();
}

void Window::NewslettersBlock::clear_tables()
{
	if (is_key_press_table_dirty)		std::memset(key_press_table.data(),		false, key_press_table.size()		* sizeof(bool));
	if (is_key_release_table_dirty)		std::memset(key_release_table.data(),	false, key_release_table.size()		* sizeof(bool));
	if (is_mouse_press_table_dirty)		std::memset(mouse_press_table.data(),	false, mouse_press_table.size()		* sizeof(bool));
	if (is_mouse_release_table_dirty)	std::memset(mouse_release_table.data(), false, mouse_release_table.size()	* sizeof(bool));

	is_key_press_table_dirty = false;
	is_key_release_table_dirty = false;
	is_mouse_press_table_dirty = false;
	is_mouse_release_table_dirty = false;
}

int32_t Window::Key_to_GLFW(Key key) {
	switch (key) {
	case Key::SPACE			:	return GLFW_KEY_SPACE;
	case Key::APOSTROPHE	:	return GLFW_KEY_APOSTROPHE;
	case Key::COMMA			:	return GLFW_KEY_COMMA;
	case Key::MINUS			:	return GLFW_KEY_MINUS;
	case Key::PERIOD		:	return GLFW_KEY_PERIOD;
	case Key::SLASH			:	return GLFW_KEY_SLASH;
	case Key::NUM_0			:	return GLFW_KEY_0;
	case Key::NUM_1			:	return GLFW_KEY_1;
	case Key::NUM_2			:	return GLFW_KEY_2;
	case Key::NUM_3			:	return GLFW_KEY_3;
	case Key::NUM_4			:	return GLFW_KEY_4;
	case Key::NUM_5			:	return GLFW_KEY_5;
	case Key::NUM_6			:	return GLFW_KEY_6;
	case Key::NUM_7			:	return GLFW_KEY_7;
	case Key::NUM_8			:	return GLFW_KEY_8;
	case Key::NUM_9			:	return GLFW_KEY_9;
	case Key::SEMICOLON		:	return GLFW_KEY_SEMICOLON;
	case Key::EQUAL			:	return GLFW_KEY_EQUAL;
	case Key::A				:	return GLFW_KEY_A;
	case Key::B				:	return GLFW_KEY_B;
	case Key::C				:	return GLFW_KEY_C;
	case Key::D				:	return GLFW_KEY_D;
	case Key::E				:	return GLFW_KEY_E;
	case Key::F				:	return GLFW_KEY_F;
	case Key::G				:	return GLFW_KEY_G;
	case Key::H				:	return GLFW_KEY_H;
	case Key::I				:	return GLFW_KEY_I;
	case Key::J				:	return GLFW_KEY_J;
	case Key::K				:	return GLFW_KEY_K;
	case Key::L				:	return GLFW_KEY_L;
	case Key::M				:	return GLFW_KEY_M;
	case Key::N				:	return GLFW_KEY_N;
	case Key::O				:	return GLFW_KEY_O;
	case Key::P				:	return GLFW_KEY_P;
	case Key::Q				:	return GLFW_KEY_Q;
	case Key::R				:	return GLFW_KEY_R;
	case Key::S				:	return GLFW_KEY_S;
	case Key::T				:	return GLFW_KEY_T;
	case Key::U				:	return GLFW_KEY_U;
	case Key::V				:	return GLFW_KEY_V;
	case Key::W				:	return GLFW_KEY_W;
	case Key::X				:	return GLFW_KEY_X;
	case Key::Y				:	return GLFW_KEY_Y;
	case Key::Z				:	return GLFW_KEY_Z;
	case Key::LEFT_BRACKET	:	return GLFW_KEY_LEFT_BRACKET;
	case Key::BACKSLASH		:	return GLFW_KEY_BACKSLASH;
	case Key::RIGHT_BRACKET	:	return GLFW_KEY_RIGHT_BRACKET;
	case Key::GRAVE_ACCENT	:	return GLFW_KEY_GRAVE_ACCENT;
	case Key::WORLD_1		:	return GLFW_KEY_WORLD_1;
	case Key::WORLD_2		:	return GLFW_KEY_WORLD_2;
	case Key::ESCAPE		:	return GLFW_KEY_ESCAPE;
	case Key::ENTER			:	return GLFW_KEY_ENTER;
	case Key::TAB			:	return GLFW_KEY_TAB;
	case Key::BACKSPACE		:	return GLFW_KEY_BACKSPACE;
	case Key::INSERT		:	return GLFW_KEY_INSERT;
	case Key::DELETE		:	return GLFW_KEY_DELETE;
	case Key::RIGHT			:	return GLFW_KEY_RIGHT;
	case Key::LEFT			:	return GLFW_KEY_LEFT;
	case Key::DOWN			:	return GLFW_KEY_DOWN;
	case Key::UP			:	return GLFW_KEY_UP;
	case Key::PAGE_UP		:	return GLFW_KEY_PAGE_UP;
	case Key::PAGE_DOWN		:	return GLFW_KEY_PAGE_DOWN;
	case Key::HOME			:	return GLFW_KEY_HOME;
	case Key::END			:	return GLFW_KEY_END;
	case Key::CAPS_LOCK		:	return GLFW_KEY_CAPS_LOCK;
	case Key::SCROLL_LOCK	:	return GLFW_KEY_SCROLL_LOCK;
	case Key::NUM_LOCK		:	return GLFW_KEY_NUM_LOCK;
	case Key::PRINT_SCREEN	:	return GLFW_KEY_PRINT_SCREEN;
	case Key::PAUSE			:	return GLFW_KEY_PAUSE;
	case Key::F1			:	return GLFW_KEY_F1;
	case Key::F2			:	return GLFW_KEY_F2;
	case Key::F3			:	return GLFW_KEY_F3;
	case Key::F4			:	return GLFW_KEY_F4;
	case Key::F5			:	return GLFW_KEY_F5;
	case Key::F6			:	return GLFW_KEY_F6;
	case Key::F7			:	return GLFW_KEY_F7;
	case Key::F8			:	return GLFW_KEY_F8;
	case Key::F9			:	return GLFW_KEY_F9;
	case Key::F10			:	return GLFW_KEY_F10;
	case Key::F11			:	return GLFW_KEY_F11;
	case Key::F12			:	return GLFW_KEY_F12;
	case Key::F13			:	return GLFW_KEY_F13;
	case Key::F14			:	return GLFW_KEY_F14;
	case Key::F15			:	return GLFW_KEY_F15;
	case Key::F16			:	return GLFW_KEY_F16;
	case Key::F17			:	return GLFW_KEY_F17;
	case Key::F18			:	return GLFW_KEY_F18;
	case Key::F19			:	return GLFW_KEY_F19;
	case Key::F20			:	return GLFW_KEY_F20;
	case Key::F21			:	return GLFW_KEY_F21;
	case Key::F22			:	return GLFW_KEY_F22;
	case Key::F23			:	return GLFW_KEY_F23;
	case Key::F24			:	return GLFW_KEY_F24;
	case Key::F25			:	return GLFW_KEY_F25;
	case Key::KP_0			:	return GLFW_KEY_KP_0;
	case Key::KP_1			:	return GLFW_KEY_KP_1;
	case Key::KP_2			:	return GLFW_KEY_KP_2;
	case Key::KP_3			:	return GLFW_KEY_KP_3;
	case Key::KP_4			:	return GLFW_KEY_KP_4;
	case Key::KP_5			:	return GLFW_KEY_KP_5;
	case Key::KP_6			:	return GLFW_KEY_KP_6;
	case Key::KP_7			:	return GLFW_KEY_KP_7;
	case Key::KP_8			:	return GLFW_KEY_KP_8;
	case Key::KP_9			:	return GLFW_KEY_KP_9;
	case Key::KP_DECIMAL	:	return GLFW_KEY_KP_DECIMAL;
	case Key::KP_DIVIDE		:	return GLFW_KEY_KP_DIVIDE;
	case Key::KP_MULTIPLY	:	return GLFW_KEY_KP_MULTIPLY;
	case Key::KP_SUBTRACT	:	return GLFW_KEY_KP_SUBTRACT;
	case Key::KP_ADD		:	return GLFW_KEY_KP_ADD;
	case Key::KP_ENTER		:	return GLFW_KEY_KP_ENTER;
	case Key::KP_EQUAL		:	return GLFW_KEY_KP_EQUAL;
	case Key::LEFT_SHIFT	:	return GLFW_KEY_LEFT_SHIFT;
	case Key::LEFT_CONTROL	:	return GLFW_KEY_LEFT_CONTROL;
	case Key::LEFT_ALT		:	return GLFW_KEY_LEFT_ALT;
	case Key::LEFT_SUPER	:	return GLFW_KEY_LEFT_SUPER;
	case Key::RIGHT_SHIFT	:	return GLFW_KEY_RIGHT_SHIFT;
	case Key::RIGHT_CONTROL	:	return GLFW_KEY_RIGHT_CONTROL;
	case Key::RIGHT_ALT		:	return GLFW_KEY_RIGHT_ALT;
	case Key::RIGHT_SUPER	:	return GLFW_KEY_RIGHT_SUPER;
	case Key::MENU			:	return GLFW_KEY_MENU;
	}

	std::cout << "[Window Error] Window::Key_to_GLFW() is called but given key is invalid" << std::endl;
	ASSERT(false);
	return -1;
}

Window::Key Window::GLFW_to_KEY(int32_t glfw_key)
{
	switch (glfw_key) {
	case GLFW_KEY_SPACE:			return Key::SPACE;
	case GLFW_KEY_APOSTROPHE:		return Key::APOSTROPHE;
	case GLFW_KEY_COMMA:			return Key::COMMA;
	case GLFW_KEY_MINUS:			return Key::MINUS;
	case GLFW_KEY_PERIOD:			return Key::PERIOD;
	case GLFW_KEY_SLASH:			return Key::SLASH;
	case GLFW_KEY_0:				return Key::NUM_0;
	case GLFW_KEY_1:				return Key::NUM_1;
	case GLFW_KEY_2:				return Key::NUM_2;
	case GLFW_KEY_3:				return Key::NUM_3;
	case GLFW_KEY_4:				return Key::NUM_4;
	case GLFW_KEY_5:				return Key::NUM_5;
	case GLFW_KEY_6:				return Key::NUM_6;
	case GLFW_KEY_7:				return Key::NUM_7;
	case GLFW_KEY_8:				return Key::NUM_8;
	case GLFW_KEY_9:				return Key::NUM_9;
	case GLFW_KEY_SEMICOLON:		return Key::SEMICOLON;
	case GLFW_KEY_EQUAL:			return Key::EQUAL;
	case GLFW_KEY_A:				return Key::A;
	case GLFW_KEY_B:				return Key::B;
	case GLFW_KEY_C:				return Key::C;
	case GLFW_KEY_D:				return Key::D;
	case GLFW_KEY_E:				return Key::E;
	case GLFW_KEY_F:				return Key::F;
	case GLFW_KEY_G:				return Key::G;
	case GLFW_KEY_H:				return Key::H;
	case GLFW_KEY_I:				return Key::I;
	case GLFW_KEY_J:				return Key::J;
	case GLFW_KEY_K:				return Key::K;
	case GLFW_KEY_L:				return Key::L;
	case GLFW_KEY_M:				return Key::M;
	case GLFW_KEY_N:				return Key::N;
	case GLFW_KEY_O:				return Key::O;
	case GLFW_KEY_P:				return Key::P;
	case GLFW_KEY_Q:				return Key::Q;
	case GLFW_KEY_R:				return Key::R;
	case GLFW_KEY_S:				return Key::S;
	case GLFW_KEY_T:				return Key::T;
	case GLFW_KEY_U:				return Key::U;
	case GLFW_KEY_V:				return Key::V;
	case GLFW_KEY_W:				return Key::W;
	case GLFW_KEY_X:				return Key::X;
	case GLFW_KEY_Y:				return Key::Y;
	case GLFW_KEY_Z:				return Key::Z;
	case GLFW_KEY_LEFT_BRACKET:		return Key::LEFT_BRACKET;
	case GLFW_KEY_BACKSLASH:		return Key::BACKSLASH;
	case GLFW_KEY_RIGHT_BRACKET:	return Key::RIGHT_BRACKET;
	case GLFW_KEY_GRAVE_ACCENT:		return Key::GRAVE_ACCENT;
	case GLFW_KEY_WORLD_1:			return Key::WORLD_1;
	case GLFW_KEY_WORLD_2:			return Key::WORLD_2;
	case GLFW_KEY_ESCAPE:			return Key::ESCAPE;
	case GLFW_KEY_ENTER:			return Key::ENTER;
	case GLFW_KEY_TAB:				return Key::TAB;
	case GLFW_KEY_BACKSPACE:		return Key::BACKSPACE;
	case GLFW_KEY_INSERT:			return Key::INSERT;
	case GLFW_KEY_DELETE:			return Key::DELETE;
	case GLFW_KEY_RIGHT:			return Key::RIGHT;
	case GLFW_KEY_LEFT:				return Key::LEFT;
	case GLFW_KEY_DOWN:				return Key::DOWN;
	case GLFW_KEY_UP:				return Key::UP;
	case GLFW_KEY_PAGE_UP:			return Key::PAGE_UP;
	case GLFW_KEY_PAGE_DOWN:		return Key::PAGE_DOWN;
	case GLFW_KEY_HOME:				return Key::HOME;
	case GLFW_KEY_END:				return Key::END;
	case GLFW_KEY_CAPS_LOCK:		return Key::CAPS_LOCK;
	case GLFW_KEY_SCROLL_LOCK:		return Key::SCROLL_LOCK;
	case GLFW_KEY_NUM_LOCK:			return Key::NUM_LOCK;
	case GLFW_KEY_PRINT_SCREEN:		return Key::PRINT_SCREEN;
	case GLFW_KEY_PAUSE:			return Key::PAUSE;
	case GLFW_KEY_F1:				return Key::F1;
	case GLFW_KEY_F2:				return Key::F2;
	case GLFW_KEY_F3:				return Key::F3;
	case GLFW_KEY_F4:				return Key::F4;
	case GLFW_KEY_F5:				return Key::F5;
	case GLFW_KEY_F6:				return Key::F6;
	case GLFW_KEY_F7:				return Key::F7;
	case GLFW_KEY_F8:				return Key::F8;
	case GLFW_KEY_F9:				return Key::F9;
	case GLFW_KEY_F10:				return Key::F10;
	case GLFW_KEY_F11:				return Key::F11;
	case GLFW_KEY_F12:				return Key::F12;
	case GLFW_KEY_F13:				return Key::F13;
	case GLFW_KEY_F14:				return Key::F14;
	case GLFW_KEY_F15:				return Key::F15;
	case GLFW_KEY_F16:				return Key::F16;
	case GLFW_KEY_F17:				return Key::F17;
	case GLFW_KEY_F18:				return Key::F18;
	case GLFW_KEY_F19:				return Key::F19;
	case GLFW_KEY_F20:				return Key::F20;
	case GLFW_KEY_F21:				return Key::F21;
	case GLFW_KEY_F22:				return Key::F22;
	case GLFW_KEY_F23:				return Key::F23;
	case GLFW_KEY_F24:				return Key::F24;
	case GLFW_KEY_F25:				return Key::F25;
	case GLFW_KEY_KP_0:				return Key::KP_0;
	case GLFW_KEY_KP_1:				return Key::KP_1;
	case GLFW_KEY_KP_2:				return Key::KP_2;
	case GLFW_KEY_KP_3:				return Key::KP_3;
	case GLFW_KEY_KP_4:				return Key::KP_4;
	case GLFW_KEY_KP_5:				return Key::KP_5;
	case GLFW_KEY_KP_6:				return Key::KP_6;
	case GLFW_KEY_KP_7:				return Key::KP_7;
	case GLFW_KEY_KP_8:				return Key::KP_8;
	case GLFW_KEY_KP_9:				return Key::KP_9;
	case GLFW_KEY_KP_DECIMAL:		return Key::KP_DECIMAL;
	case GLFW_KEY_KP_DIVIDE:		return Key::KP_DIVIDE;
	case GLFW_KEY_KP_MULTIPLY:		return Key::KP_MULTIPLY;
	case GLFW_KEY_KP_SUBTRACT:		return Key::KP_SUBTRACT;
	case GLFW_KEY_KP_ADD:			return Key::KP_ADD;
	case GLFW_KEY_KP_ENTER:			return Key::KP_ENTER;
	case GLFW_KEY_KP_EQUAL:			return Key::KP_EQUAL;
	case GLFW_KEY_LEFT_SHIFT:		return Key::LEFT_SHIFT;
	case GLFW_KEY_LEFT_CONTROL:		return Key::LEFT_CONTROL;
	case GLFW_KEY_LEFT_ALT:			return Key::LEFT_ALT;
	case GLFW_KEY_LEFT_SUPER:		return Key::LEFT_SUPER;
	case GLFW_KEY_RIGHT_SHIFT:		return Key::RIGHT_SHIFT;
	case GLFW_KEY_RIGHT_CONTROL:	return Key::RIGHT_CONTROL;
	case GLFW_KEY_RIGHT_ALT:		return Key::RIGHT_ALT;
	case GLFW_KEY_RIGHT_SUPER:		return Key::RIGHT_SUPER;
	case GLFW_KEY_MENU:				return Key::MENU;
	}

	//std::cout << "[Window Error] Window::GLFW_to_Key() is called but given glfw_key is invalid" << std::endl;
	//ASSERT(false);
	return Key(-1);
}
