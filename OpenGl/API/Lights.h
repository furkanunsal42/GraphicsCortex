#pragma once
#include <glm.hpp>
#include <string>

#include "ShaderCompiler.h"

class Light {
public:
	static int count;
	virtual void update_uniforms();
};

class AmbiantLight : public Light{
public:
	std::string shader_name = "a_lights";
	Program program;
	glm::vec3 color;
		
	AmbiantLight(const glm::vec3& color, const Program& program);
	static int count;
	void update_uniforms();
};

class DirectionalLight : public Light {
public:
	std::string shader_name = "d_lights";

	Program program;
	glm::vec3 color;
	glm::vec3 direction;

	DirectionalLight(const glm::vec3& direction, const glm::vec3& color, const Program& program);
	static int count;
	void update_uniforms();
};

class PointLight : public Light {
public:
	std::string shader_name = "p_lights";

	Program program;
	glm::vec3 color;
	glm::vec3 position;
	float constant_term;
	float linear_term;
	float exponential_term;

	PointLight(const glm::vec3& position, const glm::vec3& color, float constant_term, float linear_term, float exponential_term, const Program& program);
	static int count;
	void update_uniforms();

};