#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <vec2.hpp>

#include "Window.h"
#include "GraphicsContext.h"

class Monitor;
class OpenGLBackend;

class Frame2 : public Window, public GraphicsContext {
public:


	Frame2(
		uint32_t window_width, uint32_t window_height,
		const std::string& window_name,
		const Monitor& full_screen_target_monitor,
		GraphicsAPI api = OpenGL, uint32_t context_version_major = 4, uint32_t context_version_minor = 6
	);
	Frame2(
		uint32_t window_width, uint32_t window_height,
		const std::string& window_name,
		const Frame2& shared_context,
		const Monitor& full_screen_target_monitor,
		GraphicsAPI api = OpenGL, uint32_t context_version_major = 4, uint32_t context_version_minor = 6
	);
	Frame2(
		uint32_t window_width, uint32_t window_height,
		const std::string& window_name,
		GraphicsAPI api = OpenGL, uint32_t context_version_major = 4, uint32_t context_version_minor = 6
	);
	Frame2(
		uint32_t window_width, uint32_t window_height,
		const std::string& window_name,
		const Frame2& shared_context,
		GraphicsAPI api = OpenGL, uint32_t context_version_major = 4, uint32_t context_version_minor = 6
	);

	static Frame2 create_from_current();

	Frame2(const Frame2& other) = delete;
	~Frame2();

	void release();

	
private:

	friend OpenGLBackend;
};