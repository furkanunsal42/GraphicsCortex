#pragma once
#include "Config.h"
#include <string>
#include <chrono>

#define ASSERT(x) if (!(x)) __debugbreak();

#ifdef ENABLE_OPENGL_DEBUGING
#define GLCall(x) clear_errors(#x, __LINE__, __FILE__);\
	x;\
	ASSERT(log_errors(#x, __LINE__, __FILE__));

#define PrintTime(x)\
	 { \
		auto begin = std::chrono::high_resolution_clock::now(); \
		x; \
		auto stop = std::chrono::high_resolution_clock::now(); \
		std::cout << _STRINGIZE(x) << " | " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - begin).count() << " ms" << std::endl; \
	 }

#else
#define GLCall(x) x;
#define PrintTime(x) x;
#endif

bool log_errors(const char* function, int line, const char* file);

void clear_errors(const char* function, int line, const char* file);
