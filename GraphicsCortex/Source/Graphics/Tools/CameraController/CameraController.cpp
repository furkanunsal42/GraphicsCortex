#include "CameraController.h"

#include "gtx/matrix_decompose.hpp"

CameraController::CameraController()
{
	camera.mouse_sensitivity = 0.25;
}

void CameraController::handle_movements(Window& window, double deltatime)
{
	if (first_handle_movement) {
		window.newsletters->on_mouse_scroll_events.subscribe([&](const glm::dvec2& scroll) {
			camera_distance += -scroll.y * scroll_sensitivity;
			});
		first_handle_movement = false;
	}

	if (window.get_mouse_button(Window::MouseButton::BUTTON2) == Window::PressAction::PRESS) {
		if (!movement_focus)
			cursor_position_when_movement_begin = window.get_cursor_position();
		movement_focus = true;
	}
	if (window.get_mouse_button(Window::MouseButton::BUTTON2) == Window::PressAction::RELEASE) {
		movement_focus = false;
	}

	if (movement_focus) {

		glm::dvec2 position = window.get_cursor_position();
		float input_rotation_x = glm::radians((float)(camera.mouse_sensitivity * -(position.y - cursor_position_when_movement_begin.y)));
		float input_rotation_y = glm::radians((float)(camera.mouse_sensitivity * -(position.x - cursor_position_when_movement_begin.x)));
		
		cursor_position_when_movement_begin = window.get_cursor_position();
		
		rotation_x += input_rotation_x;
		rotation_y += input_rotation_y;

		camera.rotation_quat = glm::quat(glm::vec3(rotation_x, 0, 0));
		camera.rotation_quat = glm::quat(glm::vec3(0, rotation_y, 0)) * camera.rotation_quat;
	}

	glm::vec3 forward_vector = (camera.rotation_quat * glm::vec3(0, 0, -1));
	camera.position = camera_origin - forward_vector * camera_distance;

	camera.update_matrixes();
}

void CameraController::handle_movements(glm::vec2 cursor_position, glm::vec2 scroll, bool mouse_button_left, bool mouse_button_right, bool mouse_button_middle)
{
	camera_distance += -scroll.y * scroll_sensitivity;

	if (mouse_button_right) {
		if (!movement_focus) {
			cursor_position_when_movement_begin = cursor_position;

			// CALCULATE ONCE: Check if camera is inverted at the start of the click
			glm::vec3 current_up = camera.rotation_quat * glm::vec3(0, 1, 0);
			float up_dot = glm::dot(current_up, glm::vec3(0, 1, 0));
			cached_horizontal_sign = (up_dot >= 0.0f) ? 1.0f : -1.0f;

			movement_focus = true;
		}
	}
	else {
		movement_focus = false;
	}

	if (movement_focus) {
		glm::dvec2 position = cursor_position;

		// Your exact math and casting
		float delta_x = glm::radians((position.x - cursor_position_when_movement_begin.x));
		float delta_y = glm::radians((position.y - cursor_position_when_movement_begin.y));
		cursor_position_when_movement_begin = cursor_position;

		// Use the CACHED sign from when the click started
		float yaw_angle = -delta_x * camera.mouse_sensitivity * cached_horizontal_sign;
		float pitch_angle = -delta_y * camera.mouse_sensitivity;

		// Standard incremental application
		glm::quat global_yaw = glm::angleAxis(yaw_angle, glm::vec3(0, 1, 0));

		glm::vec3 local_right = camera.rotation_quat * glm::vec3(1, 0, 0);
		glm::quat local_pitch = glm::angleAxis(pitch_angle, local_right);

		camera.rotation_quat = global_yaw * local_pitch * camera.rotation_quat;
		camera.rotation_quat = glm::normalize(camera.rotation_quat);
	}

	glm::vec3 forward_vector = (camera.rotation_quat * glm::vec3(0, 0, -1));
	camera.position = camera_origin - forward_vector * camera_distance;

	camera.update_matrixes();
}

glm::vec3 CameraController::get_camera_position()
{
	camera.position = camera_origin - get_camera_forward() * camera_distance;
	return camera.position;
}

glm::vec3 CameraController::get_camera_forward()
{
	camera.rotation_quat = glm::quat(glm::vec3(rotation_x, 0, 0));
	camera.rotation_quat = glm::quat(glm::vec3(0, rotation_y, 0)) * camera.rotation_quat;
	glm::vec3 forward_vector = (camera.rotation_quat * glm::vec3(0, 0, -1));
	return forward_vector;
}
