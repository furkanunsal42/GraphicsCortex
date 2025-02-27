# include "Lights.h"
#include <string>
#include <iostream>

AmbiantLight::AmbiantLight(const glm::vec3& color) : 
	color(color) {}

DirectionalLight::DirectionalLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color) :
	direction(direction), color(color) {}

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, float constant_term, float linear_term, float exponential_term) :
	position(position), color(color), constant_term(constant_term), linear_term(linear_term), exponential_term(exponential_term) { }

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction,  const glm::vec3& color, float constant_term, float linear_term, float exponential_term, float angle) :
	position(position), direction(direction), color(color), constant_term(constant_term), linear_term(linear_term), exponential_term(exponential_term), cos_angle(glm::cos(glm::radians(angle))) { }
