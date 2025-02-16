#pragma once
#include <vector>
#include <memory>

#include "Framebuffer.h"

class Scene;
class Graphic;
class RenderPipeline;

class RenderPass {
public:
	RenderPass(const RenderPass& other) = delete;
	RenderPass() = default;
	~RenderPass() = default;

	virtual void on_render(int pass_index, RenderPipeline& pipeline, Scene& scene, const std::vector<std::reference_wrapper<Graphic>>& graphics) = 0;
};

class RenderPipeline {
public:
	
	std::shared_ptr<Framebuffer> framebuffer = std::make_shared<Framebuffer>();
	RenderPipeline(const RenderPipeline& other) = delete;
	RenderPipeline(int width, int height, TextureBase2::ColorTextureFormat internal_format, int multisample);
	~RenderPipeline() = default;

	void push_render_pass(std::shared_ptr<RenderPass> renderpass);
	void render(Scene& scene);

	std::vector<std::shared_ptr<RenderPass>> passes;
	int width;
	int height;
	TextureBase2::ColorTextureFormat internal_format;
	int multisample;

	std::vector<std::shared_ptr<TextureBase2>> texture_stack;
};
