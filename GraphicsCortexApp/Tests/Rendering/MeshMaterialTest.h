#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class MeshMaterialTest : public TestBench {
public:

	bool run() {

		default_window_desc = new WindowDescription();
		default_window_desc->w_resolution = glm::ivec2(1920, 1080);
		default_init();

		Asset sculpture("../GraphicsCortex/Models/circuit/nogaro.obj");

		std::shared_ptr<MeshMaterial> mesh_material = std::make_shared<MeshMaterial>(sculpture.load_mesh_material());

		Scene scene;
		std::shared_ptr<Entity> entity_a = std::make_shared<Entity>();

		entity_a->add_component<MeshComponent>(std::make_shared<Mesh>(sculpture.load_mesh()));
		entity_a->add_component<MaterialComponent>(default_program::surface_program_s());
		entity_a->add_component<MeshRendererComponent>();
		entity_a->add_component<TransformComponent>();
		scene.add_entity(entity_a);

		entity_a->get_component<TransformComponent>()->transform = glm::scale(glm::mat4(1), glm::vec3(0.01))
			;
		entity_a->get_component<MaterialComponent>()->set_uniform("light_colors[0]", glm::vec3(1, 1, 1));
		entity_a->get_component<MaterialComponent>()->set_uniform("light_positions[0]", glm::vec3(0, 0.1, 0));
		entity_a->get_component<MaterialComponent>()->set_mesh_material(mesh_material);

		glm::ivec2 resolution = default_window->get_framebuffer_resolution();
		RenderPipeline pipeline(resolution.x, resolution.y, Texture2D::ColorTextureFormat::RGBA8, Texture2D::DepthStencilTextureFormat::DEPTH24_STENCIL8, 0);
		pipeline.push_render_pass(std::make_shared<RenderPass_Clear>(glm::vec4(0, 0, 0, 1)));
		pipeline.push_render_pass(std::make_shared<RenderPass_Forward>());

		Camera camera;
		camera.mouse_sensitivity = 10;
		camera.fov = 100;

		glEnable(GL_DEPTH_TEST);

		while (true) {

			double deltatime = default_window->handle_events(true);
			camera.handle_movements((GLFWwindow*)default_window->get_handle(), deltatime);
			std::cout << deltatime << std::endl;

			pipeline.render(scene, camera);

			pipeline.framebuffer->blit_to_screen(
				0, 0, resolution.x, resolution.y,
				0, 0, resolution.x, resolution.y,
				Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR
			);

			default_window->swap_buffers();
			glFinish();
		}
	}

};