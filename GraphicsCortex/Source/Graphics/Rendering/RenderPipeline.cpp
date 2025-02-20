#include "RenderPipeline.h"
#include "Components/Component_MeshRenderer.h"
#include "Debuger.h"

RenderPipeline::RenderPipeline(int width, int height, TextureBase2::ColorTextureFormat internal_format, int multisample)
{
	texture = std::make_shared<Texture2D>(width, height, internal_format, 1, 0, multisample);
	framebuffer->attach_color(0, texture);
}

void RenderPipeline::push_render_pass(std::shared_ptr<RenderPass> renderpass)
{
	if (initialized) {
		std::cout << "[OpenGL Error] RenderPipeline::push_render_pass() is called after render() is called. RenderPipeline cannot be modified after initialization" << std::endl;
		ASSERT(false);
	}

	passes.push_back(renderpass);
}

void RenderPipeline::render(Scene& scene, Camera& camera)
{
	framebuffer->activate_draw_buffer(0);
	framebuffer->bind_draw();

	if (!initialized) {
		for (int32_t i = 0; i < passes.size(); i++) {
			std::shared_ptr<RenderPass>& pass = passes[i];
			pass->on_initialize(i, *this, scene);
		}
		initialized = true;
	}
	
	for (int32_t i = 0; i < passes.size(); i++) {
		std::shared_ptr<RenderPass>& pass = passes[i];
		pass->on_render(i, *this, scene, camera);
	}
}
