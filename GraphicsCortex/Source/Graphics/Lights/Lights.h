#pragma once
#include "Config.h"

#include <glm.hpp>

class Light {
public:
};

class AmbiantLight : public Light{
public:
	AmbiantLight(const glm::vec3& color = glm::vec3(0.1, 0.1, 0.1));
	
	glm::vec3 color;
};

class DirectionalLight : public Light {
public:
	
	DirectionalLight(const glm::vec3& direction = glm::vec3(0, -1, 0), const glm::vec3& color = glm::vec3(1, 1, 1));
	
	glm::vec3 color;
	glm::vec3 direction;
};

class PointLight : public Light {
public:
	
	PointLight(const glm::vec3& position, const glm::vec3& color);
	
	glm::vec3 color;
	glm::vec3 position;
};

class SpotLight : public Light {
public:
	
	SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float angle);

	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 direction;
	float cos_angle;
};