#pragma once

#if defined(WIN32) || defined(_WINDOWS)
#define windows
#endif
#if defined(__linux__)
#define linux 
#endif

#ifdef windows
#define NOMINMAX
#include <Windows.h>
#endif

#ifdef linux
#include <unistd.h>
#endif

#include "ProcessHandle.h"

#include <string>
#include <functional>
#include <thread>
#include <vector>

class Event {
public:

#ifdef windows
	typedef HANDLE handle_t;
#endif
#ifdef linux
	typedef int32_t handle_t;
#endif

	enum reset_type {
		manual = 0,
		auto_reset = 1,
	};

	enum wait_op_return {
		FAILED = 0,
		SIGNALED = 1,
		TIMEOUT = 2,
		ABANDONED = 3,
	};

	Event(Event& other) = delete;
	Event(reset_type type = auto_reset);
	Event(Event::handle_t handle);
	Event(size_t handle);

	~Event();
	void set_ownership(bool is_event_handle_owned);
	bool close();

	bool set();
	bool reset();

	wait_op_return wait();
	wait_op_return wait_timeout(int milliseconds);
	
	void add_listener(std::function<void()> lambda);
	void clear_listeners();

	const Event::handle_t get_handle();
	const Event::handle_t get_handle_duplicate(process_handle_t target_process);

	const size_t get_handle_int();
	const size_t get_handle_duplicate_int(process_handle_t target_process);

private:
	
	const bool _is_inheritable = true;
	
	bool _open(reset_type type);

	Event::handle_t _handle;
	reset_type type;
	bool _is_event_open = false;
	bool _is_event_owned = false;

	const uint32_t _listener_check_period_ms = 1;
	bool _should_terminate_listeners = false;
	std::vector<std::function<void()>> _functions;
	std::vector<std::thread> _threads;
};

std::ostream& operator<<(std::ostream& o, Event& event);
std::wostream& operator<<(std::wostream& o, Event& event);