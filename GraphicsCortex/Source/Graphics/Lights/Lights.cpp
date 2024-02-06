# include "Lights.h"
#include <string>
#include <iostream>

void Light::update_default_uniforms(Program& program) {

};

AmbiantLight::AmbiantLight(const glm::vec3& color) : 
	color(color) {}

void AmbiantLight::update_default_uniforms(Program& program) {
	program.update_uniform("a_lights[" + std::to_string(temp_light_index) + "].color", color);
	program.update_uniform("a_lights_count", temp_light_index + 1);

	temp_light_index++;
}

int AmbiantLight::temp_light_index = 0;

DirectionalLight::DirectionalLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color) :
	position(position), direction(direction), color(color)
{
	update_matricies();
}

void DirectionalLight::update_matricies() {
	light_view_matrix = glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));
	float scale = 300;
	light_projection_matrix = glm::ortho(-scale, scale, -scale, scale, -500.0f, 500.0f);
}

void DirectionalLight::update_default_uniforms(Program& program) {
	program.update_uniform("d_lights[" + std::to_string(temp_light_index) + "].color", color);
	program.update_uniform("d_lights[" + std::to_string(temp_light_index) + "].direction", direction);
	program.update_uniform("d_lights[" + std::to_string(temp_light_index) + "].view_matrix", light_view_matrix);
	program.update_uniform("d_lights[" + std::to_string(temp_light_index) + "].projection_matrix", light_projection_matrix);
	program.update_uniform("d_lights_count", temp_light_index + 1);

	temp_light_index++;
}

int DirectionalLight::temp_light_index = 0;

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, float constant_term, float linear_term, float exponential_term) :
	position(position), color(color), constant_term(constant_term), linear_term(linear_term), exponential_term(exponential_term) { }

void PointLight::update_default_uniforms(Program& program) {
	program.update_uniform("p_lights[" + std::to_string(temp_light_index) + "].position", position);
	program.update_uniform("p_lights[" + std::to_string(temp_light_index) + "].color", color);
	program.update_uniform("p_lights[" + std::to_string(temp_light_index) + "].constant_term", constant_term);
	program.update_uniform("p_lights[" + std::to_string(temp_light_index) + "].linear_term", linear_term);
	program.update_uniform("p_lights[" + std::to_string(temp_light_index) + "].exponential_term", exponential_term);
	program.update_uniform("p_lights_count", temp_light_index + 1);

	temp_light_index++;
}

int PointLight::temp_light_index = 0;

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction,  const glm::vec3& color, float constant_term, float linear_term, float exponential_term, float angle) :
	position(position), direction(direction), color(color), constant_term(constant_term), linear_term(linear_term), exponential_term(exponential_term), cos_angle(glm::cos(glm::radians(angle))) { }

void SpotLight::update_default_uniforms(Program& program) {
	program.update_uniform("s_lights[" + std::to_string(temp_light_index) + "].position", position);
	program.update_uniform("s_lights[" + std::to_string(temp_light_index) + "].direction", direction);
	program.update_uniform("s_lights[" + std::to_string(temp_light_index) + "].color", color);
	program.update_uniform("s_lights[" + std::to_string(temp_light_index) + "].constant_term", constant_term);
	program.update_uniform("s_lights[" + std::to_string(temp_light_index) + "].linear_term", linear_term);
	program.update_uniform("s_lights[" + std::to_string(temp_light_index) + "].exponential_term", exponential_term);
	program.update_uniform("s_lights[" + std::to_string(temp_light_index) + "].cos_angle", cos_angle);
	program.update_uniform("s_lights_count", temp_light_index + 1);

	temp_light_index++;
}

int SpotLight::temp_light_index = 0;
