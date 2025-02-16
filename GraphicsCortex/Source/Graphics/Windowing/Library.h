#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "Window.h"
#include "Monitor.h"
#include <chrono>

struct OpenGLBackend{
public:

	//static void initiailze();
	//static void release();

	static bool _is_glfw_initialized;
	static bool _is_glew_initialized;
	static std::chrono::time_point<std::chrono::system_clock> _opengl_initialization_time;

	static void _init_glfw();
	static void _init_glew();

	static void _release_glfw();
	static void _release_glew();
	
};

