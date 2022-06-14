#pragma once
#include <string>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) clear_errors();\
	x;\
	ASSERT(log_errors(#x, __LINE__, __FILE__));

bool log_errors(const char* function, int line, const char* file);

void clear_errors();
