#pragma once

#include "CortexRendererBase.h"
#include "DefaultRenderPasses.h"
#include "ECS_Systems/RenderingSystem.h"

class Renderer_FlatColor {
public:

	uint32_t flat_layer = UnifiedRenderer::invalid_id;

	RendererSyncSystem::LayerResolver layer_sorter = [this](Entity2 e, CortexScene& scene) -> uint32_t {
		return this->flat_layer;
		};

	URenderPass_FlatColor	flatcolor;
	URenderPass_Presnet		present;

	void setup() {

		if (flat_layer == UnifiedRenderer::invalid_id)
			flat_layer = UnifiedRenderer::get().create_render_layer();

		flatcolor.init(glm::ivec2(1920, 1080));
		flatcolor.render_pass().set_render_layer(flat_layer);

		present.in_color = flatcolor.out_color;

	}

	void render(Camera& camera) {
		present.draw_begin();
		flatcolor.render(camera);
		present.draw_end();
	}

};

class Renderer_Deferred : public CortexRendererBase {
public:
	
	uint32_t solid_layer		= UnifiedRenderer::invalid_id;
	uint32_t transparent_layer	= UnifiedRenderer::invalid_id;

	RendererSyncSystem::LayerResolver layer_sorter = [this](Entity2 e, CortexScene& scene) -> uint32_t {
		return scene.get<RendererComponent2>(e).is_transparent ? this->transparent_layer : this->solid_layer;
		};

	URenderPass_Presnet present;
	URenderPass_GBuffer g_buffer;

	void init(glm::ivec2 resolution) override {
		
		if (solid_layer == UnifiedRenderer::invalid_id)
			solid_layer = UnifiedRenderer::get().create_render_layer();

		if (transparent_layer == UnifiedRenderer::invalid_id)
			transparent_layer = UnifiedRenderer::get().create_render_layer();

		g_buffer.init(resolution);
		g_buffer.render_pass().set_render_layer(solid_layer);

		present.in_color = g_buffer.out_albedo_texture;
	}

	void render(Camera& camera) override {

		g_buffer.render(camera);

		present.blit();
	}


};