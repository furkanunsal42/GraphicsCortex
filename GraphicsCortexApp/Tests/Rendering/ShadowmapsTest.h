#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class ShadowmapsTest : public TestBench {
public:

	bool run() {

		default_window_desc = new WindowDescription();
		default_window_desc->w_resolution = glm::ivec2(1920, 1080);
		default_window_desc->f_swap_interval = 0;

		default_init();

		Scene scene;
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		glm::ivec2 resolution = default_window->get_framebuffer_resolution();
		RenderPipeline pipeline(resolution.x, resolution.y, Texture2D::ColorTextureFormat::RGBA8, Texture2D::DepthStencilTextureFormat::DEPTH24_STENCIL8, 4);
		pipeline.push_render_pass(std::make_shared<RenderPass_Clear>(glm::vec4(0, 0, 0, 1)));
		auto shadowmap_renderpass = std::make_shared<RenderPass_Shadowmaps>();
		pipeline.push_render_pass(shadowmap_renderpass);
		pipeline.push_render_pass(std::make_shared<RenderPass_Forward>());

		//Asset asset("../GraphicsCortex/Models/sculpture/scene.gltf");
		//Asset asset("../GraphicsCortex/Models/City/edited_city.obj");
		//Asset asset("../GraphicsCortex/Models/teducar/source/teduCar.fbx");
		//Asset asset("../GraphicsCortex/Models/Thinker/source/Rodin_Thinker.obj");
		//Asset asset("../GraphicsCortex/Models/medival/source/medival.fbx");
		//Asset asset("../GraphicsCortex/Models/bmw/scene.gltf");
		//Asset asset("../GraphicsCortex/Models/Sponza/scene.gltf");
		//Asset asset_curtains("../GraphicsCortex/Models/Sponza/NewSponza_Curtains_glTF.gltf");
		//Asset asset("../GraphicsCortex/Models/circuit/nogaro.obj");
		Asset asset("../GraphicsCortex/ModelsKhronos/2.0/Sponza/glTF/Sponza.gltf");
		//Asset asset("../GraphicsCortex/Models/gun/Cerberus_LP.FBX");

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(asset.load_mesh());
		std::shared_ptr<MeshMaterial> mesh_material = std::make_shared<MeshMaterial>(asset.load_mesh_material());
		std::shared_ptr<Program> program = default_program::surface_program_s();

		std::shared_ptr<Entity> sponza = std::make_shared<Entity>();
		sponza->add_component<MeshComponent>(mesh);
		sponza->add_component<MaterialComponent>(program);
		sponza->add_component<MeshRendererComponent>();
		sponza->add_component<TransformComponent>();
		sponza->get_component<TransformComponent>()->set_scale(glm::vec3(1));
		sponza->get_component<MaterialComponent>()->set_mesh_material(mesh_material);
		scene.add_entity(sponza);

		//std::shared_ptr<Entity> entity_p_light = std::make_shared<Entity>();
		//entity_p_light->add_component<TransformComponent>();
		//entity_p_light->add_component<LightComponent>(LightComponent::point, glm::vec3(1, 0, 0)*30.0f);
		//entity_p_light->get_component<TransformComponent>()->set_position(glm::vec3(0, 4, 0));
		//scene.add_entity(entity_p_light);

		std::shared_ptr<Entity> entity_d_light = std::make_shared<Entity>();
		entity_d_light->add_component<TransformComponent>();
		entity_d_light->add_component<LightComponent>(LightComponent::directional, glm::vec3(1) * 10.0f);
		entity_d_light->get_component<TransformComponent>()->set_z_direction(glm::vec3(0.1, -1, 0.1));
		scene.add_entity(entity_d_light);

		//std::shared_ptr<Entity> entity_s_light = std::make_shared<Entity>();
		//entity_s_light->add_component<TransformComponent>();
		//entity_s_light->add_component<LightComponent>(LightComponent::spot, glm::vec3(1, 1, 1) * 60.0f, 3.14/4);
		//entity_s_light->get_component<TransformComponent>()->set_position(glm::vec3(0, 4, 0));
		//entity_s_light->get_component<TransformComponent>()->set_z_direction(glm::vec3(0, 0, -1));
		//scene.add_entity(entity_s_light);

		Camera camera;
		camera.fov = 100;
		camera.max_distance = 200.0f;
		camera.screen_width = resolution.x;
		camera.screen_height = resolution.y;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glDepthMask(GL_TRUE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

		Framebuffer fb;

		bool view_mode = false;
		int32_t cascade = 5;
		default_window->newsletters->on_key_events.subscribe([&](const Window::KeyPressResult& key_result) {
			if (key_result.key == Window::Key::P && key_result.action == Window::PressAction::RELEASE)
				view_mode = !view_mode;
			if (key_result.key == Window::Key::UP && key_result.action == Window::PressAction::RELEASE)
				cascade++;
			if (key_result.key == Window::Key::DOWN && key_result.action == Window::PressAction::RELEASE)
				cascade--;
			cascade = std::max(std::min(cascade, 5), 0);
			});

		while (true) {

			double deltatime = default_window->handle_events(true);
			camera.handle_movements((GLFWwindow*)default_window->get_handle(), deltatime);

			pipeline.render(scene, camera);

			if (view_mode) {
				//shadowmap_renderpass->framebuffer->attach_color(0, *shadowmap_renderpass->d_shadowmaps_color, cascade, 0);
				//shadowmap_renderpass->framebuffer->activate_draw_buffer(0);
				//shadowmap_renderpass->framebuffer->blit_to_screen(
				//	0, 0, 1024, 1024,
				//	0, 0, resolution.x, resolution.y,
				//	Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR
				//);
			}
			else {
				pipeline.framebuffer->activate_draw_buffer(0);
				pipeline.framebuffer->blit_to_screen(
					0, 0, resolution.x, resolution.y,
					0, 0, resolution.x, resolution.y,
					Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR
				);
			}

			default_window->swap_buffers();
			//glFinish();
		}
	}

};