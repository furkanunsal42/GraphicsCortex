#pragma once
#include "SharedPtr.h"
#include "SharedPtr_Program.h"

#include "Lights.h"

class Light_s {
public:

	_SHARED_POINTER_DEFINITIONS(Light)

		Light_s() :
		obj(std::make_shared<Light>()) {}

};

class AmbiantLight_s {
public:

	_SHARED_POINTER_DEFINITIONS(AmbiantLight)

		AmbiantLight_s(const glm::vec3& color, Program_s program) :
		obj(std::make_shared<AmbiantLight>(color, program)) {}

};

class DirectionalLight_s {
public:

	_SHARED_POINTER_DEFINITIONS(DirectionalLight)

		DirectionalLight_s(const glm::vec3& direction, const glm::vec3& color, Program_s program) :
		obj(std::make_shared<DirectionalLight>(direction, color, program)) {}
};

class PointLight_s {
public:

	_SHARED_POINTER_DEFINITIONS(PointLight)

		PointLight_s(const glm::vec3& position, const glm::vec3& color, float constant_term, float linear_term, float exponential_term, Program_s program) :
		obj(std::make_shared<PointLight>(position, color, constant_term, linear_term, exponential_term, program)) {}

};

class SpotLight_s {
public:

	_SHARED_POINTER_DEFINITIONS(SpotLight)

		SpotLight_s(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float constant_term, float linear_term, float exponential_term, float angle, Program_s program) :
		obj(std::make_shared<SpotLight>(position, direction, color, constant_term, linear_term, exponential_term, angle, program)) {}

};