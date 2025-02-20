#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class RenderPassClearTest : public TestBench {
public:

	bool run() {

		default_init();

		std::shared_ptr<Scene> scene = std::make_shared<Scene>();
		
		glm::ivec2 pipeline_resolution(512, 512);
		
		std::shared_ptr<RenderPipeline> pipeline = std::make_shared<RenderPipeline>(pipeline_resolution.x, pipeline_resolution.y, Texture2D::ColorTextureFormat::RGBA8, 0);
		pipeline->push_render_pass(std::make_shared<RenderPass_Clear>(glm::vec4(1, 0, 1, 1)));
		Camera cam;

		while (true) {
			default_window->handle_events(true);
			
			pipeline->render(*scene, cam);
			
			pipeline->framebuffer->blit_to_screen(0, 0, pipeline_resolution.x, pipeline_resolution.y, 0, 0, pipeline_resolution.x, pipeline_resolution.y, Framebuffer::Channel::COLOR, Framebuffer::Filter::NEAREST);
		}

		return true;
	}
};
