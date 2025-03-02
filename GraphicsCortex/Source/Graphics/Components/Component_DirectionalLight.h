#pragma once
#include "Component/Component.h"

#include "Lights/Lights.h"

class DirectionalLightComponent : public Component {
public:

	DirectionalLightComponent(DirectionalLight directional_light = DirectionalLight()) { this->directional_light = directional_light; }

	DirectionalLight directional_light;
};