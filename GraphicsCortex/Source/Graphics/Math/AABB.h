#pragma once

#include "glm.hpp"

template<int32_t L, typename T>
class AABB {
public:
	typedef glm::vec<L, T> vec_t;

	AABB() = default;
	AABB(vec_t low, vec_t high);

	bool does_contain(const vec_t& point);
	bool operator==(const AABB<L, T>& other);
	AABB<L, T> intersection(const AABB<L, T>& other);

	vec_t low	= vec_t(0);
	vec_t high	= vec_t(0);
};

template<int32_t L, typename T>
inline AABB<L, T>::AABB(vec_t low, vec_t high) :
	low(low), high(high)
{
}

template<int32_t L, typename T>
inline bool AABB<L, T>::does_contain(const vec_t& point)
{
	return glm::all(glm::greaterThanEqual(point, low)) && glm::all(glm::lessThan(point, high));
}

template<int32_t L, typename T>
inline bool AABB<L, T>::operator==(const AABB& other)
{
	return low == other.low && high == other.high;
}

template<int32_t L, typename T>
inline AABB<L, T> AABB<L, T>::intersection(const AABB& other)
{
	AABB<L, T> candidate = AABB<L, T>(glm::max(low, other.low), glm::min(high, other.high));
	
	if (glm::any(glm::greaterThan(candidate.low, candidate.high)))
		return AABB<L, T>(vec_t(0), vec_t(0));
	
	return candidate;
}

typedef AABB<1, float> AABB1;
typedef AABB<2, float> AABB2;
typedef AABB<3, float> AABB3;
typedef AABB<4, float> AABB4;