#include "Camera.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx\rotate_vector.hpp"

#include <iostream>
#include "Default_Assets.h"

Camera::Camera() :
	screen_width(100.0f), screen_height(100.0f) {}

Camera::Camera(float width, float height):
	screen_width(width), screen_height(height) {}

void Camera::update_matrixes() {
	// apply position and rotation to camera
	view_matrix = glm::mat4_cast(glm::inverse(rotation_quat));
	view_matrix = glm::translate(view_matrix, -position);

	if (perspective) {
		projection_matrix = glm::perspective(glm::radians(fov), screen_width / screen_height, min_distance, max_distance);
	}
	else {
		projection_matrix = glm::ortho(-screen_width / screen_height * ortho_size, screen_width / screen_height * ortho_size, -1.0f * ortho_size, 1.0f * ortho_size, min_distance, max_distance);
	}

}

void Camera::update_uniforms(Program& program) {
	program.update_uniform("camera_coords", position.x, position.y, position.z);
}

void Camera::handle_movements(GLFWwindow* window, double frame_time_ms) {
	if (glfwGetKey(window, GLFW_KEY_W) == 1) {
		glm::vec3 forward_vector = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 euler_angles = glm::eulerAngles(rotation_quat);
		forward_vector = glm::rotate(forward_vector, euler_angles.y, glm::vec3(0.0f, 1.0f, 0.0f));
		forward_vector = glm::normalize(forward_vector);
		position += forward_vector * (float)(movement_speed * frame_time_ms);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == 1) {
		glm::vec3 forward_vector = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 euler_angles = glm::eulerAngles(rotation_quat);
		forward_vector = glm::rotate(forward_vector, euler_angles.y, glm::vec3(0.0f, 1.0f, 0.0f));
		forward_vector = glm::normalize(forward_vector);
		position += -forward_vector * (float)(movement_speed * frame_time_ms);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == 1) {
		glm::vec3 forward_vector = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 euler_angles = glm::eulerAngles(rotation_quat);
		forward_vector = glm::rotate(forward_vector, euler_angles.y, glm::vec3(0.0f, 1.0f, 0.0f));
		forward_vector = glm::normalize(forward_vector);
		position += glm::cross(forward_vector, up_vector) * (float)(movement_speed * frame_time_ms);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == 1) {
		glm::vec3 forward_vector = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 euler_angles = glm::eulerAngles(rotation_quat);
		forward_vector = glm::rotate(forward_vector, euler_angles.y, glm::vec3(0.0f, 1.0f, 0.0f));
		forward_vector = glm::normalize(forward_vector);
		position += -glm::cross(forward_vector, up_vector) * (float)(movement_speed * frame_time_ms);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == 1) {
		position +=  up_vector * (float)(movement_speed * frame_time_ms);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == 1) {
		glm::vec3 forward_vector = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 euler_angles = glm::eulerAngles(rotation_quat);
		forward_vector = glm::rotate(forward_vector, euler_angles.y, glm::vec3(0.0f, 1.0f, 0.0f));
		forward_vector = glm::normalize(forward_vector);
		position += -up_vector * (float)(movement_speed * frame_time_ms);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == 1) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, screen_width / 2, screen_height / 2);
		mouse_focus = true;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == 1) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		mouse_focus = false;
	}
	if (mouse_focus) {
		double mouse_x, mouse_y;
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		//rotation.x += glm::radians((float)(mouse_sensitivity * -(mouse_y - screen_height / 2) / screen_height));
		//rotation.y += glm::radians((float)(mouse_sensitivity * -(mouse_x - screen_width / 2) / screen_width));

		glfwSetCursorPos(window, screen_width / 2, screen_height / 2);
	}
}

const glm::vec3& Camera::get_position() {
	return position;
}

const glm::quat& Camera::get_rotation() {
	return rotation_quat;
}

void Camera::set_position(const glm::vec3& position){
	this->position = position;
}
void Camera::set_rotation(const glm::quat& rotation){
	this->rotation_quat = rotation;
}
void Camera::set_rotation(const glm::vec3& rotation) {
	this->rotation_quat = glm::quat(rotation);
}
