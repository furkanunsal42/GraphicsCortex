#pragma once

#include <cstdint>
#include <memory>
#include "vec2.hpp"

class GraphicsContext {
public:

	enum GraphicsAPI {
		OpenGL,
		OpenGL_ES,
		Vulkan,
		None,
	};

	enum OpenGLProfile {
		Core,
		Compatibility,
	};

	GraphicsContext(
		GraphicsAPI api = OpenGL, uint32_t context_version_major = 4, uint32_t context_version_minor = 6
	);
	GraphicsContext(
		const GraphicsContext& shared_context,
		GraphicsAPI api = OpenGL, uint32_t context_version_major = 4, uint32_t context_version_minor = 6
	);

	static std::shared_ptr<GraphicsContext> create_from_current();

	GraphicsContext(const GraphicsContext& other) = delete;
	~GraphicsContext();

	void release();

	// graphics context
	void context_make_current();
	
	void set_context_version(uint32_t major, uint32_t minor);
	uint32_t get_context_version_major();
	uint32_t get_context_version_minor();

	void set_context_api(GraphicsAPI api);
	GraphicsAPI get_context_api();

	void set_context_forward_compatibility(bool value);
	bool get_context_forawrd_compatibility();

	void set_context_debug_mode(bool value);
	bool get_context_debug_mode();
	
	void set_context_profile(OpenGLProfile profile);
	OpenGLProfile get_context_profile();
	
	double handle_events(bool print_performances = true);

protected:

	void _initialize();
	bool _is_context_initialized = false;
	void* context = nullptr;
	void* context_shared = nullptr;

	// graphics context
	uint32_t ctx_version_major;
	uint32_t ctx_version_minor;
	GraphicsAPI ctx_api;
	bool ctx_forward_compatibility = false;
	bool ctx_debug_mode = true;
	OpenGLProfile ctx_profile = Compatibility;
};