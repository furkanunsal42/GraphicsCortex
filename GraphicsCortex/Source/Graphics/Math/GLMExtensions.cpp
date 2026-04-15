#include "GLMExtensions.h"

glm::ivec3 cortexglm::to_ivec3(glm::ivec3 size3, int32_t blank_value) {
	return glm::ivec3(size3.x, size3.y, size3.z);
}

glm::ivec3 cortexglm::to_ivec3(glm::ivec2 size2, int32_t blank_value) {
	return glm::ivec3(size2.x, size2.y, blank_value);
}

glm::ivec3 cortexglm::to_ivec3(glm::ivec1 size1, int32_t blank_value) {
	return glm::ivec3(size1.x, blank_value, blank_value);
}