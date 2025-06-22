#pragma once

#include <cstdint>

struct RenderParameters {
public:
	
	static const int32_t maximum_clip_plane_count = 8;
	
	RenderParameters(bool effective = false);

	void apply() const;

	enum BlendFunction {
		ZERO, 
		ONE, 
		SOURCE_COLOR, 
		ONE_MINUS_SOURCE_COLOR, 
		DESTINATION_COLOR, 
		ONE_MINUS_DESTINATION_COLOR, 
		SOURCE_ALPHA, 
		ONE_MINUS_SOURCE_ALPHA, 
		DESTINATION_ALPHA, 
		ONE_MINUS_DESTINATION_ALPHA,
	};
	
	enum CullDirection {
		FRONT,
		BACK,
		FRONT_AND_BACK
	};

	enum StencilFunction {
		NEVER, 
		LESS, 
		LEQUAL, 
		GREATER, 
		GEQUAL, 
		EQUAL, 
		NOTEQUAL, 
		ALWAYS
	};

	enum StencilOperation {
		KEEP, 
		ZERO, 
		REPLACE, 
		INCR, 
		INCR_WRAP, 
		DECR, 
		DECR_WRAP,
		INVERT
	};

	bool effective;

	bool blend = true;
	BlendFunction blend_source_function = SOURCE_ALPHA;
	BlendFunction blend_target_function = ONE_MINUS_SOURCE_ALPHA;

	bool clip_plane[maximum_clip_plane_count];

	bool cull_face = true;
	CullDirection cull_face_direction = BACK;
	
	bool depth_test = true;
	bool depth_clamp = false;
	float depth_clamp_near_value = 0.0f;
	float depth_clamp_far_value = 1.0f;

	bool framebuffer_srgb = false;
	
	bool multisample = true;

	bool primitive_restart = false;
	bool primitive_restart_fixed_index = false;
	uint32_t primitive_restart_index = -1u;

	//bool scissor_test = false;

	bool stencil_test = false;
	StencilFunction stencil_function = ALWAYS;
	int32_t stencil_referance = 0;
	uint32_t stencil_mask = -1u;
	StencilOperation stencil_operation_on_stencil_fail = KEEP;
	StencilOperation stencil_operation_on_depth_fail = KEEP;
	StencilOperation stencil_operation_on_pass = KEEP;
	
	//bool stencil_test_seperate = false;
	//StencilOperation stencil_operation_backface_on_stencil_fail = KEEP;
	//StencilOperation stencil_operation_backface_on_depth_fail = KEEP;
	//StencilOperation stencil_operation_backface_on_pass = KEEP;
};