#pragma once

#include <glm.hpp>

namespace cortexglm {
	
	glm::ivec3 to_ivec3(glm::ivec3 size3, int32_t blank_value = 0);
	glm::ivec3 to_ivec3(glm::ivec2 size2, int32_t blank_value = 0);
	glm::ivec3 to_ivec3(glm::ivec1 size1, int32_t blank_value = 0);
	
}


