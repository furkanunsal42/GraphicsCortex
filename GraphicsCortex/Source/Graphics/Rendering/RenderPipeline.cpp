#include "RenderPipeline.h"

RenderPipeline::RenderPipeline(int width, int height, TextureBase2::ColorTextureFormat internal_format, int multisample)
{
	if (multisample == 0)
		framebuffer->attach_color(0, std::make_shared<Renderbuffer>(width, height, internal_format, multisample));
	else 
		framebuffer->attach_color(0, std::make_shared<Texture2D>(width, height, internal_format, multisample));
}

void RenderPipeline::push_render_pass(std::shared_ptr<RenderPass> renderpass)
{
	passes.push_back(renderpass);
}

void RenderPipeline::render(Scene& scene)
{

	for (int i = 0; i < passes.size(); i++) {
		passes[i]->on_render(i, *this, scene);
	}
}
