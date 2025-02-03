#include "GL/glew.h"
#include "GraphicsContext.h"
#include "GLFW/glfw3.h"
#include "Library.h"

#include <iostream>
#include "glm.hpp"

GraphicsContext::GraphicsContext(const GraphicsContextDescription& params)
{
	_initialize(params);
}

GraphicsContext::GraphicsContext(glm::ivec2 framebuffer_resolution, GraphicsAPI api, uint32_t context_version_major, uint32_t context_version_minor)
{
	GraphicsContextDescription params;
	params.f_resolution = framebuffer_resolution;
	params.ctx_api = api;
	params.ctx_version_major = context_version_major;
	params.ctx_version_minor = context_version_minor;

	_initialize(params);
}

GraphicsContext::GraphicsContext(glm::ivec2 framebuffer_resolution, GraphicsContext& shared_context, GraphicsAPI api, uint32_t context_version_major, uint32_t context_version_minor) :
	GraphicsContext(framebuffer_resolution, api, context_version_major, context_version_minor)
{
	GraphicsContextDescription params;
	params.f_resolution = framebuffer_resolution;
	params.ctx_api = api;
	params.ctx_version_major = context_version_major;
	params.ctx_version_minor = context_version_minor;
	params.context_shared = shared_context.handle;

	if (shared_context.get_context_api() != api) {
		std::cout << "[GraphicsCortex Error] Context Sharing cannot be used between different Graphics APIs" << std::endl;
		__debugbreak();
	}

	_initialize(params);
}

// private constructor for create_from_current(); context is created from an another application that uses this library as a dll.
GraphicsContext::GraphicsContext(void* context)
{
	this->handle = context;
}

std::shared_ptr<GraphicsContext> GraphicsContext::create_from_current()
{
	GLFWwindow* context = glfwGetCurrentContext();
	auto graphics_context = std::shared_ptr<GraphicsContext>(new GraphicsContext((void*)context));
	return graphics_context;
};

void GraphicsContext::_initialize(const GraphicsContextDescription& description)
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

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	glm::ivec2 clipped_resolution = description.f_resolution;
	clipped_resolution = glm::max(clipped_resolution, glm::ivec2(1, 1));

	handle = glfwCreateWindow(clipped_resolution.x, clipped_resolution.y, "", nullptr, (GLFWwindow*)description.context_shared);
	context_make_current();

	if (description.f_multisample_count > 0) {
		glEnable(GL_MULTISAMPLE);
	}

	if (description.n_create_newsletters) {
		newsletters = new NewslettersBlock();
		glfwSetWindowUserPointer((GLFWwindow*)handle, this);
		
		glfwSetFramebufferSizeCallback((GLFWwindow*)handle, [](GLFWwindow* window, int width, int height) {
			GraphicsContext* context = (GraphicsContext*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr){
				newsletters->on_framebuffer_resolution_events.publish(glm::ivec2(width, height));
			}
			});
		
		glfwSetWindowCloseCallback((GLFWwindow*)handle, [](GLFWwindow* window) {
			GraphicsContext* context = (GraphicsContext*)glfwGetWindowUserPointer(window);
			NewslettersBlock* newsletters = context->newsletters;
			if (newsletters != nullptr) {
				newsletters->on_should_close_events.publish();
			}
			});
	}

	OpenGLBackend::_init_glew();
}

GraphicsContext::~GraphicsContext()
{
	release();
}

void GraphicsContext::release()
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

bool GraphicsContext::should_close()
{
	return glfwWindowShouldClose((GLFWwindow*)handle) == GLFW_TRUE;
}

void GraphicsContext::set_should_close(bool value)
{
	glfwSetWindowShouldClose((GLFWwindow*)handle, value ? GLFW_TRUE : GLFW_FALSE);
}

void GraphicsContext::swap_buffers()
{
	glfwSwapBuffers((GLFWwindow*)handle);
}

void GraphicsContext::poll_events()
{
	glfwPollEvents();
}

void GraphicsContext::wait_events()
{
	glfwWaitEvents();
}

void GraphicsContext::wait_events_timeout(double timeout_seconds)
{
	glfwWaitEventsTimeout(timeout_seconds);
}

void GraphicsContext::post_empty_event() {
	glfwPostEmptyEvent();
}

double GraphicsContext::handle_events(bool print_performances) {

	glfwPollEvents();
	glfwSwapBuffers((GLFWwindow*)handle);

	return 0;
}

void GraphicsContext::context_make_current()
{
	glfwMakeContextCurrent((GLFWwindow*)handle);
}

uint32_t GraphicsContext::get_context_version_major()
{
	GLint version_major = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &version_major);
	return version_major;
}


uint32_t GraphicsContext::get_context_version_minor()
{
	GLint version_minor = 0;
	glGetIntegerv(GL_MINOR_VERSION, &version_minor);
	return version_minor;
}


GraphicsContext::GraphicsAPI GraphicsContext::get_context_api()
{
	const unsigned char* version_str = glGetString(GL_VERSION);
	std::cout << version_str << std::endl;
	return OpenGL;
}

bool GraphicsContext::get_context_forward_compatibility()
{
	GLint flags = 0;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	return flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT;
}

//bool GraphicsContext::get_context_debug_mode()
//{
//	__debugbreak();
//	return true;
//}

GraphicsContext::OpenGLProfile GraphicsContext::get_context_profile()
{
	GLint profile = 0;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

	if (profile & GL_CONTEXT_CORE_PROFILE_BIT) {
		return GraphicsContext::OpenGLProfile::Core;
	}

	if (profile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT) {
		return GraphicsContext::OpenGLProfile::Compatibility;
	}

	__debugbreak();
	return GraphicsContext::OpenGLProfile::Compatibility;
}

// framebuffer

glm::ivec2 GraphicsContext::get_framebuffer_resolution() {
	glm::ivec2 size;
	glfwGetFramebufferSize((GLFWwindow*)handle, &size.x, &size.y);
	return size;
}

glm::ivec4 GraphicsContext::get_framebuffer_color_bits()
{
	glm::ivec4 color_bits;
	glGetNamedFramebufferAttachmentParameteriv(0, GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE, &color_bits.r);
	glGetNamedFramebufferAttachmentParameteriv(0, GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE, &color_bits.g);
	glGetNamedFramebufferAttachmentParameteriv(0, GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE, &color_bits.b);
	glGetNamedFramebufferAttachmentParameteriv(0, GL_FRONT_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &color_bits.a);
	return color_bits;
}

glm::ivec2 GraphicsContext::get_framebuffer_depth_stencil_bits()
{
	glm::ivec4 depth_stencil_bits;
	glGetNamedFramebufferAttachmentParameteriv(0, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &depth_stencil_bits.x);
	glGetNamedFramebufferAttachmentParameteriv(0, GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &depth_stencil_bits.y);
	return depth_stencil_bits;
}

int32_t GraphicsContext::get_framebuffer_multisample_count()
{
	int32_t samples;
	glGetIntegerv(GL_SAMPLES, &samples);
	return samples;
}

void GraphicsContext::set_framebuffer_swap_interval(int32_t value)
{
	glfwSwapInterval(value);
}

//GraphicsContext::NewslettersBlock* GraphicsContext::get_newsletters() {
//	return newsletters;
//}