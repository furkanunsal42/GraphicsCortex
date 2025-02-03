#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "StandardFrame.h"
#include "Monitor.h"
#include "Library.h"

#include <iostream>
#include "glm.hpp"

Frame2::Frame2(const FrameDescription& description)
{
	_initialize(description);
}

Frame2::Frame2(glm::ivec2 window_resolution, const std::string& window_name, Monitor& full_screen_target_monitor, GraphicsAPI api, uint32_t context_version_major, uint32_t context_version_minor)
{
	FrameDescription desc;
	desc.w_resolution = window_resolution;
	desc.f_resolution = window_resolution;
	desc.w_name = window_name;
	desc.w_fullscreen_monitor_ptr = &full_screen_target_monitor;
	desc.ctx_api = api;
	desc.ctx_version_major = context_version_major;
	desc.ctx_version_minor = context_version_minor;

	_initialize(desc);
}

Frame2::Frame2(glm::ivec2 window_resolution, const std::string& window_name, const Frame2& shared_context, Monitor& full_screen_target_monitor, GraphicsAPI api, uint32_t context_version_major, uint32_t context_version_minor)
{
	FrameDescription desc;
	desc.w_resolution = window_resolution;
	desc.f_resolution = window_resolution;
	desc.w_name = window_name;
	desc.w_fullscreen_monitor_ptr = &full_screen_target_monitor;
	desc.context_shared = shared_context.handle;
	desc.ctx_api = api;
	desc.ctx_version_major = context_version_major;
	desc.ctx_version_minor = context_version_minor;

	_initialize(desc);
}

Frame2::Frame2(glm::ivec2 window_resolution, const std::string& window_name, GraphicsAPI api, uint32_t context_version_major, uint32_t context_version_minor)
{
	FrameDescription desc;
	desc.w_resolution = window_resolution;
	desc.f_resolution = window_resolution;
	desc.w_name = window_name;
	desc.ctx_api = api;
	desc.ctx_version_major = context_version_major;
	desc.ctx_version_minor = context_version_minor;

	_initialize(desc);
}

Frame2::Frame2(glm::ivec2 window_resolution, const std::string& window_name, const Frame2& shared_context, GraphicsAPI api, uint32_t context_version_major, uint32_t context_version_minor)
{
	FrameDescription desc;
	desc.w_resolution = window_resolution;
	desc.f_resolution = window_resolution;
	desc.w_name = window_name;
	desc.context_shared = shared_context.handle;
	desc.ctx_api = api;
	desc.ctx_version_major = context_version_major;
	desc.ctx_version_minor = context_version_minor;

	_initialize(desc);
}

std::shared_ptr<Frame2> Frame2::create_from_current()
{
	GLFWwindow* context = glfwGetCurrentContext();
	auto frame = std::shared_ptr<Frame2>(new Frame2((void*)context));
	return frame;
}

Frame2::~Frame2()
{
	release();
}

void Frame2::release()
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


bool Frame2::should_close()
{
	return glfwWindowShouldClose((GLFWwindow*)handle) == GLFW_TRUE;
}

void Frame2::set_should_close(bool value)
{
	glfwSetWindowShouldClose((GLFWwindow*)handle, value ? GLFW_TRUE : GLFW_FALSE);
}

void Frame2::swap_buffers()
{
	glfwSwapBuffers((GLFWwindow*)handle);
}

void Frame2::poll_events()
{
	glfwPollEvents();
}

void Frame2::wait_events()
{
	glfwWaitEvents();
}

void Frame2::wait_events_timeout(double timeout_seconds)
{
	glfwWaitEventsTimeout(timeout_seconds);
}

void Frame2::post_empty_event() {
	glfwPostEmptyEvent();
}

double Frame2::handle_events(bool print_performances) {

	glfwPollEvents();
	glfwSwapBuffers((GLFWwindow*)handle);

	return 0;
}

Frame2::Frame2(void* context)
{
	this->handle = context;
}

void Frame2::_initialize(const FrameDescription& description)
{
	OpenGLBackend::_init_glfw();

	// context
	switch (description.ctx_api) {
	case OpenGL: 	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API); break;
	case OpenGL_ES:	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API); break;
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
	glfwWindowHint(GLFW_RESIZABLE, description.w_resizable ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, description.w_visible ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED, description.w_decorated ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_AUTO_ICONIFY, description.w_auto_iconify_when_fullscreen ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING, description.w_always_on_top ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, description.w_transparent ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, description.w_scale_window_size ? GLFW_TRUE : GLFW_FALSE);
	//glfwWindowHint(GLFW_SCALE_FRAMEBUFFER,		description.w_scale_framebuffer_size ? GLFW_TRUE : GLFW_FALSE);
	if (description.w_have_initial_position) {
		//	glfwWindowHint(GLFW_POSITION_X,				description.w_initial_position.x);
		//	glfwWindowHint(GLFW_POSITION_Y,				description.w_initial_position.y);
	}
	glfwWindowHint(GLFW_MAXIMIZED, description.w_maximized_on_create ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_FOCUSED, description.w_focus_on_create ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_FOCUS_ON_SHOW, description.w_focus_on_show ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_REFRESH_RATE, description.w_fullscreen_refresh_rate);
	//glfwWindowHint(GLFW_MOUSE_PASSTHROUGH,		description.i_mouse_passthrough ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_CENTER_CURSOR, description.i_center_cursor_on_create ? GLFW_TRUE : GLFW_FALSE);

	glm::ivec2 clipped_resolution = description.w_resolution;
	clipped_resolution = glm::max(clipped_resolution, glm::ivec2(1, 1));
	if (glm::any(glm::lessThanEqual(description.w_resolution, glm::ivec2(0, 0))))
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	void* monitor_ptr = nullptr;
	if (description.w_fullscreen_monitor_ptr != nullptr)
		monitor_ptr = (GLFWmonitor*)(description.w_fullscreen_monitor_ptr->monitor_ptr);

	handle = glfwCreateWindow(clipped_resolution.x, clipped_resolution.y, description.w_name.c_str(), (GLFWmonitor*)monitor_ptr, nullptr);
	glfwMakeContextCurrent((GLFWwindow*)handle); // ?

	if (description.GraphicsContextDescription::n_create_newsletters) {
		newsletters = new NewslettersBlock();
		glfwSetWindowUserPointer((GLFWwindow*)handle, this);

		glfwSetWindowCloseCallback((GLFWwindow*)handle, [](GLFWwindow* window) {
			Frame2* context = (Frame2*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_should_close_events.publish();
			}
			});

		glfwSetFramebufferSizeCallback((GLFWwindow*)handle, [](GLFWwindow* window, int width, int height) {
			Frame2* context = (Frame2*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_framebuffer_resolution_events.publish(glm::ivec2(width, height));
			}
			});

		glfwSetWindowSizeCallback((GLFWwindow*)handle, [](GLFWwindow* window, int width, int height) {
			Frame2* context = (Frame2*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_resolution_events.publish(glm::ivec2(width, height));
			}
			});
		glfwSetWindowContentScaleCallback((GLFWwindow*)handle, [](GLFWwindow* window, float xscale, float yscale) {
			Frame2* context = (Frame2*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_content_scale_events.publish(glm::vec2(xscale, yscale));
			}
			});
		glfwSetWindowPosCallback((GLFWwindow*)handle, [](GLFWwindow* window, int xpos, int ypos) {
			Frame2* context = (Frame2*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_position_events.publish(glm::ivec2(xpos, ypos));
			}
			});
		glfwSetWindowIconifyCallback((GLFWwindow*)handle, [](GLFWwindow* window, int iconified) {
			Frame2* context = (Frame2*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_iconify_events.publish(iconified == GLFW_TRUE ? IconifyResult::ICONIFIED : IconifyResult::RESTORED);
			}
			});
		glfwSetWindowMaximizeCallback((GLFWwindow*)handle, [](GLFWwindow* window, int maximized) {
			Frame2* context = (Frame2*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_maximize_events.publish(maximized == GLFW_TRUE ? MaximizeResult::MAXIMIZED : MaximizeResult::RESTORED);
			}
			});
		glfwSetWindowFocusCallback((GLFWwindow*)handle, [](GLFWwindow* window, int focused) {
			Frame2* context = (Frame2*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_focus_events.publish(focused == GLFW_TRUE ? FocusResult::FOCUS_GAINED : FocusResult::FOCUS_LOST);
			}
			});
		glfwSetWindowRefreshCallback((GLFWwindow*)handle, [](GLFWwindow* window) {
			Frame2* context = (Frame2*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_window_refresh_events.publish();
			}
			});

		glfwSetKeyCallback((GLFWwindow*)handle, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
			Frame2* context = (Frame2*)glfwGetWindowUserPointer(window);
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
			Frame2* context = (Frame2*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_char_events.publish(codepoint);
			}
			});
		glfwSetCursorPosCallback((GLFWwindow*)handle, [](GLFWwindow* window, double xpos, double ypos) {
			Frame2* context = (Frame2*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_cursor_position_events.publish(glm::dvec2(xpos, ypos));
			}
			});
		glfwSetCursorEnterCallback((GLFWwindow*)handle, [](GLFWwindow* window, int32_t entered) {
			Frame2* context = (Frame2*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_cursor_enter_or_leave_events.publish(entered == GLFW_TRUE ? CursorEnterResult::CURSOR_ENTERED : CursorEnterResult::CURSOR_EXITED);
			}
			});
		glfwSetMouseButtonCallback((GLFWwindow*)handle, [](GLFWwindow* window, int32_t button, int32_t action, int32_t mods) {
			Frame2* context = (Frame2*)glfwGetWindowUserPointer(window);
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
			Frame2* context = (Frame2*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_mouse_scroll_events.publish(glm::dvec2(xoffset, yoffset));
			}
			});
		glfwSetDropCallback((GLFWwindow*)handle, [](GLFWwindow* window, int path_count, const char* paths[]) {
			Frame2* context = (Frame2*)glfwGetWindowUserPointer(window);
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
