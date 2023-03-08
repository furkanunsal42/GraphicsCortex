#pragma once

#include <iostream>
#include <type_traits>
#include "Model.h"
#include "glm.hpp"

template <typename T>
class Vec2 {
public:
	T x = NULL;
	T y = NULL;

	Vec2(const glm::vec2& glm_vector) :
		x(glm_vector.x), y(glm_vector.y) {}

	Vec2(const T& x = NULL, const T& y = NULL) :
		x(x), y(y) {}

	Vec2 operator+(const Vec2& other) const {
		return Vec2(x + other.x, y + other.y);
	}
	Vec2 operator-(const Vec2& other) const {
		return Vec2(x - other.x, y - other.y);
	}
	bool operator==(const Vec2& other) {
		if (x == other.x && y == other.y)
			return true;
		return false;
	}
	bool operator!=(const Vec2& other) {
		return !operator==(other);
	}

	glm::vec2 get_glm() const {
		return glm::vec2(x, y);
	}

	template<typename NUM>
	std::enable_if_t<std::is_arithmetic_v<NUM>, Vec2> operator*(const NUM& coefficient) const {
		return Vec2(x * coefficient, y * coefficient);
	}

	template<typename NUM>
	std::enable_if_t<std::is_arithmetic_v<NUM>, Vec2> operator/(const NUM& coefficient) const {
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

	Vec3(const glm::vec3& glm_vector) :
		x(glm_vector.x), y(glm_vector.y), z(glm_vector.z) {}

	Vec3(const T& x = NULL, const T& y = NULL, const T& z = NULL) :
		x(x), y(y), z(z) {}

	Vec3 operator+(const Vec3& other) const {
		return Vec3(x + other.x, y + other.y, z + other.z);
	}
	Vec3 operator-(const Vec3& other) const {
		return Vec3(x - other.x, y - other.y, z - other.z);
	}
	bool operator==(const Vec3& other) {
		if (x == other.x && y == other.y && z == other.z)
			return true;
		return false;
	}
	bool operator!=(const Vec3& other) {
		return !operator==(other);
	}
	glm::vec3 get_glm() const {
		return glm::vec3(x, y, z);
	}

	template<typename NUM>
	std::enable_if_t<std::is_arithmetic_v<NUM>, Vec3> operator*(const NUM& coefficient) const {
		return Vec3(x * coefficient, y * coefficient, z * coefficient);
	}

	template<typename NUM>
	std::enable_if_t<std::is_arithmetic_v<NUM>, Vec3> operator/(const NUM& coefficient) const {
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

	Vec4(const glm::vec4& glm_vector) :
		x(glm_vector.x), y(glm_vector.y), z(glm_vector.z), w(glm_vector.w) {}

	Vec4(const T& x = NULL, const T& y = NULL, const T& z = NULL, const T& w = NULL) :
		x(x), y(y), z(z), w(w) {}

	Vec4 operator+(const Vec4& other) const {
		return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
	}
	Vec4 operator-(const Vec4& other) const {
		return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
	}
	bool operator==(const Vec4& other) {
		if (x == other.x && y == other.y && z == other.z && w == other.w)
			return true;
		return false;
	}
	bool operator!=(const Vec4& other) {
		return !operator==(other);
	}

	glm::vec4 get_glm() const {
		return glm::vec4(x, y, z, w);
	}

	template<typename NUM>
	std::enable_if_t<std::is_arithmetic_v<NUM>, Vec4> operator*(const NUM& coefficient) const {
		return Vec4(x * coefficient, y * coefficient, z * coefficient, w * coefficient);
	}

	template<typename NUM>
	std::enable_if_t<std::is_arithmetic_v<NUM>, Vec4> operator/(const NUM& coefficient) const {
		return Vec4(x / coefficient, y / coefficient, z / coefficient, w / coefficient);
	}
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const Vec4<T>& other) {
	stream << "Vec4(" << other.x << ", " << other.y << ", " << other.z << ", " << other.w << ")";
	return stream;
}

// easier to type namings
typedef Vec2<float> vec2;	// default
typedef Vec2<int8_t> vec2c;
typedef Vec2<int16_t> vec2s;
typedef Vec2<int32_t> vec2i;
typedef Vec2<uint32_t> vec2ui;
typedef Vec2<uint64_t> vec2l;
typedef Vec2<float> vec2f;
typedef Vec2<double> vec2d;

typedef Vec3<float> vec3;	// default
typedef Vec3<int8_t> vec3c;
typedef Vec3<int16_t> vec3s;
typedef Vec3<int32_t> vec3i;
typedef Vec3<uint32_t> vec3ui;
typedef Vec3<uint64_t> vec3l;
typedef Vec3<float> vec3f;
typedef Vec3<double> vec3d;

typedef Vec4<float> vec4;	// default
typedef Vec4<int8_t> vec4c;
typedef Vec4<int16_t> vec4s;
typedef Vec4<int32_t> vec4i;
typedef Vec4<uint32_t> vec4ui;
typedef Vec4<uint64_t> vec4l;
typedef Vec4<float> vec4f;
typedef Vec4<double> vec4d;


class AABB2 {
public:
	AABB2(const Vec2<float> position = {0, 0}, const Vec2<float> size = {0, 0}) :
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

	bool operator==(const AABB2& other) {
		if (position == other.position && size == other.size)
			return true;
		return false;
	}
	bool operator!=(const AABB2& other) {
		return !operator==(other);
	}

	Model generate_model() const {
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

	bool operator==(const AABB3& other) {
		if (position == other.position && size == other.size)
			return true;
		return false;
	}
	bool operator!=(const AABB3& other) {
		return !operator==(other);
	}

	template<typename T>
	bool does_contain(Vec3<T> point_position);
	Model generate_model();
};
