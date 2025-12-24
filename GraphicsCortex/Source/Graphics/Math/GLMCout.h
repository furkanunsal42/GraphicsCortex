#pragma once

#include <iostream>
#include "glm.hpp"
#include "gtc/quaternion.hpp"

template<typename T>
std::ostream& operator<<(std::ostream& stream, const glm::vec<1, T>& vector) {
	return stream << vector.x;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const glm::vec<2, T>& vector) {
	return stream << vector.x << ", " << vector.y;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const glm::vec<3, T>& vector) {
	return stream << vector.x << ", " << vector.y << ", " << vector.z;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const glm::vec<4, T>& vector) {
	return stream << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, glm::qua<T> quat) {
	return stream << quat.x << ", " << quat.y << ", " << quat.z << ", " << quat.w;
}

