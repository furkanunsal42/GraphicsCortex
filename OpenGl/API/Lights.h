#pragma once
#include "Config.h"

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
	Program* program;
	glm::vec3 color;
		
	AmbiantLight(const glm::vec3& color, Program& program);
	static int count;
	void update_uniforms();
};

class DirectionalLight : public Light {
public:
	std::string shader_name = "d_lights";

	Program* program;
	glm::vec3 color;
	glm::vec3 direction;

	DirectionalLight(const glm::vec3& direction, const glm::vec3& color, Program& program);
	static int count;
	void update_uniforms();
};

class PointLight : public Light {
public:
	std::string shader_name = "p_lights";

	Program* program;
	glm::vec3 color;
	glm::vec3 position;
	float constant_term;
	float linear_term;
	float exponential_term;

	PointLight(const glm::vec3& position, const glm::vec3& color, float constant_term, float linear_term, float exponential_term, Program& program);
	static int count;
	void update_uniforms();

};

class SpotLight : public Light {
public:
	std::string shader_name = "s_lights";

	Program* program;
	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 direction;
	float constant_term;
	float linear_term;
	float exponential_term;
	float angle;

	SpotLight(const glm::vec3& position, const glm::vec3& direction,  const glm::vec3& color, float constant_term, float linear_term, float exponential_term, float angle,  Program& program);
	static int count;
	void update_uniforms();

};