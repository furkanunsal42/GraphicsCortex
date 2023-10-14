#pragma once
#include "Config.h"

#include <glm.hpp>
#include <string>
#include <memory>

#include "ShaderCompiler.h"
#include "UnifromQueue.h"
#include "SharedPtr_Program.h"

class Light {
private:
	uniform_update_queue _uniform_update_queue;
public:
	bool _uniforms_defined = false; // needed to manualy reset if light is moved from a shader to another, not the expected use of this class but to make is possible, we have to make this parameter public.
	static int count;
	virtual void update_uniforms();

	std::shared_ptr<Program> program;

	template<typename T>
	void add_uniform_update_queue(uniform_update<T>* uniform_queue) {
		uniform_queue->program = program;
		uniform_queue->uniform_id = program->uniforms[uniform_queue->uniform_name].index;
		_uniform_update_queue.add_uniform_update(*uniform_queue);

	}
	template<typename T>
	void add_uniform_update_queue(dynamic_uniform_update<T>* dynamic_uniform_queue) {
		dynamic_uniform_queue->program = program;
		dynamic_uniform_queue->uniform_id = program->uniforms[dynamic_uniform_queue->uniform_name].index;
		_uniform_update_queue.add_uniform_update(*dynamic_uniform_queue);
	}

	void set_uniform_upadte_queue(uniform_update_queue& original) {
		_uniform_update_queue.copy(original);
		_uniform_update_queue.link_program(program);
		_uniform_update_queue.update_uniform_ids();
	}

	void set_uniform_upadte_queue(uniform_update_queue original) {
		_uniform_update_queue.copy(original);
		_uniform_update_queue.link_program(program);
		_uniform_update_queue.update_uniform_ids();
	}

	void update_uniform_queue();
};

class AmbiantLight : public Light{
public:
	static int count;
	std::string shader_name = "a_lights";
	glm::vec3 color;
		
	AmbiantLight(const glm::vec3& color, Program_s program);
	AmbiantLight(const glm::vec3& color = glm::vec3(0.1, 0.1, 0.1));
	void update_uniforms();
};

class DirectionalLight : public Light {
public:
	static int count;
	std::string shader_name = "d_lights";
	glm::vec3 color;
	glm::vec3 direction;
	
	glm::vec3 position;
	glm::mat4 light_view_matrix;
	glm::mat4 light_projection_matrix;

	DirectionalLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, Program_s program);
	DirectionalLight(const glm::vec3& position = glm::vec3(0, 0, 0), const glm::vec3& direction = glm::vec3(0, -1, 0), const glm::vec3& color = glm::vec3(1, 1, 1));
	void update_matricies();
	void update_uniforms();
};

class PointLight : public Light {
public:
	static int count;
	std::string shader_name = "p_lights";
	glm::vec3 color;
	glm::vec3 position;
	float constant_term;
	float linear_term;
	float exponential_term;

	PointLight(const glm::vec3& position, const glm::vec3& color, float constant_term, float linear_term, float exponential_term, Program_s program);
	void update_uniforms();
};

class SpotLight : public Light {
public:
	static int count;
	std::string shader_name = "s_lights";
	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 direction;
	float constant_term;
	float linear_term;
	float exponential_term;
	float cos_angle;

	SpotLight(const glm::vec3& position, const glm::vec3& direction,  const glm::vec3& color, float constant_term, float linear_term, float exponential_term, float angle, Program_s program);
	void update_uniforms();
};