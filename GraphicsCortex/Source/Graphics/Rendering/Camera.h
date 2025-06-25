#pragma once
#include "Config.h"

#include <GL/glew.h>
#include <glm.hpp>
#include <string>
#include <GLFW/glfw3.h>

#include "ShaderCompiler.h"


class Camera : public UpdatesDefaultUniforms{
public:
	glm::vec3 up_vector = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view_matrix = glm::mat4(1.0f);
	glm::mat4 projection_matrix = glm::mat4(1.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat rotation_quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	const glm::vec3& get_position();
	const glm::quat& get_rotation();

	void set_position(const glm::vec3& position);
	void set_rotation(const glm::quat& rotation);
	void set_rotation(const glm::vec3& rotation);

	bool perspective = true;
	float ortho_size = 1.0f;
	float fov = 45.0f;
	float min_distance = 0.1f;
	float max_distance = 100.0f;
	float screen_width = 1;
	float screen_height = 1;
	float movement_speed = 0.0025f;
	float mouse_sensitivity = 0.1;
	bool mouse_focus = false;
	Camera();
	Camera(float width, float height);
	void update_matrixes();
	void handle_movements(GLFWwindow* window, double frame_time_ms = 16.6f);
	void handle_movements(GLFWwindow* window, glm::vec2 mouse_rest_position, double frame_time_ms = 16.6f);

	void update_default_uniforms(Program& program);
};
