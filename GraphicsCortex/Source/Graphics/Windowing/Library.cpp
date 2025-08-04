#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Library.h"
#include "Debuger.h"

#include <iostream>

bool OpenGLBackend::_is_glfw_initialized = false;
bool OpenGLBackend::_is_glew_initialized = false;
std::chrono::time_point<std::chrono::system_clock> OpenGLBackend::_opengl_initialization_time;

void OpenGLBackend::_init_glfw()
{
	if (_is_glfw_initialized) return;

	if (!glfwInit()) {
		std::cout << "[OpenGL Error] OpenGL failed to initialize " << std::endl;
		__debugbreak();
	}

	_is_glfw_initialized = true;
}

void OpenGLBackend::_release_glfw()
{
	if (!_is_glfw_initialized) return;

	glfwTerminate();

	_is_glfw_initialized = false;
}


void OpenGLBackend::_init_glew()
{
	if (_is_glew_initialized) return;

	glewInit();
	OpenGLBackend::_opengl_initialization_time = std::chrono::system_clock::now();

	_is_glew_initialized = true;
}

void OpenGLBackend::_release_glew()
{
	if (!_is_glew_initialized) return;

	_is_glew_initialized = false;
}

/*

std::unordered_map<
	OpenGLBackend::OpenGLResourcePlain, size_t,
	OpenGLBackend::HashOpenGLResourcePlain, OpenGLBackend::EqualOpenGLResourcePlain>
	OpenGLBackend::opengl_resource_reference_counter;

OpenGLBackend::OpenGLResourcePlain::OpenGLResourcePlain(const OpenGLResource& resource){
	id = resource.id;
	type = resource.type;
}

OpenGLBackend::OpenGLResourcePlain::OpenGLResourcePlain(ResourceType type, uint32_t id){
	this->id = id;
	this->type = type;
}

inline OpenGLBackend::OpenGLResource OpenGLBackend::create_object(ResourceType type)
{
	OpenGLResource new_resource;
	new_resource.type = type;
	switch (type) {
	case Texture1D:
		GLCall(glCreateTextures(GL_TEXTURE_1D, 1, &new_resource.id));
		break;
	case Texture2D: 
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &new_resource.id));
		break;
	case Renderbuffer: 
		GLCall(glCreateTextures(GL_RENDERBUFFER, 1, &new_resource.id));
		break;
	case Texture2DArray: 
		GLCall(glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &new_resource.id));
		break;
	case Texture3D: 
		GLCall(glCreateTextures(GL_TEXTURE_3D, 1, &new_resource.id));
		break;
	case Framebuffer: 
		GLCall(glCreateFramebuffers(1, &new_resource.id));
		break;
	case Buffer:
		GLCall(glCreateBuffers(1, &new_resource.id));
		break;
	case VertexArray: 
		GLCall(glCreateVertexArrays(1, &new_resource.id));
		break;
	}

	if (new_resource.id == 0) {
		ASSERT(false);
	}

	if (opengl_resource_reference_counter.find(new_resource) != opengl_resource_reference_counter.end()) {
		ASSERT(false);
	}

	opengl_resource_reference_counter[new_resource] = 1;

	return new_resource;
}

void OpenGLBackend::force_release_object(OpenGLResource& resource)
{
	if (resource.id == 0)
		return;

	switch (resource.type) {
	case Texture1D:
	case Texture2D:
	case Renderbuffer:
	case Texture2DArray:
	case Texture3D:
		GLCall(glDeleteTextures(1, &resource.id));
		break;
	case Framebuffer:
		GLCall(glDeleteFramebuffers(1, &resource.id));
		break;
	case Buffer:
		GLCall(glDeleteBuffers(1, &resource.id));
		break;
	case VertexArray:
		GLCall(glDeleteVertexArrays(1, &resource.id));
		break;
	}

	opengl_resource_reference_counter.erase(resource);

	resource.id = 0;
}

void OpenGLBackend::reference_object(const OpenGLResource& resource)
{
	if (resource.id == 0)
		return;

	if (opengl_resource_reference_counter.find(resource) == opengl_resource_reference_counter.end()) {
		ASSERT(false);
	}

	opengl_resource_reference_counter[resource]++;
}

void OpenGLBackend::dereference_object(OpenGLResource& resource)
{
	if (resource.id == 0)
		return;

	if (opengl_resource_reference_counter.find(resource) == opengl_resource_reference_counter.end()) {
		ASSERT(false);
	}

	opengl_resource_reference_counter[resource]--;

	if (opengl_resource_reference_counter[resource] == 0) {
		OpenGLBackend::force_release_object(resource);
	}
}

OpenGLBackend::OpenGLResource::OpenGLResource(ResourceType type)
{
	OpenGLResource new_resource = OpenGLBackend::create_object(type);
	this->type = new_resource.type;
	this->id = new_resource.id;
	new_resource.id = 0;
}

OpenGLBackend::OpenGLResource::~OpenGLResource()
{
	OpenGLBackend::dereference_object(*this);
}

OpenGLBackend::OpenGLResource::OpenGLResource(const OpenGLResource& other)
{
	id = other.id;
	type = other.type;

	OpenGLBackend::reference_object(*this);
}

OpenGLBackend::OpenGLResource::OpenGLResource(OpenGLResource&& other)
{
	id = other.id;
	type = other.type;

	OpenGLBackend::dereference_object(other);
	other.id = 0;
}

OpenGLBackend::OpenGLResource OpenGLBackend::OpenGLResource::operator=(const OpenGLResource& other)
{
	OpenGLBackend::dereference_object(*this);

	id = other.id;
	type = other.type;

	OpenGLBackend::reference_object(other);

	return *this;
}

OpenGLBackend::OpenGLResource OpenGLBackend::OpenGLResource::operator=(OpenGLResource&& other)
{
	OpenGLBackend::dereference_object(*this);

	id = other.id;
	type = other.type;

	other.id = 0;

	return *this;
}

size_t OpenGLBackend::OpenGLResource::get_reference_count()
{
	auto iterator = OpenGLBackend::opengl_resource_reference_counter.find(*this);

	if (iterator == OpenGLBackend::opengl_resource_reference_counter.end())
		return 0;

	return iterator->second;
}

void OpenGLBackend::OpenGLResource::force_release()
{
	OpenGLBackend::force_release_object(*this);
}

*/