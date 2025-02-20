#pragma once
#include "RenderPipeline.h"
#include "vec4.hpp"

class RenderPass_Clear : public RenderPass {
public:
	
	RenderPass_Clear() = default;
	RenderPass_Clear(glm::vec4 color) : color(color) { ; }
	RenderPass_Clear(float r, float g, float b, float a) : color(glm::vec4(r, g, b, a)) { ; }

	glm::vec4 color = glm::vec4(0, 0, 0, 1);

	void on_initialize(int pass_index, RenderPipeline& pipeline, Scene& scene) {

	}

	void on_render(int pass_index, RenderPipeline& pipeline, Scene& scene) {
		
		pipeline.framebuffer->clear_bound_drawbuffer(color.r, color.g, color.b, color.a);
	}
};