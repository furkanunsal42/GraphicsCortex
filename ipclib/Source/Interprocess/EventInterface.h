#pragma once
#include "Event.h"

#define ipc_api __declspec(dllexport)

extern "C" {

	intptr_t ipc_api event_create();
	intptr_t ipc_api event_create_r(Event::reset_type type);
	intptr_t ipc_api event_create_h(Event::handle_t handle);
	intptr_t ipc_api event_create_s(size_t handle);

	void ipc_api event_destroy(void* event);

	void ipc_api event_set_ownership(void* event, bool is_event_handle_owned);
	bool ipc_api event_close(void* event);

	bool ipc_api event_set(void* event);
	bool ipc_api event_reset(void* event);

	Event::wait_op_return ipc_api event_wait(void* event);
	Event::wait_op_return ipc_api event_wait_timeout(void* event, int milliseconds);

	//void event_add_listener(void* event, std::function<void()> lambda);
	void ipc_api event_clear_listeners(void* event);

	Event::handle_t ipc_api event_get_handle(void* event);
	Event::handle_t ipc_api event_get_handle_duplicate(void* event, process_handle_t target_process);

	size_t ipc_api event_get_handle_int(void* event);
	size_t ipc_api event_get_handle_duplicate_int(void* event, process_handle_t target_process);

	//std::ostream& operator<<(std::ostream& o, Event& event);
}