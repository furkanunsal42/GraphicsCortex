#include "CortexMath.h"

std::ostream& operator<<(std::ostream& os, const glm::vec2 vector){
	os << "vec2(" << vector.x << ", " << vector.y << ")";
	return os;
}
std::ostream& operator<<(std::ostream& os, const glm::vec3 vector){
	os << "vec3(" << vector.x << ", " << vector.y  << ", " << vector.z << ")";
	return os;
}
std::ostream& operator<<(std::ostream& os, const glm::vec4 vector){
	os << "vec4(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
	return os;
}
