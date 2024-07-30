#pragma once

#if defined(WIN32) || defined(_WINDOWS)
#define NOMINMAX
#define windows
#endif

#if defined(__linux__)
#define linux 
#endif

#ifdef windows
#include <Windows.h>
typedef HANDLE process_handle_t;
#endif

#ifdef linux
#include <unistd.h>
typedef uint32_t process_handle_t;
#endif

process_handle_t get_current_process_handle();
size_t get_current_process_handle_int();
process_handle_t get_current_process_handle_duplicate(process_handle_t target_process);
process_handle_t get_current_process_handle_duplicate(size_t target_process);
size_t get_current_process_handle_duplicate_int(process_handle_t target_process);
size_t get_current_process_handle_duplicate_int(size_t target_process);
