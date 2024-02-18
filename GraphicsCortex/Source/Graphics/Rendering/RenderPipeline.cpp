#include "RenderPipeline.h"
#include "Scene.h"

RenderPipeline::RenderPipeline(int width, int height, TextureBase2::ColorTextureFormat internal_format, int multisample) :
	width(width), height(height), internal_format(internal_format), multisample(multisample)
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
	scene._update_pipeline_rendering_list();

	for (int i = 0; i < passes.size(); i++) {
		passes[i]->on_render(i, *this, scene, scene.pipeline_rendering_list);
	}

	if (texture_stack.size() == 0) {
		std::cout << "[Pipeline Error] RenderPipeline finished execution without leaving a texture in stack" << std::endl;
		ASSERT(false);
	}
	std::shared_ptr<TextureBase2> color_texture = texture_stack.back();
	texture_stack.pop_back();

	if (std::dynamic_pointer_cast<Texture2D, TextureBase2>(color_texture))
		framebuffer->attach_color(0, std::dynamic_pointer_cast<Texture2D, TextureBase2>(color_texture), 0);
	if (std::dynamic_pointer_cast<Renderbuffer, TextureBase2>(color_texture))
		framebuffer->attach_color(0, std::dynamic_pointer_cast<Renderbuffer, TextureBase2>(color_texture));

	framebuffer->set_read_buffer(0);
	framebuffer->blit_to_screen(0, 0, width, height, 0, 0, width, height, Framebuffer::Channel::COLOR_DEPTH_STENCIL, Framebuffer::Filter::NEAREST);

	texture_stack.clear();
}
