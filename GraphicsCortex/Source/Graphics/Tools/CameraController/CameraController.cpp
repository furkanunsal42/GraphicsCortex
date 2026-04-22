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
