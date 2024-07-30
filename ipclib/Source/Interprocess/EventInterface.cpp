#include "EventInterface.h"

intptr_t ipc_api event_create()
{
	return (intptr_t) new Event();
}

intptr_t ipc_api event_create_r(Event::reset_type type)
{
	return (intptr_t) new Event(type);
}

intptr_t ipc_api event_create_h(Event::handle_t handle)
{
	return (intptr_t) new Event(handle);
}

intptr_t ipc_api event_create_s(size_t handle)
{
	return (intptr_t) new Event(handle);
}

void ipc_api event_destroy(void* event)
{
	delete event;
}

void ipc_api event_set_ownership(void* event, bool is_event_handle_owned)
{
	((Event*)event)->set_ownership(is_event_handle_owned);
}

bool ipc_api event_close(void* event)
{
	return ((Event*)event)->close();
}

bool ipc_api event_set(void* event)
{
	return ((Event*)event)->set();
}

bool ipc_api event_reset(void* event)
{
	return ((Event*)event)->reset();
}

Event::wait_op_return ipc_api event_wait(void* event)
{
	return ((Event*)event)->wait();
}

Event::wait_op_return ipc_api event_wait_timeout(void* event, int milliseconds)
{
	return ((Event*)event)->wait_timeout(milliseconds);
}

void ipc_api event_clear_listeners(void* event)
{
	((Event*)event)->clear_listeners();
}

Event::handle_t ipc_api event_get_handle(void* event)
{
	return ((Event*)event)->get_handle();
}

Event::handle_t ipc_api event_get_handle_duplicate(void* event, process_handle_t target_process)
{
	return ((Event*)event)->get_handle_duplicate(target_process);
}

size_t ipc_api event_get_handle_int(void* event)
{
	return ((Event*)event)->get_handle_int();
}

size_t ipc_api event_get_handle_duplicate_int(void* event, process_handle_t target_process)
{
	return ((Event*)event)->get_handle_duplicate_int(target_process);
}
