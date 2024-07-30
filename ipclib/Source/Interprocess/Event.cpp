#include "Event.h"

#if defined(WIN32) || defined(_WINDOWS)
#define windows
#endif

#if defined(__linux__)
#define linux 
#endif

#include <iostream>
#include "IPCAssert.h"

Event::Event(reset_type type)
{
	bool result;
	result = _open(type);
	
	if (!result) {
		std::cout << "[OS Error] Event failed to create" << std::endl;
		ASSERT(false);
	}
}

Event::Event(Event::handle_t handle)
{
	_handle = handle;
	
	_is_event_open = handle != nullptr;
	set_ownership(true /*false*/);	// handles are always owned by default!
	type = auto_reset; // TODO: we need to learn it from WIN32!!!
}

Event::Event(size_t handle)
	: Event(reinterpret_cast<Event::handle_t>(handle)) { }

Event::~Event()
{
	bool result;
	result = close();

	if (!result) {
		std::cout << "[OS Error] Event failed to destroy" << std::endl;
		ASSERT(false);
	}

	clear_listeners();
}

void Event::set_ownership(bool is_event_handle_owned)
{
	if (_is_event_open) _is_event_owned = is_event_handle_owned;
}

bool Event::close()
{
	if (!_is_event_open) return true;
	if (!_is_event_owned) return true;

	bool result = false;

#ifdef windows

	result = CloseHandle(_handle);

#endif
#ifdef linux

	result = false;
	std::cout << "[OS Error] Events in linux based systems are not supported" << std::endl;
	ASSERT(false);

#endif

	if (result) _is_event_open = false;
	if (result) _is_event_owned = false;
	return result;
}

bool Event::set()
{
	if (!_is_event_open) {
		std::cout << "[OS Error] Event::set() is called but Event is closed" << std::endl;
		ASSERT(false);
	}

	bool result = false;
#ifdef windows
	
	result = SetEvent(_handle);
	
#endif
#ifdef linux

	result = false;
	std::cout << "[OS Error] Events in linux based systems are not supported" << std::endl;
	ASSERT(false);

#endif

	return result;
}

bool Event::reset()
{
	if (!_is_event_open) {
		std::cout << "[OS Error] Event::set() is called but Event is closed" << std::endl;
		ASSERT(false);
	}

	bool result = false;
#ifdef windows

	result = ResetEvent(_handle);

#endif
#ifdef linux

	result = false;
	std::cout << "[OS Error] Events in linux based systems are not supported" << std::endl;
	ASSERT(false);

#endif

	return result;
}

Event::wait_op_return Event::wait()
{
	if (!_is_event_open) {
		std::cout << "[OS Error] Event::set() is called but Event is closed" << std::endl;
		ASSERT(false);
	}

	wait_op_return result;

#ifdef windows

	DWORD flag = WaitForSingleObject(_handle, INFINITE);
	result = 
		flag == WAIT_ABANDONED ? ABANDONED :
		flag == WAIT_OBJECT_0 ? SIGNALED :
		flag == WAIT_TIMEOUT ? TIMEOUT :
		flag == WAIT_FAILED ? FAILED : FAILED;


#endif
#ifdef linux

	result = false;
	std::cout << "[OS Error] Events in linux based systems are not supported" << std::endl;
	ASSERT(false);

#endif

	return result;
}

Event::wait_op_return Event::wait_timeout(int milliseconds)
{
	if (!_is_event_open) {
		std::cout << "[OS Error] Event::set() is called but Event is closed" << std::endl;
		ASSERT(false);
	}

	wait_op_return result;

#ifdef windows

	DWORD flag = WaitForSingleObject(_handle, milliseconds);
	result =
		flag == WAIT_ABANDONED ? ABANDONED :
		flag == WAIT_OBJECT_0 ? SIGNALED :
		flag == WAIT_TIMEOUT ? TIMEOUT :
		flag == WAIT_FAILED ? FAILED : FAILED;


#endif
#ifdef linux

	result = false;
	std::cout << "[OS Error] Events in linux based systems are not supported" << std::endl;
	ASSERT(false);

#endif

	return result;
}

void Event::add_listener(std::function<void()> lambda)
{
	_functions.push_back([this, lambda]() {
		if (this->type == auto_reset) {
			while (!this->_should_terminate_listeners) {
				Event::wait_op_return result = this->wait_timeout(_listener_check_period_ms);
				if (result == Event::SIGNALED)
					lambda();
				else if (result != Event::TIMEOUT)
					return;
			}
		}

		if (this->type == manual) {
			while (!this->_should_terminate_listeners) {
				Event::wait_op_return result = this->wait_timeout(_listener_check_period_ms);
				if (result == Event::SIGNALED) {
					lambda();
					return;
				}
				else if (result != Event::TIMEOUT) {
					return;
				}
			}
		}
		});

	_threads.push_back(std::thread(_functions.back()));
}

void Event::clear_listeners()
{
	_should_terminate_listeners = true;
	for (std::thread& thread : _threads)
		thread.join();

	_threads.clear();
	_functions.clear();
}

const Event::handle_t Event::get_handle()
{
	return _handle;
}

const Event::handle_t Event::get_handle_duplicate(process_handle_t target_process)
{
	Event::handle_t duplicate;
	bool result;

#ifdef windows

	result = DuplicateHandle(GetCurrentProcess(), _handle, target_process, &duplicate, 0, _is_inheritable, DUPLICATE_SAME_ACCESS);

#endif
#ifdef linux

	result = false;
	std::cout << "[OS Error] Events in linux based systems are not supported" << std::endl;
	ASSERT(false);

#endif

	return duplicate;
}

const size_t Event::get_handle_int()
{

	return reinterpret_cast<std::size_t>(_handle);
}

const size_t Event::get_handle_duplicate_int(process_handle_t target_process)
{
	return reinterpret_cast<std::size_t>(get_handle_duplicate(target_process));
}

bool Event::_open(reset_type type)
{
	if (_is_event_open) {
		std::cout << "[OS Error] Event::_open() is called but Event is already open" << std::endl;
		ASSERT(false);
	}

	bool result = false;

#ifdef windows

	bool is_manual_reset = type == manual ? true : false;

	SECURITY_ATTRIBUTES security = {
		sizeof(security), nullptr, true
	};
	
	_handle = CreateEvent(&security, is_manual_reset, false, nullptr);
	result = _handle != nullptr;

#endif
#ifdef linux

	result = false;
	std::cout << "[OS Error] Events in linux based systems are not supported" << std::endl;
	ASSERT(false);

#endif

	_is_event_open = result;
	set_ownership(result);
	this->type = type;
	return result;
}

std::ostream& operator<<(std::ostream& o, Event& event)
{
	o << "e:" << event.get_handle_int();
	return o;
}

std::wostream& operator<<(std::wostream& o, Event& event)
{
	o << L"e:" << event.get_handle_int();
	return o;
}

