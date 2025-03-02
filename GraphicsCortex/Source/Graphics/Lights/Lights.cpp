# include "Lights.h"
#include <string>
#include <iostream>

AmbiantLight::AmbiantLight(const glm::vec3& color) : 
	color(color) {}

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color) :
	direction(direction), color(color) {}

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color) :
	position(position), color(color) { }

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction,  const glm::vec3& color, float angle) :
	position(position), direction(direction), color(color), cos_angle(glm::cos(glm::radians(angle))) { }
