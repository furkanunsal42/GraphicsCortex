#include "GraphicsContext.h"
#include "GLFW/glfw3.h"
#include "Library.h"

#include <iostream>

GraphicsContext::GraphicsContext(GraphicsAPI api, uint32_t context_version_major, uint32_t context_version_minor)
{
	ctx_version_major = context_version_major;
	ctx_version_minor = context_version_minor;
	ctx_api = api;
}

GraphicsContext::GraphicsContext(const GraphicsContext& shared_context, GraphicsAPI api, uint32_t context_version_major, uint32_t context_version_minor) :
	GraphicsContext(api, context_version_major, context_version_minor)
{
	if (shared_context.ctx_api != api) {
		std::cout << "[GraphicsCortex Error] Context Sharing cannot be used between different Graphics APIs" << std::endl;
		__debugbreak();
	}
	context_shared = shared_context.context_shared;
}

std::shared_ptr<GraphicsContext> GraphicsContext::create_from_current()
{
	GLFWwindow* context = glfwGetCurrentContext();
	auto graphics_context = std::make_shared<GraphicsContext>(OpenGL, 4, 6);

	graphics_context->_is_context_initialized = true;
	graphics_context->context = context;
	graphics_context->context_shared = nullptr;
	
	graphics_context->ctx_version_major = graphics_context->get_context_version_major();
	graphics_context->ctx_version_minor = graphics_context->get_context_version_minor();
	graphics_context->ctx_api = OpenGL;

	return graphics_context;
};

GraphicsContext::~GraphicsContext()
{
	if (_is_context_initialized)
		release();
}

void GraphicsContext::_initialize()
{
	if (_is_context_initialized) return;

	OpenGLBackend::_init_glfw();

	// context
	switch (ctx_api) {
	case OpenGL: 	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API); break;
	case OpenGL_ES:	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API); break;
	case None:	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); break;
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


	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	context = glfwCreateWindow(1, 1, "", nullptr, (GLFWwindow*)context_shared);
	glfwMakeContextCurrent((GLFWwindow*)context);

	OpenGLBackend::_init_glew();

	_is_context_initialized = true;
}

void GraphicsContext::release()
{
	if (!_is_context_initialized) return;
	
	glfwDestroyWindow((GLFWwindow*)context);

	_is_context_initialized = false;
}

void GraphicsContext::context_make_current()
{
	_initialize();

	glfwMakeContextCurrent((GLFWwindow*)context);
}

void GraphicsContext::set_context_version(uint32_t major, uint32_t minor)
{
	if (_is_context_initialized) {
		std::cout << "[OpenGL Error] GrpahicsContext tried to change context version after initialization" << std::endl;
		__debugbreak();
	}

	ctx_version_major = major;
	ctx_version_minor = minor;
}

uint32_t GraphicsContext::get_context_version_major()
{
	return ctx_version_major;
}

uint32_t GraphicsContext::get_context_version_minor()
{
	return ctx_version_minor;
}

void GraphicsContext::set_context_api(GraphicsAPI api)
{
	if (_is_context_initialized) {
		std::cout << "[OpenGL Error] GrpahicsContext tried to change context api after initialization" << std::endl;
		__debugbreak();
	}

	ctx_api = api;
}

GraphicsContext::GraphicsAPI GraphicsContext::get_context_api()
{
	return ctx_api;
}

void GraphicsContext::set_context_forward_compatibility(bool value)
{
	if (_is_context_initialized) {
		std::cout << "[OpenGL Error] GrpahicsContext tried to change context forward compatibility after initialization" << std::endl;
		__debugbreak();
	}

	ctx_forward_compatibility = value;
}

bool GraphicsContext::get_context_forawrd_compatibility()
{
	return ctx_forward_compatibility;
}

void GraphicsContext::set_context_debug_mode(bool value)
{
	if (_is_context_initialized) {
		std::cout << "[OpenGL Error] GrpahicsContext tried to change context debug mode after initialization" << std::endl;
		__debugbreak();
	}

	ctx_debug_mode = value;
}

bool GraphicsContext::get_context_debug_mode()
{
	return ctx_debug_mode;
}


void GraphicsContext::set_context_profile(OpenGLProfile profile)
{
	if (_is_context_initialized) {
		std::cout << "[OpenGL Error] GrpahicsContext tried to change context profile after initialization" << std::endl;
		__debugbreak();
	}
	
	ctx_profile = profile;
}

GraphicsContext::OpenGLProfile GraphicsContext::get_context_profile()
{
	return ctx_profile;
}

double GraphicsContext::handle_events(bool print_performances) {
	_initialize();

	glfwPollEvents();
	glfwSwapBuffers((GLFWwindow*)context);

	return 0;
}


/*
//			---------------			default framebuffer			--------------- 
glm::ivec2 GraphicsContext::get_framebuffer_resolution()
{
	glm::ivec2 framebuffer_resolution;
	glfwGetFramebufferSize((GLFWwindow*)context, &framebuffer_resolution.x, &framebuffer_resolution.y);
	return framebuffer_resolution;
}

glm::ivec2 GraphicsContext::get_resolution()
{
	glm::ivec2 context_size;
	glfwGetWindowSize((GLFWwindow*)context, &context_size.x, &context_size.y);
	return context_size;
}

void GraphicsContext::set_resolution(glm::ivec2 resolution)
{
	glfwSetWindowSize((GLFWwindow*)context, resolution.x, resolution.y);
}

void GraphicsContext::set_framebuffer_resolution(glm::ivec2 resolution)
{
	glm::ivec2 framebuffer_resolution;
	glfwSetSize((GLFWwindow*)context, &framebuffer_resolution.x, &framebuffer_resolution.y);
	return framebuffer_resolution;
}

double GraphicsContext::handle_events(bool print_performances) {
	_initialize();

	glfwPollEvents();
	glfwSwapBuffers((GLFWwindow*)context);
	return 0;
}
*/