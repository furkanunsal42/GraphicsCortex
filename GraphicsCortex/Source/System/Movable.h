#pragma once

#include <stdint.h>
#include <utility>
#include <iostream>

template<typename T>
class Movable {
public:

	Movable(const T& value) :
		value(value) {
	}
	Movable& operator=(const T& value)
	{
		this->value = value;
		return *this;
	}

	Movable() = default;
	Movable(Movable& value) = default;
	Movable(Movable&& other) {
		std::swap(other.value, this->value);
	};
	Movable& operator=(const Movable& other) = default;
	Movable& operator=(Movable&& other) {
		std::swap(other.value, value);
		return *this;
	}

	T value = T();
};

template<typename T>
class MovableOnly {
public:

	MovableOnly(const T& value) :
		value(value) {
	}
	MovableOnly& operator=(const T& value)
	{
		this->value = value;
		return *this;
	}

	MovableOnly() = default;
	MovableOnly(MovableOnly& value) = delete;
	MovableOnly(MovableOnly&& other) {
		std::swap(other.value, this->value);
	};
	MovableOnly& operator=(const MovableOnly& other) = delete;
	MovableOnly& operator=(MovableOnly&& other) {
		std::swap(other.value, value);
		return *this;
	}

	T value = T();
};

typedef MovableOnly<uint32_t> gl_handle;
