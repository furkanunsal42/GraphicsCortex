#pragma once
#include "Component/Component.h"

#include "Lights/Lights.h"

class PointLightComponent : public Component {
public:

	PointLightComponent(PointLight point_light = PointLight(glm::vec3(0), glm::vec3(1))) { this->point_light = point_light; }

	PointLight point_light = PointLight(glm::vec3(0), glm::vec3(1));
};