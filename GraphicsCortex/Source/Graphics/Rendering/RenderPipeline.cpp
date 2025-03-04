#include "RenderPipeline.h"
#include "Components/Component_MeshRenderer.h"
#include "Debuger.h"

RenderPipeline::RenderPipeline(int width, int height, TextureBase2::ColorTextureFormat internal_format, int multisample)
{
	texture = std::make_shared<Texture2D>(width, height, internal_format, 1, 0, multisample);
	framebuffer->attach_color(0, texture);

	this->width = width;
	this->height = height;
	this->internal_format = internal_format;
	this->depthstencil_internal_format = invalid_depth_stencil_format;
	this->multisample = multisample;
}

RenderPipeline::RenderPipeline(int width, int height, Texture2D::ColorTextureFormat internal_format, Texture2D::DepthStencilTextureFormat depth_stencil_format, int multisample) {

	if (depth_stencil_format == Texture2D::DepthStencilTextureFormat::DEPTH_COMPONENT32F) {
		depth = std::make_shared<Texture2D>(width, height, depth_stencil_format, 1, 0, multisample);
		framebuffer->attach_depth(depth);
	}
	else if (depth_stencil_format == Texture2D::DepthStencilTextureFormat::DEPTH_COMPONENT24) {
		depth = std::make_shared<Texture2D>(width, height, depth_stencil_format, 1, 0, multisample);
		framebuffer->attach_depth(depth);
	}
	else if (depth_stencil_format == Texture2D::DepthStencilTextureFormat::DEPTH_COMPONENT16) {
		depth = std::make_shared<Texture2D>(width, height, depth_stencil_format, 1, 0, multisample);
		framebuffer->attach_depth(depth);
	}
	else if (depth_stencil_format == Texture2D::DepthStencilTextureFormat::DEPTH32F_STENCIL8) {
		depth_stencil = std::make_shared<Texture2D>(width, height, depth_stencil_format, 1, 0, multisample);
		framebuffer->attach_depth_stencil(depth_stencil);
	}
	else if (depth_stencil_format == Texture2D::DepthStencilTextureFormat::DEPTH24_STENCIL8)  {
		depth_stencil = std::make_shared<Texture2D>(width, height, depth_stencil_format, 1, 0, multisample);
		framebuffer->attach_depth_stencil(depth_stencil);
	}
	else if (depth_stencil_format == Texture2D::DepthStencilTextureFormat::STENCIL_INDEX8) {
		stencil = std::make_shared<Texture2D>(width, height, depth_stencil_format, 1, 0, multisample);
		framebuffer->attach_stencil(stencil);
	}
	else {
		std::cout << "[OpenGL Error] RenderPipeline is tried to be constructed with an invalid DepthStencilTextureFormat : " << (int32_t)depth_stencil_format << " given value must be a sized depth, stencil or depth-stencil format" << std::endl;
		ASSERT(false);
	}

	texture = std::make_shared<Texture2D>(width, height, internal_format, 1, 0, multisample);
	framebuffer->attach_color(0, texture);

	this->width = width;
	this->height = height;
	this->internal_format = internal_format;
	this->depthstencil_internal_format = depth_stencil_format;
	this->multisample = multisample;
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
		framebuffer->activate_draw_buffer(0);
		framebuffer->bind_draw();
		pass->on_render(i, *this, scene, camera);
	}
}
