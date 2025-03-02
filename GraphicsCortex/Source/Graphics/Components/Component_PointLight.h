#pragma once
#include "Component/Component.h"

#include "Lights/Lights.h"

class PointLightComponent : public Component {
public:

	PointLightComponent(PointLight point_light = PointLight()) { this->point_light = point_light; }

	PointLight point_light = PointLight();
};