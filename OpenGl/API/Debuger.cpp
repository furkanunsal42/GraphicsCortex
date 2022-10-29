#include <GL\glew.h>

#include "Debuger.h"

#include <iostream>
#include <fstream>
#include <iostream>

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

void clear_errors(const char* function, int line, const char* file) {
	while (unsigned int error = glGetError()) {
		std::cout << "there was a previous error on: " << function << " line: " << line << " file : " << file << "Error : " << error << '\n';
	}
}

