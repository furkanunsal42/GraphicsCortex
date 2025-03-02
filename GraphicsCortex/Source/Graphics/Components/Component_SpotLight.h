#pragma once
#include "Component/Component.h"

#include "vec3.hpp"

class LightComponent : public Component {
public:

	enum Type {
		directional,
		point, 
		spot,
	};

	LightComponent(Type type = Type::directional, glm::vec3 color = glm::vec3(1), float max_angle = 3.14 / 4):
		type(type), color(color), max_angle(max_angle) {}

	Type type;
	glm::vec3 color;
	float max_angle;
};