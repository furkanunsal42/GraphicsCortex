#pragma once
#include "Timer.h"

#include <iostream>
#include "Debuger.h"

template<bool async, typename... T>
inline Newsletter<T...>& Timer<async, T...>::get_newsletter() {
	return newsletter;
}

template<bool async, typename... T>
inline bool Timer<async, T...>::is_async_timer() {
	return async;
}

template<bool async, typename... T>
inline void Timer<async, T...>::start_one_shot(std::chrono::milliseconds delay) {
	
	if (async) {
		thread = std::thread([=]() {
			std::this_thread::sleep_for(delay);
			newsletter.publish();
			});
	}
	else {
		thread = std::thread([=]() {
			std::this_thread::sleep_for(delay);
			publish_counter++;
			});
	}
}

template<bool async, typename... T>
inline void Timer<async, T...>::start_periodic(std::chrono::milliseconds period) {

	if (async) {
		thread = std::thread([=]() {
			while (!should_stop) {
				std::this_thread::sleep_for(period);
				newsletter.publish();
			}
			});
	}
	else {
		thread = std::thread([&]() {
			while (!should_stop) {
				std::this_thread::sleep_for(period);
				publish_counter++;
			}
			});
	}
}

template<bool async, typename... T>
inline void Timer<async, T...>::reset() {
	
}

template<bool async, typename... T>
inline void Timer<async, T...>::stop() {
	if (!is_running())
		return;

	should_stop = true;
	thread.join();
	thread = std::thread();
	should_stop = false;
}

template<bool async, typename... T>
inline bool Timer<async, T...>::is_running() {
	return thread.get_id() != std::thread().get_id();
}

template<bool async, typename... T>
inline void Timer<async, T...>::handle_events() {
	if (async) {
		std::cout << "[Timer Error] async threads do not need to call handle_events()" << std::endl;
		ASSERT(false);
	}

	size_t publish_count = publish_counter;

	for (size_t i = 0; i < publish_count; i++) {
		newsletter.publish();
	}

	publish_counter -= publish_count;
}
