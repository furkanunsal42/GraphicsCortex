#include "ProcessHandleInterface.h"

process_handle_t ipc_api process_get_current_process_handle()
{
    return get_current_process_handle();
}

size_t ipc_api process_get_current_process_handle_int()
{
    return get_current_process_handle_int();
}

process_handle_t ipc_api process_get_current_process_handle_duplicate_h(process_handle_t target_process)
{
    return get_current_process_handle_duplicate(target_process);
}

process_handle_t ipc_api process_get_current_process_handle_duplicate_s(size_t target_process)
{
    return get_current_process_handle_duplicate(target_process);
}

size_t ipc_api process_get_current_process_handle_duplicate_int_h(process_handle_t target_process)
{
    return get_current_process_handle_duplicate_int(target_process);
}

size_t ipc_api process_get_current_process_handle_duplicate_int_s(size_t target_process)
{
    return get_current_process_handle_duplicate_int(target_process);
}
