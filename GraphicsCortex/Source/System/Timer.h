#pragma once
#include <chrono>
#include <thread>
#include <atomic>

#include "Newsletter.h"

template<bool async, typename... T>
class Timer {
public:

	~Timer() { stop(); }

	bool is_async_timer();

	Newsletter<T...>& get_newsletter();
	
	void start_one_shot(std::chrono::milliseconds delay);
	void start_periodic(std::chrono::milliseconds period);
	void reset();
	void stop();
	bool is_running();

	void handle_events();

private:

	bool should_stop = false;
	std::atomic<size_t> publish_counter = 0;
	std::thread thread;
	Newsletter<T...> newsletter;
};

#include "TimerTemplated.h"