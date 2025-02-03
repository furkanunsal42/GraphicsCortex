#pragma once

#include <cstdint>
#include <memory>
#include "vec2.hpp"
#include "vec4.hpp"
#include <vector>
#include "Newsletter.h"

struct GraphicsContextDescription;

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

	GraphicsContext(const GraphicsContextDescription& params);

	GraphicsContext(
		glm::ivec2 framebuffer_resolution,
		GraphicsAPI api = OpenGL, uint32_t context_version_major = 4, uint32_t context_version_minor = 6
	);
	GraphicsContext(
		glm::ivec2 framebuffer_resolution,
		GraphicsContext& shared_context,
		GraphicsAPI api = OpenGL, uint32_t context_version_major = 4, uint32_t context_version_minor = 6
	);

	static std::shared_ptr<GraphicsContext> create_from_current();

	GraphicsContext(const GraphicsContext& other) = delete;
	~GraphicsContext();

	void release();

	// events
	bool should_close();
	void set_should_close(bool value);

	void swap_buffers();
	void poll_events();
	void wait_events();
	void wait_events_timeout(double timeout_seconds);
	void post_empty_event();
	double handle_events(bool print_performance = true);

	// graphics context
	void context_make_current();
	
	uint32_t get_context_version_major();
	uint32_t get_context_version_minor();
	GraphicsAPI get_context_api();
	bool get_context_forward_compatibility();
	//bool get_context_debug_mode();
	OpenGLProfile get_context_profile();
	
	// default framebuffer
	glm::ivec2 get_framebuffer_resolution();
	glm::ivec4 get_framebuffer_color_bits();
	glm::ivec2 get_framebuffer_depth_stencil_bits();
	int32_t get_framebuffer_multisample_count();

	void set_framebuffer_swap_interval(int32_t value);

	// newsletters
	struct NewslettersBlock;
	NewslettersBlock* newsletters = nullptr;
protected:
	GraphicsContext() = default;

	GraphicsContext(void* context);

	struct NewslettersBlock {
		Newsletter<>						on_should_close_events;
		Newsletter<const glm::ivec2&>		on_framebuffer_resolution_events;
	};

	void _initialize(const GraphicsContextDescription& description);
	void* handle = nullptr;
};

struct GraphicsContextDescription {
	// graphics context
	void* context_shared = nullptr;
	uint32_t ctx_version_major = 4;
	uint32_t ctx_version_minor = 6;
	GraphicsContext::GraphicsAPI ctx_api = GraphicsContext::OpenGL;
	bool ctx_forward_compatibility = false;
	bool ctx_debug_mode = true;
	GraphicsContext::OpenGLProfile ctx_profile = GraphicsContext::Compatibility;

	// default framebuffer
	glm::ivec2 f_resolution = glm::ivec2(0, 0);
	glm::ivec4 f_color_bits = glm::ivec4(8, 8, 8, 8);
	glm::ivec2 f_depth_stencil_bits = glm::ivec2(24, 8);
	int32_t f_multisample_count = 1;
	bool f_srgb_enabled = false;
	bool f_double_buffered = true;
	int32_t f_swap_interval = 0;

	bool n_create_newsletters = true;
};
