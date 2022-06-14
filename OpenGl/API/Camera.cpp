#include "Camera.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx\rotate_vector.hpp"

Camera::Camera() :
	screen_width(100.0f), screen_height(100.0f) {}

Camera::Camera(float width, float height):
	screen_width(width), screen_height(height) {}

void Camera::update_matrixes() {
	// apply position and rotation to camera
	view_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(-1.0f, 0.0f, 0.0f));
	view_matrix = glm::rotate(view_matrix, glm::radians(rotation.y), glm::vec3(0.0f, -1.0f, 0.0f));
	view_matrix = glm::rotate(view_matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, -1.0f));
	view_matrix = glm::translate(view_matrix, -position);

	if (perspective) {
		projection_matrix = glm::perspective(glm::radians(fov), screen_width / screen_height, min_distance, max_distance);
	}
	else {
		projection_matrix = glm::ortho(-screen_width / screen_height, screen_width / screen_height, -1.0f, 1.0f);
	}

}

void Camera::handle_movements(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_W) == 1) {
		glm::vec3 forward_vector = glm::vec3(0.0f, 0.0f, -1.0f);
		forward_vector = glm::rotate(forward_vector, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		forward_vector = glm::normalize(forward_vector);
		position += forward_vector * movement_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == 1) {
		glm::vec3 forward_vector = glm::vec3(0.0f, 0.0f, -1.0f);
		forward_vector = glm::rotate(forward_vector, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		forward_vector = glm::normalize(forward_vector);
		position += -forward_vector * movement_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == 1) {
		glm::vec3 forward_vector = glm::vec3(0.0f, 0.0f, -1.0f);
		forward_vector = glm::rotate(forward_vector, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		forward_vector = glm::normalize(forward_vector);
		position += glm::cross(forward_vector, up_vector) * movement_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == 1) {
		glm::vec3 forward_vector = glm::vec3(0.0f, 0.0f, -1.0f);
		forward_vector = glm::rotate(forward_vector, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		forward_vector = glm::normalize(forward_vector);
		position += -glm::cross(forward_vector, up_vector) * movement_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == 1) {
		position +=  up_vector * movement_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == 1) {
		glm::vec3 forward_vector = glm::vec3(0.0f, 0.0f, -1.0f);
		forward_vector = glm::rotate(forward_vector, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		forward_vector = glm::normalize(forward_vector);
		position += -up_vector * movement_speed;
	}
}

