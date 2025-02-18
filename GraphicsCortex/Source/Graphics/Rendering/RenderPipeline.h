#pragma once
#include <vector>
#include <memory>

#include "Framebuffer.h"
#include "Components/Scene.h"

class RenderPipeline;
class RenderPass {
public:
	RenderPass(const RenderPass& other) = delete;
	RenderPass() = default;
	~RenderPass() = default;

	// gets called after RenderPipeline is initalized, RenderPasses should prepare their constant values here and they may reject the RenderPass configuration
	virtual void on_initialize(int pass_index, RenderPipeline& pipeline, Scene& scene) = 0;
	
	// implementation of the pass
	virtual void on_render(int pass_index, RenderPipeline& pipeline, Scene& scene) = 0;
};

class RenderPipeline {
public:

	RenderPipeline(const RenderPipeline& other) = delete;
	RenderPipeline(int width, int height, TextureBase2::ColorTextureFormat internal_format, int multisample);
	~RenderPipeline() = default;

	void push_render_pass(std::shared_ptr<RenderPass> renderpass);
	void pop_render_pass(std::shared_ptr<RenderPass> renderpass);
	void render(Scene& scene);

private:
	friend RenderPass;
	std::shared_ptr<Framebuffer> framebuffer = std::make_shared<Framebuffer>();
	int width;
	int height;
	TextureBase2::ColorTextureFormat internal_format;
	int multisample;

	std::vector<std::shared_ptr<RenderPass>> passes;
	std::vector<std::shared_ptr<TextureBase2>> texture_stack;
};