#pragma once
#include "RenderPipeline.h"
#include "Components/Component_MeshRenderer.h"

#include "Components/Component_Skylight.h"

#include "UniformBuffer.h"
#include "PrimitiveRenderer.h"
#include "Component/Entity.h"

class RenderPass_Skybox : public RenderPass {
public:

	RenderPass_Skybox() = default;

	void on_initialize(int pass_index, RenderPipeline& pipeline, Scene& scene) {

	}

	void on_render(int pass_index, RenderPipeline& pipeline, Scene& scene, Camera& camera) {

		std::span<SkylightComponent*> sky_components = scene.get_components<SkylightComponent>();
		if (sky_components.size() == 0)
			return;

		SkylightComponent* primary_skylight = nullptr;
		for (SkylightComponent* sky_component : sky_components) {
			if (!sky_component->is_primary()) continue;
			primary_skylight = sky_component;
			break;
		}

		if (primary_skylight != nullptr)
			primary_skylight->render(camera);
	}
};
