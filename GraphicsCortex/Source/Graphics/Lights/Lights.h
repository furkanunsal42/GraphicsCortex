#pragma once
#include "Config.h"

#include <glm.hpp>
#include <string>
#include <memory>

#include "ShaderCompiler.h"

class Scene;

class Light : public UpdatesDefaultUniforms {
private:
	static int temp_light_index;

	friend Scene;
public:
	void update_default_uniforms(Program& program);
};

class AmbiantLight : public Light{
private:
	static int temp_light_index;
	friend Scene;
public:
	glm::vec3 color;
		
	AmbiantLight(const glm::vec3& color = glm::vec3(0.1, 0.1, 0.1));
	void update_default_uniforms(Program& program);
};

class DirectionalLight : public Light {
private:
	static int temp_light_index;
	friend Scene;
public:
	static int count;
	glm::vec3 color;
	glm::vec3 direction;
	
	glm::vec3 position;
	glm::mat4 light_view_matrix;
	glm::mat4 light_projection_matrix;

	DirectionalLight(const glm::vec3& position = glm::vec3(0, 0, 0), const glm::vec3& direction = glm::vec3(0, -1, 0), const glm::vec3& color = glm::vec3(1, 1, 1));
	void update_matricies();
	void update_default_uniforms(Program& program);
};

class PointLight : public Light {
private:
	static int temp_light_index;
	friend Scene;
public:
	static int count;
	glm::vec3 color;
	glm::vec3 position;
	float constant_term;
	float linear_term;
	float exponential_term;

	PointLight(const glm::vec3& position, const glm::vec3& color, float constant_term, float linear_term, float exponential_term);
	void update_default_uniforms(Program& program);
};

class SpotLight : public Light {
private:
	static int temp_light_index;
	friend Scene;
public:
	static int count;
	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 direction;
	float constant_term;
	float linear_term;
	float exponential_term;
	float cos_angle;

	SpotLight(const glm::vec3& position, const glm::vec3& direction,  const glm::vec3& color, float constant_term, float linear_term, float exponential_term, float angle);
	void update_default_uniforms(Program& program);
};