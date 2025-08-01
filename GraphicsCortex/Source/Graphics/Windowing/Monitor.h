#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <span>

#include "vec2.hpp"
#include "vec3.hpp"

class Window;

class Monitor {
public:
	
	static const std::span<Monitor> get_all_monitors();
	bool operator==(const Monitor& other);

	glm::ivec2 get_position() const;
	glm::ivec2 get_resolution() const;
	glm::ivec2 get_position_undecorated() const;
	glm::ivec2 get_resolution_undecorated() const;
	glm::ivec2 get_physical_size_mm() const;
	glm::vec2 get_content_scale() const;
	std::u8string get_name() const;
	glm::ivec3 get_color_bits() const;
	int32_t get_refresh_rate() const;
	
private:
	Monitor(void* monitor_ptr = nullptr);

	void* monitor_ptr = nullptr;

	static bool monitors_up_to_date;
	static int32_t monitor_count;
	static void** all_monitors;

	friend Window;
};