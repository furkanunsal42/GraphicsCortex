#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "StandardFrame.h"
#include "Monitor.h"
#include "Library.h"

#include <iostream>

/*
Frame2::Frame2(uint32_t window_width, uint32_t window_height, const std::string& window_name, const Monitor& full_screen_target_monitor, GraphicsAPI api, uint32_t context_version_major, uint32_t context_version_minor) :
	Frame2(window_width, window_height, window_name, api, context_version_major, context_version_minor)
{
	w_fullscreen_monitor_ptr = full_screen_target_monitor.monitor_ptr;
}

Frame2::Frame2(uint32_t window_width, uint32_t window_height, const std::string& window_name, const Frame2& shared_context, const Monitor& full_screen_target_monitor, GraphicsAPI api, uint32_t context_version_major, uint32_t context_version_minor) :
	Frame2(window_width, window_height, window_name, shared_context, api, context_version_major, context_version_minor)
{
	w_fullscreen_monitor_ptr = full_screen_target_monitor.monitor_ptr;
}

Frame2::Frame2(uint32_t window_width, uint32_t window_height, const std::string& window_name, GraphicsAPI api, uint32_t context_version_major, uint32_t context_version_minor)
{
	ctx_version_major = context_version_major;
	ctx_version_minor = context_version_minor;
	ctx_api = api;

	f_width = window_width;
	f_height = window_height;

	w_width = window_width;
	w_height = window_height;
	w_name = window_name;
}

Frame2::Frame2(uint32_t window_width, uint32_t window_height, const std::string& window_name, const Frame2& shared_context, GraphicsAPI api, uint32_t context_version_major, uint32_t context_version_minor) :
	Frame2(window_width, window_height, window_name, api, context_version_major, context_version_minor)
{
	if (shared_context.ctx_api != api) {
		std::cout << "[GraphicsCortex Error] Context Sharing cannot be used between different Graphics APIs" << std::endl;
		__debugbreak();
	}
	glfw_window_shared = shared_context.glfw_window;
}

Frame2::~Frame2()
{
	if (_is_frame_initialized)
		release();
}

void Frame2::_initialize()
{
	if (_is_frame_initialized) return;

	GraphicsCortex::_init_glfw();
	
	// context
	switch (ctx_api) {
	case OpenGL		: 	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API); break;
	case OpenGL_ES	:	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API); break;
	case None		:	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); break;
	}
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ctx_version_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ctx_version_minor);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, ctx_forward_compatibility ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, ctx_debug_mode ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, ctx_profile == OpenGLProfile::Core ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
	//glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, );
	glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_ANY_RELEASE_BEHAVIOR);
	glfwWindowHint(GLFW_CONTEXT_NO_ERROR, GLFW_FALSE);



	// windowing
	glfwWindowHint(GLFW_VISIBLE, (w_width == 0 || w_height == 0) ? GLFW_FALSE : GLFW_TRUE);

	glfw_window = glfwCreateWindow(w_width, w_height, w_name.c_str(), (GLFWmonitor*)w_fullscreen_monitor_ptr, (GLFWwindow*)glfw_window_shared);
	glfwMakeContextCurrent((GLFWwindow*)glfw_window);

	GraphicsCortex::_init_glew();


	_is_frame_initialized = true;
}

void Frame2::release()
{
	std::cout << "frame2 released" << std::endl;
}

//			---------------			graphics context			--------------- 



//			---------------			default framebuffer			--------------- 



//			---------------			windowing					--------------- 

bool Frame2::should_close() {
	_initialize();
	
	return glfwWindowShouldClose((GLFWwindow*)glfw_window);
}

double Frame2::handle_window_events(bool print_performances) {
	_initialize();

	glfwPollEvents();
	glfwSwapBuffers((GLFWwindow*)glfw_window);
	return 0;
}
*/

//			---------------			input handling				--------------- 

