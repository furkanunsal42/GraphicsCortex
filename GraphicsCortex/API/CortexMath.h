#pragma once

#include <iostream>
#include <type_traits>
#include "Model.h"

template <typename T>
class Vec2 {
public:
	T x = NULL;
	T y = NULL;

	Vec2(const T& x = NULL, const T& y = NULL) :
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
public:
	T x = NULL;
	T y = NULL;
	T z = NULL;

	Vec3(const T& x = NULL, const T& y = NULL, const T& z = NULL) :
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
public:
	T x = NULL;
	T y = NULL;
	T z = NULL;
	T w = NULL;

	Vec4(const T& x = NULL, const T& y = NULL, const T& z = NULL, const T& w = NULL) :
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
public:
	AABB2(const Vec2<float> position, const Vec2<float> size) :
		position(position), size(size) {}

	Vec2<float> position; // top-left corner
	Vec2<float> size;

	template<typename T>
	bool does_contain(Vec2<T> point_position) {
		if (position.x > point_position.x || position.x + size.x <= point_position.x)
			return false;
		if (position.y > point_position.y || position.y + size.y <= point_position.y)
			return false;
		return true;
	}
	Model generate_model() {
		std::vector<float> verticies{
			0,			0,			0,
			size.x,		0,			0,
			size.x,		-size.y,	0,
			0,			-size.y,	0,
		};

		std::vector<unsigned int> indicies{
			0, 1, 2,
			0, 2, 3,
		};
		
		return Model(verticies, indicies);
	}
};

class AABB3 {
public:
	AABB3(const Vec3<float> position, const Vec3<float> size) : 
		position(position), size(size) {}

	Vec3<float> position; // bottom_left_back corner (min x, min y, min z)
	Vec3<float> size;

	template<typename T>
	bool does_contain(Vec3<T> point_position);
	Model generate_model();
};