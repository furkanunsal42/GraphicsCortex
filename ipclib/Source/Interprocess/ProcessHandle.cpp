#include "ProcessHandle.h"

#if defined(WIN32) || defined(_WINDOWS)
#define NOMINMAX
#define windows
#endif

#if defined(__linux__)
#define linux 
#endif

process_handle_t get_current_process_handle()
{
    process_handle_t handle = 0;

#ifdef windows

    handle = GetCurrentProcess();

#endif
#ifdef linux

    std::cout << "[OS Error] Process handles in linux based systems are not supported" << std::endl;
    ASSERT(false);

#endif

    return handle;
}

size_t get_current_process_handle_int()
{
    return reinterpret_cast<size_t>(get_current_process_handle());
}

process_handle_t get_current_process_handle_duplicate(process_handle_t target_process)
{
    process_handle_t duplicate = 0;

#ifdef windows

    process_handle_t handle = GetCurrentProcess();
    bool result = DuplicateHandle(GetCurrentProcess(), handle, target_process, &duplicate, 0, true, DUPLICATE_SAME_ACCESS);

#endif
#ifdef linux

    std::cout << "[OS Error] Process handles in linux based systems are not supported" << std::endl;
    ASSERT(false);

#endif

    return handle;
}

process_handle_t get_current_process_handle_duplicate(size_t target_process)
{
    return get_current_process_handle_duplicate(reinterpret_cast<process_handle_t>(target_process));
}

size_t get_current_process_handle_duplicate_int(process_handle_t target_process)
{
    return reinterpret_cast<size_t>(get_current_process_handle_duplicate(target_process));
}

size_t get_current_process_handle_duplicate_int(size_t target_process)
{
    return reinterpret_cast<size_t>(get_current_process_handle_duplicate(target_process));
}
