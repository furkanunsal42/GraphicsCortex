#include <GL/glew.h>

#include "Debuger.h"
#include "Library.h"

#include <iostream>
#include <fstream>
#include <iostream>
#include <chrono>

bool log_errors(const char* function, int line, const char* file) {
	bool error_safe = true;
	while (unsigned int error = glGetError()) {
		std::string error_name = std::to_string(error);
		switch (error) {
		case 0x0500: error_name = "GL_INVALID_ENUM"; break;
		case 0x0501: error_name = "GL_INVALID_VALUE"; break;
		case 0x0502: error_name = "GL_INVALID_OPERATION"; break;
		case 0x0503: error_name = "GL_STACK_OVERFLOW"; break;
		case 0x0504: error_name = "GL_STACK_UNDERFLOW"; break;
		case 0x0505: error_name = "GL_OUT_OF_MEMORY"; break;
		}
		std::cout << "[OpenGL ERROR] on " << function << " line: " << line << " file: " << file << " Error:" << error_name << '\n';
		error_safe = false;
	}
	return error_safe;
}

void clear_errors(const char* function, int line, const char* file) {
	while (unsigned int error = glGetError()) {
		std::string error_name = std::to_string(error);
		switch (error) {
		case 0x0500: error_name = "GL_INVALID_ENUM"; break;
		case 0x0501: error_name = "GL_INVALID_VALUE"; break;
		case 0x0502: error_name = "GL_INVALID_OPERATION"; break;
		case 0x0503: error_name = "GL_STACK_OVERFLOW"; break;
		case 0x0504: error_name = "GL_STACK_UNDERFLOW"; break;
		case 0x0505: error_name = "GL_OUT_OF_MEMORY"; break;
		}
		std::cout << "there was a previous error on: " << function << " line: " << line << " file : " << file << " Error : " << error_name << '\n';
	}
}

void opengl_debug_callback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, char const* message, void const* user_param) {
	auto const src_str = [source]() {
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		}
		return ".";
	}();

	auto const type_str = [type]() {
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		}
		return ".";
	}();


	auto const severity_str = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		}
		return ".";
	}();

	long time_ms = (std::chrono::system_clock::now() - OpenGLBackend::_opengl_initialization_time).count() / 1000;
	std::cout << "[OpenGL Callack] t:" << time_ms << "ms " << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}

