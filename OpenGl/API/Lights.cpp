# include "Lights.h"
#include <string>
#include <iostream>


void Light::update_uniforms() {
	std::cout << "[WARNING] light.update_uniforms() executed" << std::endl;
}

void Light::define_uniforms(int max_count) {
	std::cout << "[WARNING] light._define_uniforms(max_count) executed" << std::endl;
}

void Light::update_uniform_queue() {
	_uniform_update_queue.update_uniforms();
}

AmbiantLight::AmbiantLight(const glm::vec3& color, Program& renderer) :
	color(color)
{
	program = &renderer;
}

void AmbiantLight::update_uniforms() {
	std::string uniform_name = shader_name + "[" + std::to_string(count) + "]";
	program->update_uniform(uniform_name + ".color", color.x, color.y, color.z);

	// temp
	count++;
	program->update_uniform(shader_name+"_count", count);
}

void AmbiantLight::define_uniforms(int max_count) {
	if (_uniforms_defined)
		return;
	for (int count = 0; count < max_count; count++){
		std::string uniform_name = shader_name + "[" + std::to_string(count) + "]";
		program->define_uniform(uniform_name + ".color");
	}
	program->define_uniform(shader_name + "_count");
	
	_uniforms_defined = true;
}

int AmbiantLight::count = 0;

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color, Program& renderer) :
	direction(direction), color(color) {program = &renderer;}

void DirectionalLight::update_uniforms() {
	std::string uniform_name = shader_name + "[" + std::to_string(count) + "]";
	program->update_uniform(uniform_name + ".direction", direction.x, direction.y, direction.z);
	program->update_uniform(uniform_name + ".color", color.x, color.y, color.z);

	// temp
	count++;
	program->update_uniform(shader_name + "_count", count);
}

void DirectionalLight::define_uniforms(int max_count) {
	if (_uniforms_defined)
		return;
	for (int count = 0; count < max_count; count++) {
		std::string uniform_name = shader_name + "[" + std::to_string(count) + "]";
		program->define_uniform(uniform_name + ".direction");
		program->define_uniform(uniform_name + ".color");
	}
	program->define_uniform(shader_name + "_count");

	_uniforms_defined = true;
}

int	DirectionalLight::count = 0;

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, float constant_term, float linear_term, float exponential_term, Program& renderer) :
	position(position), color(color), constant_term(constant_term), linear_term(linear_term), exponential_term(exponential_term) {program = &renderer;}

void PointLight::update_uniforms() {
	std::string uniform_name = shader_name + "[" + std::to_string(count) + "]";
	program->update_uniform(uniform_name + ".position", position.x, position.y, position.z);
	program->update_uniform(uniform_name + ".color", color.x, color.y, color.z);
	program->update_uniform(uniform_name + ".constant_term", constant_term);
	program->update_uniform(uniform_name + ".linear_term", linear_term);
	program->update_uniform(uniform_name + ".exponential_term", exponential_term);

	// temp
	count++;
	program->update_uniform(shader_name + "_count", count);
}

void PointLight::define_uniforms(int max_count) {
	if (_uniforms_defined)
		return;
	for (int count = 0; count < max_count; count++) {
		std::string uniform_name = shader_name + "[" + std::to_string(count) + "]";
		program->define_uniform(uniform_name + ".position");
		program->define_uniform(uniform_name + ".color");
		program->define_uniform(uniform_name + ".constant_term");
		program->define_uniform(uniform_name + ".linear_term");
		program->define_uniform(uniform_name + ".exponential_term");
	}
	program->define_uniform(shader_name + "_count");

	_uniforms_defined = true;
}

int PointLight::count = 0;


SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction,  const glm::vec3& color, float constant_term, float linear_term, float exponential_term, float angle, Program& renderer) :
	position(position), direction(direction), color(color), constant_term(constant_term), linear_term(linear_term), exponential_term(exponential_term), cos_angle(glm::cos(glm::radians(angle))) {program = &renderer;}

void SpotLight::update_uniforms() {
	std::string uniform_name = shader_name + "[" + std::to_string(count) + "]";
	program->update_uniform(uniform_name + ".position", position.x, position.y, position.z);
	program->update_uniform(uniform_name + ".direction", direction.x, direction.y, direction.z);
	program->update_uniform(uniform_name + ".color", color.x, color.y, color.z);
	program->update_uniform(uniform_name + ".constant_term", constant_term);
	program->update_uniform(uniform_name + ".linear_term", linear_term);
	program->update_uniform(uniform_name + ".exponential_term", exponential_term);
	program->update_uniform(uniform_name + ".cos_angle", cos_angle);

	// temp
	count++;
	program->update_uniform(shader_name + "_count", count);
}

void SpotLight::define_uniforms(int max_count) {
	if (_uniforms_defined)
		return;
	for (int count = 0; count < max_count; count++) {
		std::string uniform_name = shader_name + "[" + std::to_string(count) + "]";
		program->define_uniform(uniform_name + ".position");
		program->define_uniform(uniform_name + ".direction");
		program->define_uniform(uniform_name + ".color");
		program->define_uniform(uniform_name + ".constant_term");
		program->define_uniform(uniform_name + ".linear_term");
		program->define_uniform(uniform_name + ".exponential_term");
		program->define_uniform(uniform_name + ".cos_angle");
	}
	program->define_uniform(shader_name + "_count");

	_uniforms_defined = true;
}

int SpotLight::count = 0;