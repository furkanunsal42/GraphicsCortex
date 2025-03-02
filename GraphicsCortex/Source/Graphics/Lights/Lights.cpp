# include "Lights.h"
#include <string>
#include <iostream>

AmbiantLight::AmbiantLight(const glm::vec3& color) : 
	color(color) {}

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color) :
	direction(direction), color(color) {}

PointLight::PointLight(const glm::vec3& color) :
	color(color) { }

SpotLight::SpotLight(const glm::vec3& direction,  const glm::vec3& color, float angle) :
	direction(direction), color(color), angle_radian(glm::cos(glm::radians(angle))) { }
