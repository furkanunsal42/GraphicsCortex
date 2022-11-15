#pragma once
#include "Config.h"

#include <GL\glew.h>
#include <glm.hpp>
#include <string>
#include <GLFW\glfw3.h>

#include "ShaderCompiler.h"

class Camera {
public:
	glm::vec3 up_vector = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view_matrix = glm::mat4(1.0f);
	glm::mat4 projection_matrix = glm::mat4(1.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	bool perspective = true;
	float ortho_size = 1.0f;
	float fov = 45.0f;
	float min_distance = 0.1f;
	float max_distance = 100.0f;
	float screen_width;
	float screen_height;
	float movement_speed = 0.0025f;
	float mouse_sensitivity = 15.0f;
	bool mouse_focus = false;
	Camera();
	Camera(float width, float height);
	void update_matrixes();
	void update_uniforms(Program& program);
	void handle_movements(GLFWwindow* window, double frame_time_ms = 16.6f);
};
