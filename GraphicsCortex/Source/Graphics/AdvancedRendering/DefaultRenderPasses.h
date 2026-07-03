#pragma once

#include "UnifiedRenderer.h"
#include "CortexRendererBase.h"
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

	void blit() {
		fb.attach_color(0, *in_color);
		fb.blit_to_screen(
			in_color->get_size(),
			primitive_renderer::get_viewport_size(),
			Framebuffer::Channel::COLOR,
			Framebuffer::Filter::LINEAR
		);
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
	Texture2D::ColorTextureFormat out_color_format = Texture2D::ColorTextureFormat::RGBA16F;

	std::shared_ptr<Texture2D> out_color;
	
	glm::vec4 color = glm::vec4(0.6f, 0.76f, 1, 1);

	void init(glm::ivec2 resolution) {
		if (!render_pass().is_shader_set()) {
			render_pass().set_shader(Shader(
				unified_renderer_shader_parent_path			/ "programmable_vertex_pulling.vert",
				advanced_renderpasses_shader_parent_path	/ "flatcolor.frag"
			));
		}
		
		RenderParameters param(true);
		render_pass().set_parameters(param);

		out_color = std::make_shared<Texture2D>(resolution.x, resolution.y, out_color_format, 1, 0, 0);
	}

	void render(Camera& camera) {
		out_color->clear(glm::vec4(0, 0, 0, 1));
		render_pass().get_program()->update_uniform("color", color);
		UnifiedRenderer::get().render(render_pass_id, camera);
	}
};

class URenderPass_GBuffer : public UnifiedRendererPassBase {
public:

	Texture2D::DepthStencilTextureFormat inout_depth_stencil_format = Texture2D::DepthStencilTextureFormat::DEPTH24_STENCIL8;

	std::shared_ptr<Texture2D> inout_depth_stencil_texture;

	std::shared_ptr<Texture2D> out_albedo_texture;
	std::shared_ptr<Texture2D> out_normal_texture;
	std::shared_ptr<Texture2D> out_ao_roughness_metallic_texture;
	std::shared_ptr<Texture2D> out_emissive_texture;

	void init(glm::ivec2 resolution) {

		if (!render_pass().is_shader_set()) {
			render_pass().set_shader(Shader(
				unified_renderer_shader_parent_path			/ "programmable_vertex_pulling.vert",
				advanced_renderpasses_shader_parent_path	/ "g_buffer.frag"
			));

			render_pass().require_attribute(CortexRendererBase::attrib_normal,	UnifiedRenderer::GenericType::vec3_t, "v_normal", false);
			render_pass().require_attribute(CortexRendererBase::attrib_uv0,		UnifiedRenderer::GenericType::vec2_t, "v_uv0", false);
		}

		RenderParameters param(true);
		param.blend = false;
		render_pass().set_parameters(param);

		inout_depth_stencil_texture = std::make_shared<Texture2D>(
			resolution.x,
			resolution.y,
			inout_depth_stencil_format,
			1, 0, 0);

		out_albedo_texture = std::make_shared<Texture2D>(
			resolution.x,
			resolution.y,
			Texture2D::ColorTextureFormat::RGB8,
			1, 0, 0);

		out_ao_roughness_metallic_texture = std::make_shared<Texture2D>(
			resolution.x,
			resolution.y,
			Texture2D::ColorTextureFormat::RGB8,
			1, 0, 0);

		out_normal_texture = std::make_shared<Texture2D>(
			resolution.x,
			resolution.y,
			Texture2D::ColorTextureFormat::RGB8,
			1, 0, 0);

		out_emissive_texture = std::make_shared<Texture2D>(
			resolution.x,
			resolution.y,
			Texture2D::ColorTextureFormat::RGB8,
			1, 0, 0);
	}
	
	void render(Camera& camera) {

		old_framebuffer = Framebuffer::get_current_draw();
		
		fb.attach_color(0, *out_albedo_texture);
		fb.attach_color(1, *out_ao_roughness_metallic_texture);
		fb.attach_color(2, *out_normal_texture);
		fb.attach_color(3, *out_emissive_texture);
		fb.attach_depth_stencil(*inout_depth_stencil_texture);

		fb.activate_draw_buffer(0);
		fb.activate_draw_buffer(1);
		fb.activate_draw_buffer(2);
		fb.activate_draw_buffer(3);

		fb.bind_draw();

		primitive_renderer::clear(0, 0, 0, 1);
		UnifiedRenderer::get().render(render_pass_id, camera);

		old_framebuffer.bind_draw();
	}

private:

	Framebuffer fb;
	Framebuffer	old_framebuffer;
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
