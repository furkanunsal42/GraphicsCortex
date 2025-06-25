#pragma once

#include "vec3.hpp"
#include "vec4.hpp"

class AABB3 {
public:
	AABB3(glm::vec3 low, glm::vec3 high);

	glm::vec3 low;
	glm::vec3 high;

	bool does_contain(const glm::vec3& point);
	bool operator==(const AABB3& other);
};