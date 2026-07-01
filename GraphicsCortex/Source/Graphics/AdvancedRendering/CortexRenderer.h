#pragma once

#include "CortexRendererBase.h"
#include "DefaultRenderPasses.h"

class CortexRenderer : public CortexRendererBase {
public:
	
	uint32_t solid_layer		= UnifiedRenderer::invalid_id;
	uint32_t transparent_layer	= UnifiedRenderer::invalid_id;

	URenderPass_Presnet present;
	URenderPass_GBuffer g_buffer;

	void init(glm::ivec2 resolution, Texture2D::ColorTextureFormat format = Texture2D::ColorTextureFormat::RGBA16F) override {
		
		if (solid_layer == UnifiedRenderer::invalid_id)
			solid_layer = UnifiedRenderer::get().create_render_layer();

		if (transparent_layer == UnifiedRenderer::invalid_id)
			transparent_layer = UnifiedRenderer::get().create_render_layer();

		g_buffer.init(resolution);
		g_buffer.render_pass().set_render_layer(solid_layer);

		present.in_color = g_buffer.out_albedo_texture;
	}

	void render() override {
		present.draw_begin();

		g_buffer.render();

		present.draw_end();
	}

};