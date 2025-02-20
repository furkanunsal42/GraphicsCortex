#pragma once
#include "RenderPipeline.h"
#include "Components/Component_MeshRenderer.h"
#include "PrimitiveRenderer.h"

class RenderPass_Forward : public RenderPass {

	RenderPass_Forward() = default;

	void on_initialize(int pass_index, RenderPipeline& pipeline, Scene& scene) {

	}

	void on_render(int pass_index, RenderPipeline& pipeline, Scene& scene, Camera& camera) {

		std::span<MeshRendererComponent*> mesh_renderers = scene.get_components<MeshRendererComponent>();

		primitive_renderer::render();
	}
};
