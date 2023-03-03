#pragma once
#include "SharedPtr.h"
#include "Lights.h"
#include "Graphic.h"

class Light_s{
public:

	_SHARED_POINTER_DEFINITIONS(Light)

	Light_s() :
		obj(std::make_shared<Light>()) {}

};

class AmbiantLight_s{
public:

	_SHARED_POINTER_DEFINITIONS(AmbiantLight)

	AmbiantLight_s(const glm::vec3& color, Program_s program) :
		obj(std::make_shared<AmbiantLight>(color, program)) {}

};

class DirectionalLight_s{
public:

	_SHARED_POINTER_DEFINITIONS(DirectionalLight)

	DirectionalLight_s(const glm::vec3& direction, const glm::vec3& color, Program_s program) :
		obj(std::make_shared<DirectionalLight>(direction, color, program)) {}
};

class PointLight_s{
public:

	_SHARED_POINTER_DEFINITIONS(PointLight)

	PointLight_s(const glm::vec3& position, const glm::vec3& color, float constant_term, float linear_term, float exponential_term, Program_s program) :
		obj(std::make_shared<PointLight>(position, color, constant_term, linear_term, exponential_term, program)) {}

};

class SpotLight_s{
public:

	_SHARED_POINTER_DEFINITIONS(SpotLight)

	SpotLight_s(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float constant_term, float linear_term, float exponential_term, float angle, Program_s program) :
		obj(std::make_shared<SpotLight>(position, direction, color, constant_term, linear_term, exponential_term, angle, program)) {}

};

class Graphic_s{
public:

	_SHARED_POINTER_DEFINITIONS(Graphic)

	Graphic_s() :
		obj(std::make_shared<Graphic>()) {}

	Graphic_s(Mesh_s mesh, Material_s material, Program_s renderer) :
		obj(std::make_shared<Graphic>(mesh, material, renderer)) {}

	Graphic_s(Mesh_s mesh, UnorderedMaterial_s material, Program_s renderer) :
		obj(std::make_shared<Graphic>(mesh, material, renderer)) {}

	Graphic_s(const std::vector<float>& verticies, int data_dim) :
		obj(std::make_shared<Graphic>(verticies, data_dim)) {}

	Graphic_s(Material_s material, Program_s renderer) :
		obj(std::make_shared<Graphic>(material, renderer)) {}

	Graphic_s(UnorderedMaterial_s material, Program_s renderer) :
		obj(std::make_shared<Graphic>(material, renderer)) {}

};