#include <GL\glew.h>
#include <string>
#include <iostream>
#include <fstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) clear_errors();\
	x;\
	ASSERT(log_errors(#x, __LINE__, __FILE__));

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
		std::cout << "[OpenGL ERROR] on " << function << " line: " << line << " file: " << file << "\nError:" << error_name << '\n';
		error_safe = false;
	}
	return error_safe;
}

void clear_errors() {
	while (unsigned int error = glGetError()) {
		std::cout << "there was a previous error: " << error << '\n';
	}
}

