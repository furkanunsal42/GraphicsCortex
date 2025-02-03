#pragma once
#include "Newsletter.h"

template<typename... T>
void Newsletter<T...>::publish(T... value) {
	for (auto iterator = _subscribers.begin(); iterator != _subscribers.end(); iterator++)
		iterator->second(value...);
}

template<typename... T>
size_t Newsletter<T...>::subscribe(std::function<void(T...)> callback) {
	size_t id = _next_id++;
	_subscribers.emplace_back(id, callback);
	return id;
}

template<typename ...T>
void Newsletter<T...>::unsubscribe(size_t subscripstion_id)
{
	_subscribers.erase(std::find_if(_subscribers.begin(), _subscribers.end(), [&](const std::pair<size_t, std::function<void(T...)>>& element) {
		return element.first == subscripstion_id;
		}));
}

template<typename ...T>
void Newsletter<T...>::unsubscribe_everyone()
{
	_subscribers.clear();
}

