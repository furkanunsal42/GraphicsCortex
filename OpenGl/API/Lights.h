#pragma once
#include "Config.h"

#include <glm.hpp>
#include <string>

#include "ShaderCompiler.h"

class Light {
public:
	bool _uniforms_defined = false; // needed to manualy reset if light is moved from a shader to another, not the expected use of this class but to make is possible, we have to make this parameter public.
	static int count;
	virtual void update_uniforms();
	virtual void define_uniforms(int max_count);
};

class AmbiantLight : public Light{
public:
	std::string shader_name = "a_lights";
	Program* program;
	glm::vec3 color;
		
	AmbiantLight(const glm::vec3& color, Program& program);
	static int count;
	void update_uniforms();
	void define_uniforms(int max_count);
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
	void define_uniforms(int max_count);
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
	void define_uniforms(int max_count);
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
	void define_uniforms(int max_count);
};