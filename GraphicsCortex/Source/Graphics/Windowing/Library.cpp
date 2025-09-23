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