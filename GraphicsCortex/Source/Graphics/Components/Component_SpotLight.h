#pragma once
#include "Component/Component.h"

#include "Lights/Lights.h"

class SpotLightComponent : public Component {
public:

	SpotLightComponent(SpotLight spot_light = SpotLight(glm::vec3(0), glm::vec3(0, -1, 0), glm::vec3(1), 3.14 / 2)) { this->spot_light = spot_light; }

	SpotLight spot_light = SpotLight(glm::vec3(0), glm::vec3(0, -1, 0), glm::vec3(1), 3.14 / 2);
};