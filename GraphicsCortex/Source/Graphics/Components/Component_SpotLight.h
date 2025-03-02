#pragma once
#include "Component/Component.h"

#include "Lights/Lights.h"

class SpotLightComponent : public Component {
public:

	SpotLightComponent(SpotLight spot_light = SpotLight()) { this->spot_light = spot_light; }

	SpotLight spot_light = SpotLight();
};