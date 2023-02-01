#pragma once

#include <iostream>
#include <type_traits>
#include "Model.h"

template <typename T>
class Vec2 {
public:
	T x;
	T y;

	Vec2(const T& x, const T& y) : 
		x(x), y(y) {}

	Vec2 operator+(const Vec2& other) {
		return Vec2(x + other.x, y + other.y);
	}
	Vec2 operator-(const Vec2& other) {
		return Vec2(x - other.x, y - other.y);
	}

	template<typename NUM>
	std::enable_if_t<std::is_arithmetic_v<NUM>, Vec2> operator*(const NUM& coefficient) {
		return Vec2(x * coefficient, y * coefficient);
	}

	template<typename NUM>
	std::enable_if_t<std::is_arithmetic_v<NUM>, Vec2> operator/(const NUM& coefficient) {
		return Vec2(x / coefficient, y / coefficient);
	}
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const Vec2<T>& other) {
	stream << "Vec2(" << other.x << ", " << other.y << ")";
	return stream;
}

template <typename T>
class Vec3 {
	T x;
	T y;
	T z;

	Vec3(const T& x, const T& y, const T& z) :
		x(x), y(y), z(z) {}

	Vec3 operator+(const Vec3& other) {
		return Vec3(x + other.x, y + other.y, z + other.z);
	}
	Vec3 operator-(const Vec3& other) {
		return Vec3(x - other.x, y - other.y, z - other.z);
	}

	template<typename NUM>
	std::enable_if_t<std::is_arithmetic_v<NUM>, Vec3> operator*(const NUM& coefficient) {
		return Vec3(x * coefficient, y * coefficient, z * coefficient);
	}

	template<typename NUM>
	std::enable_if_t<std::is_arithmetic_v<NUM>, Vec3> operator/(const NUM& coefficient) {
		return Vec3(x / coefficient, y / coefficient, z / coefficient);
	}
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const Vec3<T>& other) {
	stream << "Vec3(" << other.x << ", " << other.y << ", " << other.z << ")";
	return stream;
}


template <typename T>
class Vec4 {
	T x;
	T y;
	T z;
	T w;

	Vec4(const T& x, const T& y, const T& z, const T& w) :
		x(x), y(y), z(z), w(w) {}

	Vec4 operator+(const Vec4& other) {
		return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
	}
	Vec4 operator-(const Vec4& other) {
		return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	template<typename NUM>
	std::enable_if_t<std::is_arithmetic_v<NUM>, Vec4> operator*(const NUM& coefficient) {
		return Vec4(x * coefficient, y * coefficient, z * coefficient, w * coefficient);
	}

	template<typename NUM>
	std::enable_if_t<std::is_arithmetic_v<NUM>, Vec4> operator/(const NUM& coefficient) {
		return Vec4(x / coefficient, y / coefficient, z / coefficient, w / coefficient);
	}
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const Vec4<T>& other) {
	stream << "Vec4(" << other.x << ", " << other.y << ", " << other.z << ", " << other.w << ")";
	return stream;
}


class AABB2 {
	Vec2<float> position; // bottom-left corner (min x, min y)
	Vec2<float> size;

	template<typename T>
	bool does_contain(Vec2<T> point_position);
	Model generate_model();
};

class AABB3 {
	Vec3<float> position; // bottom_left_back corner (min x, min y, min z)
	Vec3<float> size;

	template<typename T>
	bool does_contain(Vec3<T> point_position);
	Model generate_model();
};