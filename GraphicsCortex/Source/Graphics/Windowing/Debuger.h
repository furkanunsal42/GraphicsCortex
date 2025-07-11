#pragma once
#include "Config.h"
#include <string>
#include <chrono>

#define ASSERT(x) if (!(x)) __debugbreak();

#ifdef ENABLE_OPENGL_DEBUGING
#define GLCall(x)								\
	clear_errors(#x, __LINE__, __FILE__);		\
	x;											\
	ASSERT(log_errors(#x, __LINE__, __FILE__));	\

#else
#define GLCall(x) x;
#endif

bool log_errors(const char* function, int line, const char* file);

void clear_errors(const char* function, int line, const char* file);

void opengl_debug_callback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, char const* message, void const* user_param);

