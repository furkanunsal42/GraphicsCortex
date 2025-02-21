#pragma once

#include "Component/Component.h"

#include "mat4x4.hpp"

class TransformComponent : public Component {
public:

	glm::mat4 transform = glm::mat4(1);

};