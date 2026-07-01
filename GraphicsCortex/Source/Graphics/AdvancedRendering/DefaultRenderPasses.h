#pragma once

#include "UnifiedRenderer.h"
#include "PrimitiveRenderer.h"

extern std::filesystem::path advanced_renderpasses_shader_parent_path;

class UnifiedRendererPassBase {
public:

	UnifiedRendererPassBase() {
		render_pass_id = UnifiedRenderer::get().create_render_pass();
	}

	UnifiedRenderer::RenderPassInfo& render_pass() {
		return UnifiedRenderer::get().get_render_pass(render_pass_id);
	}

protected:
	
	uint32_t render_pass_id = UnifiedRenderer::invalid_id;

};

class URenderPass_Presnet {
public:
	
	std::shared_ptr<Texture2D> in_color;

	void draw_begin() {
		old_framebuffer = Framebuffer::get_current_draw();
		fb.attach_color(0, *in_color);
		fb.activate_draw_buffer(0);
		fb.bind_draw();
	}

	void draw_end() {
		fb.blit_to_screen(
			in_color->get_size(),
			primitive_renderer::get_viewport_size(),
			Framebuffer::Channel::COLOR,
			Framebuffer::Filter::LINEAR
		);
		old_framebuffer.bind_draw();
	}

private:

	Framebuffer old_framebuffer;
	Framebuffer fb;

};

class URenderPass_Clear {
public:

	std::shared_ptr<Texture2D> inout_texture;
	glm::vec4 color = glm::vec4(0);

	void render() {
		inout_texture->clear(color);
	}

};

class URenderPass_FlatColor : public UnifiedRendererPassBase {
public:

	std::shared_ptr<Texture2D>	out_color;
	
	glm::vec4 color = glm::vec4(0.6f, 0.76f, 1, 1);

	void init(glm::ivec2 resolution, Texture2D::ColorTextureFormat texture_format = Texture2D::ColorTextureFormat::RGBA16F) {
		if (!render_pass().is_shader_set()) {
			render_pass().set_shader(Shader(
				unified_renderer_shader_parent_path			/ "programmable_vertex_pulling.vert",
				advanced_renderpasses_shader_parent_path	/ "flatcolor.frag"
			));
		}

		out_color = std::make_shared<Texture2D>(resolution.x, resolution.y, texture_format, 1, 0, 0);
	}

	void render(Camera& camera) {
		out_color->clear(glm::vec4(0, 0, 0, 1));
		render_pass().get_program()->update_uniform("color", color);
		UnifiedRenderer::get().render(render_pass_id, camera);
	}
};


class URenderPass_Depth : public UnifiedRendererPassBase {
public:

	std::shared_ptr<Texture2D>	out_depth;

	void init(glm::ivec2 resolution, Texture2D::ColorTextureFormat texture_format = Texture2D::ColorTextureFormat::RGBA16F);

	void render(Camera& camera);
};

class URenderPass_CascadingShadowMap : public UnifiedRendererPassBase {
public:

	std::vector<std::shared_ptr<Texture2D>> out_shadowmap_cascade;

	void init(glm::ivec2 resolution, int32_t cascade_count = 6, Texture2D::ColorTextureFormat texture_format = Texture2D::ColorTextureFormat::RGBA16F);
	
	void render(glm::vec3 light_pos, glm::vec3 light_direction);

};

class URenderPass_Forward : public UnifiedRendererPassBase {
public:

	std::shared_ptr<Texture2D> inout_depth_stencil_texture;

	std::shared_ptr<Texture2D> out_color;

	void compile();
	void render(
		Texture2D& inout_depth_stencil_texture,
		Texture2D& out_color_texture
	);

	std::shared_ptr<Program>	forward_program;
};

class URenderPass_GBuffer : public UnifiedRendererPassBase {
public:

	std::shared_ptr<Texture2D> inout_depth_stencil_texture;

	std::shared_ptr<Texture2D> out_albedo_texture;
	std::shared_ptr<Texture2D> out_normal_texture;
	std::shared_ptr<Texture2D> out_uv_texture;
	std::shared_ptr<Texture2D> out_metallic_texture;
	std::shared_ptr<Texture2D> out_roughness_texture;

	void init(glm::ivec2 resolution);
	void compile();
	void render();

};

class URenderPass_Deferred : public UnifiedRendererPassBase {
public:

	std::shared_ptr<Texture2D> in_albedo_texture;
	std::shared_ptr<Texture2D> in_normal_texture;
	std::shared_ptr<Texture2D> in_uv_texture;
	std::shared_ptr<Texture2D> in_metallic_texture;
	std::shared_ptr<Texture2D> in_roughness_texture;
	std::shared_ptr<Texture2D> in_world_pos_texture;

	std::shared_ptr<Texture2D> out_color;

	void compile();
	void render();

};

class URenderPass_SSAO {

};

class URenderPass_SSReflection {

};

class URenderPass_SSBloom {

};

class URenderPass_SkyBox {

};

class URenderPass_SSGlobalIllumination {

};

class URenderPass_VoxelConeTracing {

};
