#include "RenderPipeline.h"
#include "ShaderCompiler.h"
#include "Scene.h"
#include "Texture2D.h"
#include "Default_Assets.h"
#include "functional"

// pops 4 Textures (depth_stencil, normal, texcoord, position)
// pushes 2 Textures (depth_stencil, render)
class RenderPass_Deferred : public RenderPass {
public:
	bool is_initialized = false;
	
	std::shared_ptr<Program> deferred_program;
	std::shared_ptr<Graphic> screen;

	std::shared_ptr<Renderbuffer> render_texture;
	std::shared_ptr<Renderbuffer> depth_stencil_texture;
	std::shared_ptr<Framebuffer> deferred;

	void init(int pass_index, RenderPipeline& pipeline, Scene& scene) {
		deferred_program = std::make_shared<Program>(Shader(
			"../GraphicsCortex/Source/GLSL/PostProcessing/screen.vert",
			"../GraphicsCortex/Source/GLSL/PostProcessing/deferred.frag"));

		screen = default_geometry::rectangle();

		render_texture = std::make_shared<Renderbuffer>(pipeline.width, pipeline.height, pipeline.internal_format, 0);
		depth_stencil_texture = std::make_shared<Renderbuffer>(pipeline.width, pipeline.height, TextureBase2::DepthStencilTextureFormat::DEPTH24_STENCIL8, 0);

		deferred = std::make_shared<Framebuffer>();
		deferred->attach_color(0, render_texture);
		deferred->attach_depth_stencil(depth_stencil_texture);

		is_initialized = true;
	}

	void on_render(int pass_index, RenderPipeline& pipeline, Scene& scene, const std::vector<std::reference_wrapper<Graphic>>& graphics) {
		if (!is_initialized) init(pass_index, pipeline, scene);

		std::shared_ptr<TextureBase2> position_texture = pipeline.texture_stack.back();
		pipeline.texture_stack.pop_back();
		std::shared_ptr<TextureBase2> texcoord_texture = pipeline.texture_stack.back();
		pipeline.texture_stack.pop_back();
		std::shared_ptr<TextureBase2> normal_texture = pipeline.texture_stack.back();
		pipeline.texture_stack.pop_back();
		std::shared_ptr<TextureBase2> depth_stencil_texture = pipeline.texture_stack.back();
		pipeline.texture_stack.pop_back();

		std::function<void(std::shared_ptr<Program>, const std::string&, std::shared_ptr<TextureBase2>)> set_texture = [](std::shared_ptr<Program> program, const std::string& uniform_name, std::shared_ptr<TextureBase2> texture) {
			if (std::dynamic_pointer_cast<Texture2D, TextureBase2>(texture))
				program->update_uniform(uniform_name, *std::dynamic_pointer_cast<Texture2D, TextureBase2>(texture));
			else 
				ASSERT(false);
			};

		set_texture(deferred_program, "position_texture", position_texture);
		set_texture(deferred_program, "texcoord_texture", texcoord_texture);
		set_texture(deferred_program, "normal_texture", normal_texture);

		set_texture(deferred_program, "albedo_texture", texcoord_texture);

		deferred->deactivate_all_draw_buffers();
		deferred->activate_draw_buffer(0);
		deferred->bind_draw();
		Framebuffer::clear_bound_drawbuffer();

		deferred_program->bind();
		
		deferred_program->update_uniform("model", glm::translate(glm::scale(glm::identity<glm::mat4>(), glm::vec3(pipeline.width, pipeline.height, 1.0f)), glm::vec3(0.5f, 0.5f, 0.0f)));
		deferred_program->update_uniform("projection", glm::ortho(0.0f, (float)pipeline.width, 0.0f, (float)pipeline.height, -1.0f, 1.0f));
		screen->draw_without_bind();

		pipeline.texture_stack.push_back(depth_stencil_texture);
		pipeline.texture_stack.push_back(render_texture);
	}
};