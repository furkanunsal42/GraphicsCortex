#pragma once

struct RenderParameters {
	bool blend = false;
	bool clip_distance = false;
	//bool color_logic_op = false;
	bool cull_face = false;
	bool depth_clamp = false;
	bool depth_test = false;
	bool dither = false;
	bool framebuffer_srgb = false;
	bool line_smooth = false;
	bool multisample = false;
	bool primitive_restart = false;
	bool primitive_restart_fixed_index = false;
	// bool resterizer_discard = false;
	bool sample_shading = false;
	bool sample_mask = false;
	bool scissor_test = false;
	bool stencil_test = false;
	bool program_point_size = false;
};