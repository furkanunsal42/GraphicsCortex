#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include "unordered_map"

#include "Window.h"
#include "Monitor.h"
#include "Movable.h"

#include <chrono>

struct OpenGLBackend{
public:

	//static void initiailze();
	//static void release();

	static bool _is_glfw_initialized;
	static bool _is_glew_initialized;
	static std::chrono::time_point<std::chrono::system_clock> _opengl_initialization_time;

	static void _init_glfw();
	static void _init_glew();

	static void _release_glfw();
	static void _release_glew();
	
	enum ResourceType {
		Texture1D,
		Texture2D,
		Renderbuffer,
		Texture2DArray,
		Texture3D,
		Framebuffer,
		Buffer,
		VertexArrays
	};

	uint32_t ResourceType_to_GL_target(ResourceType type);

	template<size_t... varying_parameter_offsets>
	class DeviceResource {
	public:

		uint32_t id;
		ResourceType type;

		DeviceResource* successor = nullptr;
		DeviceResource* master = nullptr;

		void propage_varying_parameters() {
			bool this_is_master = master == nullptr;
			bool there_is_successor = successor != nullptr;

			if (there_is_successor) {
				for (size_t offset : {varying_parameter_offsets...})
					*(successor + offset) = *(this + offset);
				
			}

			for (size_t offset : {varying_parameter_offsets...}) {
				std::cout << offset << " ";
			}
			std::cout << std::endl;
		}

		void update_varying_parameters() {
			bool this_is_master = master == nullptr;
			
			if (this_is_master) 
				for (size_t offset : {varying_parameter_offsets...})
					*(master + offset) = *(this + offset);
				propage_varying_parameters();
			
		}

		static constexpr std::array<size_t, sizeof...(varying_parameter_offsets)> parameter_offsets;

		~DeviceResource() {
			
		}
	};
};
