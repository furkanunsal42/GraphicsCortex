#include "Monitor.h"
#include "Library.h"
#include "GLFW/glfw3.h"
#include <iostream>

bool Monitor::monitors_up_to_date = false;
void** Monitor::all_monitors = nullptr;
int32_t Monitor::monitor_count = 0;

const std::span<Monitor> Monitor::get_all_monitors()
{
	OpenGLBackend::_init_glfw();

	if (!monitors_up_to_date) {
		all_monitors = (void**)glfwGetMonitors(&monitor_count);
		monitors_up_to_date = true;
	}

	glfwSetMonitorCallback([](GLFWmonitor* monitor, int event) {
		monitors_up_to_date = false;
		if (event == GLFW_CONNECTED)
			std::cout << "[GraphicsCortex Info] New Monitor is connected" << std::endl;
		if (event == GLFW_DISCONNECTED)
			std::cout << "[GraphicsCortex Info] A Monitor is disconnected" << std::endl;
		});

	return std::span<Monitor>((Monitor*)all_monitors, (size_t)monitor_count);
}

bool Monitor::operator==(const Monitor& other)
{
	return monitor_ptr == other.monitor_ptr;
}

glm::ivec2 Monitor::get_position() const
{
	glm::ivec2 position;
	glfwGetMonitorPos((GLFWmonitor*)monitor_ptr, &position.x, &position.y);
	return position;
}

glm::ivec2 Monitor::get_resolution() const
{
	glm::ivec2 resolution;
	const GLFWvidmode* video_mode = glfwGetVideoMode((GLFWmonitor*)monitor_ptr);
	resolution.x = video_mode->width;
	resolution.y = video_mode->height;
	return resolution;
}

glm::ivec2 Monitor::get_position_undecorated() const
{
	glm::ivec2 work_area_position;
	glm::ivec2 work_area_size;
	glfwGetMonitorWorkarea((GLFWmonitor*)monitor_ptr, &work_area_position.x, &work_area_position.y, &work_area_size.x, &work_area_size.y);
	return work_area_position;
}

glm::ivec2 Monitor::get_resolution_undecorated() const
{
	glm::ivec2 work_area_position;
	glm::ivec2 work_area_size;
	glfwGetMonitorWorkarea((GLFWmonitor*)monitor_ptr, &work_area_position.x, &work_area_position.y, &work_area_size.x, &work_area_size.y);
	return work_area_size;
}

glm::ivec2 Monitor::get_physical_size_mm() const
{
	glm::ivec2 size_millimeters;
	glfwGetMonitorPhysicalSize((GLFWmonitor*)monitor_ptr, &size_millimeters.x, &size_millimeters.y);
	return size_millimeters;
}

glm::vec2 Monitor::get_content_scale() const
{
	glm::vec2 content_scale;
	glfwGetMonitorContentScale((GLFWmonitor*)monitor_ptr, &content_scale.x, &content_scale.y);
	return content_scale;
}

std::u8string Monitor::get_name() const
{
	const char* name = glfwGetMonitorName((GLFWmonitor*)monitor_ptr);
	return std::u8string((const char8_t*)name);
}

glm::ivec3 Monitor::get_color_bits() const
{
	glm::ivec3 color_bits;
	const GLFWvidmode* video_mode = glfwGetVideoMode((GLFWmonitor*)monitor_ptr);
	color_bits.r = video_mode->redBits;
	color_bits.g = video_mode->greenBits;
	color_bits.b = video_mode->blueBits;
	return color_bits;
}

int32_t Monitor::get_refresh_rate() const
{
	const GLFWvidmode* video_mode = glfwGetVideoMode((GLFWmonitor*)monitor_ptr);
	return video_mode->refreshRate;
}

Monitor::Monitor(void* monitor_ptr)
{
	this->monitor_ptr = monitor_ptr;
}

