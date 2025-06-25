#include "AABB.h"

#include "glm.hpp"

AABB3::AABB3(glm::vec3 low, glm::vec3 high) :
	low(low), high(high)
{
}

bool AABB3::does_contain(const glm::vec3& point)
{
	return glm::all(glm::greaterThanEqual(point, low)) && glm::all(glm::lessThan(point, high));
}

bool AABB3::operator==(const AABB3& other)
{
	return low == other.low && high == other.high;
}

