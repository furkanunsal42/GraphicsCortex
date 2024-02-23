#include "RenderPipeline.h"
#include "ShaderCompiler.h"
#include "Scene.h"
#include "Texture2D.h"

// pops 0 Textures;
// pushes 8 Textures (depth_stencil, normal, texcoord, position, emmisive_texture, brdf_texture, metalic_roughness_ao_texture, albedo_texture)
class RenderPass_GBuffer : public RenderPass {
public:
	bool is_initialized = false;
	std::shared_ptr<Program> g_buffer_program;

	std::shared_ptr<Texture2D> albedo_texture;
	std::shared_ptr<Texture2D> metalic_roughness_ao_texture;
	std::shared_ptr<Texture2D> brdf_texture;
	std::shared_ptr<Texture2D> emmisive_texture;
	std::shared_ptr<Texture2D> position_texture;
	std::shared_ptr<Texture2D> texcoord_texture;
	std::shared_ptr<Texture2D> normal_texture;

	std::shared_ptr<Renderbuffer> depth_stencil_texture;

	std::shared_ptr<Framebuffer> g_buffer;

	void init(int pass_index, RenderPipeline& pipeline, Scene& scene) {
		g_buffer_program = std::make_shared<Program>(Shader(
			"../GraphicsCortex/Source/GLSL/Surface/surface.vert",
			"../GraphicsCortex/Source/GLSL/Surface/surface.geom",
			"../GraphicsCortex/Source/GLSL/Surface/gbuffer.frag"));

		albedo_texture					= std::make_shared<Texture2D>(pipeline.width, pipeline.height, pipeline.internal_format, 1);
		metalic_roughness_ao_texture	= std::make_shared<Texture2D>(pipeline.width, pipeline.height, pipeline.internal_format, 1);
		brdf_texture					= std::make_shared<Texture2D>(pipeline.width, pipeline.height, pipeline.internal_format, 1);
		emmisive_texture				= std::make_shared<Texture2D>(pipeline.width, pipeline.height, pipeline.internal_format, 1);
		position_texture				= std::make_shared<Texture2D>(pipeline.width, pipeline.height, pipeline.internal_format, 1);
		texcoord_texture				= std::make_shared<Texture2D>(pipeline.width, pipeline.height, pipeline.internal_format, 1);
		normal_texture					= std::make_shared<Texture2D>(pipeline.width, pipeline.height, pipeline.internal_format, 1);

		depth_stencil_texture	= std::make_shared<Renderbuffer>(pipeline.width, pipeline.height, Renderbuffer::DepthStencilTextureFormat::DEPTH24_STENCIL8, 0);

		g_buffer = std::make_shared<Framebuffer>();
		g_buffer->attach_color(0, position_texture);
		g_buffer->attach_color(1, texcoord_texture);
		g_buffer->attach_color(2, normal_texture);
		g_buffer->attach_color(3, albedo_texture);
		g_buffer->attach_color(4, metalic_roughness_ao_texture);
		g_buffer->attach_color(5, brdf_texture);
		g_buffer->attach_color(6, emmisive_texture);

		g_buffer->attach_depth_stencil(depth_stencil_texture);

		is_initialized = true;
	}

	void on_render(int pass_index, RenderPipeline& pipeline, Scene& scene, const std::vector<std::reference_wrapper<Graphic>>& graphics) {
		if (!is_initialized) init(pass_index, pipeline, scene);

		g_buffer->deactivate_all_draw_buffers();
		g_buffer->activate_draw_buffer(0);
		g_buffer->activate_draw_buffer(1);
		g_buffer->activate_draw_buffer(2);
		g_buffer->activate_draw_buffer(3);
		g_buffer->activate_draw_buffer(4);
		g_buffer->activate_draw_buffer(5);
		g_buffer->activate_draw_buffer(6);
		g_buffer->bind_draw();
		Framebuffer::clear_bound_drawbuffer(0.5, 0.5, 0.5, 1);

		g_buffer_program->bind();

		scene.camera->update_default_uniforms(*g_buffer_program);
		for (int i = 0; i < graphics.size(); i++) {
			Graphic& graphic = graphics[i];
			graphic.update_matrix();
			graphic.update_default_uniforms(*g_buffer_program);
			graphic.material->update_uniforms();
			g_buffer_program->attach_uniform_buffer(graphic.material->_pbr_material_buffer_name, graphic.material->_pbr_texture_uniform_buffer);
			g_buffer_program->update_uniform_buffer_slots();
			graphic.draw_without_bind();
			g_buffer_program->deattach_uniform_buffer(graphic.material->_pbr_material_buffer_name);

		}

		pipeline.texture_stack.push_back(depth_stencil_texture);
		pipeline.texture_stack.push_back(normal_texture);
		pipeline.texture_stack.push_back(texcoord_texture);
		pipeline.texture_stack.push_back(position_texture);
		pipeline.texture_stack.push_back(emmisive_texture);
		pipeline.texture_stack.push_back(brdf_texture);
		pipeline.texture_stack.push_back(metalic_roughness_ao_texture);
		pipeline.texture_stack.push_back(albedo_texture);
	}
};