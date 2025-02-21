#pragma once
#include <vector>
#include <memory>

#include "Framebuffer.h"
#include "Component/Scene.h"
#include "Camera.h"

class RenderPipeline;

class RenderPass {
public:
	RenderPass(const RenderPass& other) = delete;
	RenderPass() = default;
	~RenderPass() = default;

	// gets called after RenderPipeline is initalized, RenderPasses should prepare their constant values here and they may reject the RenderPass configuration
	virtual void on_initialize(int pass_index, RenderPipeline& pipeline, Scene& scene) = 0;

	// implementation of the pass
	virtual void on_render(int pass_index, RenderPipeline& pipeline, Scene& scene, Camera& camera) = 0;
};

class RenderPipeline {
public:


	RenderPipeline(const RenderPipeline& other) = delete;
	RenderPipeline(int width, int height, Texture2D::ColorTextureFormat internal_format, int multisample);
	~RenderPipeline() = default;

	void push_render_pass(std::shared_ptr<RenderPass> renderpass);
	void render(Scene& scene, Camera& camera);

//protected: 
	std::vector<std::shared_ptr<RenderPass>> passes;

	std::shared_ptr<Texture2D> texture = nullptr;
	std::shared_ptr<Framebuffer> framebuffer = std::make_shared<Framebuffer>();
	int width;
	int height;
	TextureBase2::ColorTextureFormat internal_format;
	int multisample;

	std::vector<std::shared_ptr<Texture1D>>			stack_1d;
	std::vector<std::shared_ptr<Texture2D>>			stack_2d;
	std::vector<std::shared_ptr<Texture2DArray>>	stack_2d_array;
	std::vector<std::shared_ptr<Texture3D>>			stack_3d;
	std::vector<std::shared_ptr<TextureCubeMap>>	stack_cubemap;

	bool initialized = false;
};