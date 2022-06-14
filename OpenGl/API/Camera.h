#pragma once

#include <GL\glew.h>
#include <glm.hpp>
#include <string>
#include <GLFW\glfw3.h>

class Camera {
public:
	glm::vec3 up_vector = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view_matrix = glm::mat4(1.0f);
	glm::mat4 projection_matrix = glm::mat4(1.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	bool perspective = true;
	float fov = 45.0f;
	float min_distance = 0.1f;
	float max_distance = 100.0f;
	float screen_width;
	float screen_height;
	float movement_speed = 0.015f;
	Camera();
	Camera(float width, float height);
	void update_matrixes();
	void user_input_controller(GLFWwindow* window, int key, int scancode, int action, int mods);
	void handle_movements(GLFWwindow* window);
};
