#pragma once

struct RenderParameters {
	bool blend;
	bool clip_distance;
	bool cull_face;
	bool depth_clamp;
	bool depth_test;
	bool dither;
	bool framebuffer_srgb;
	bool line_smooth;
	bool multisample;
	bool sample_shading;
	bool sample_mask;
	bool scissor_test;
	bool stencil_test;
	bool program_point_size;
};