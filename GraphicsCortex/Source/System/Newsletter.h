#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <cstdint>

template<typename... T>
class Newsletter {
public:

	void publish(T... value);
	size_t subscribe(std::function<void(T...)> callback);
	void unsubscribe(size_t subscripstion_id);

	void unsubscribe_everyone();

	constexpr static size_t invalid_id = -1;

private:

	size_t _next_id = 0;
	std::vector<std::pair<size_t, std::function<void(T...)>>> _subscribers;
	
};

#include "NewsletterTemplated.h"
