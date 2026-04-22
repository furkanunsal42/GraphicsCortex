#pragma once

#include "Camera.h"
#include "vec3.hpp"

#include "Window.h"

class CameraController {
public:

	CameraController();

	glm::vec3 camera_origin = glm::vec3(0);
	float camera_distance = 5;
	Camera camera;

	float scroll_sensitivity = 1.4;

	void handle_movements(Window& window, double deltatime);
	glm::vec3 get_camera_position();
	glm::vec3 get_camera_forward();

private:
	float rotation_x = 0;
	float rotation_y = 0;
	bool first_handle_movement = true;
	bool movement_focus = false;
	glm::dvec2 cursor_position_when_movement_begin;
};