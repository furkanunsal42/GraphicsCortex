#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class CompleteRenderingTest : public TestBench {
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
		pipeline.push_render_pass(std::make_shared<RenderPass_Shadowmaps>());
		pipeline.push_render_pass(std::make_shared<RenderPass_Forward>());
		pipeline.push_render_pass(std::make_shared<RenderPass_Skybox>());

		//Asset asset("../GraphicsCortex/Models/sibenik/sibenik.obj");
		//Asset asset("../GraphicsCortex/Models/sanmiguel/san-miguel.obj");
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
		//std::shared_ptr<Mesh> mesh_curtains = std::make_shared<Mesh>(asset_curtains.load_mesh());
		//std::shared_ptr<MeshMaterial> mesh_curtains_material = std::make_shared<MeshMaterial>(asset_curtains.load_mesh_material());
		std::shared_ptr<MeshMaterial> mesh_material = std::make_shared<MeshMaterial>(asset.load_mesh_material());
		std::shared_ptr<Program> program = default_program::surface_program_s();

		Image image_hdr("../GraphicsCortex/Images/HDR/sunflowers_puresky_4k.hdr", 4, true);
		//Image image_hdr("../GraphicsCortex/Images/HDR/warm_bar_4k.hdr", 4, true);
		//Image image_hdr("../GraphicsCortex/Images/HDR/qwantani_night_4k.hdr", 4, true);
		std::shared_ptr<Texture2D> texture_hdr = std::make_shared<Texture2D>(image_hdr.get_width(), image_hdr.get_height(), Texture2D::ColorTextureFormat::RGB16F, 1, 0, 0);
		texture_hdr->load_data(image_hdr, Texture2D::ColorFormat::RGBA, Texture2D::Type::FLOAT, 0);
		image_hdr.release();

		std::shared_ptr<Entity> sky = std::make_shared<Entity>();
		sky->add_component<SkylightComponent>();
		sky->get_component<SkylightComponent>()->set_sky_texture(*texture_hdr, 2048, TextureCubeMap::ColorTextureFormat::RGBA32F);
		scene.add_entity(sky);

		texture_hdr->release();
		
		std::shared_ptr<Entity> sponza = std::make_shared<Entity>();
		sponza->add_component<MeshComponent>(mesh);
		sponza->add_component<MaterialComponent>(program);
		sponza->add_component<MeshRendererComponent>();
		sponza->add_component<TransformComponent>();
		sponza->get_component<TransformComponent>()->set_scale(glm::vec3(1));
		sponza->get_component<MaterialComponent>()->set_mesh_material(mesh_material);
		scene.add_entity(sponza);

		//std::shared_ptr<Entity> curtains = std::make_shared<Entity>();
		//curtains->add_component<MeshComponent>(mesh_curtains);
		//curtains->add_component<MaterialComponent>(program);
		//curtains->add_component<MeshRendererComponent>();
		//curtains->add_component<TransformComponent>();
		//curtains->get_component<TransformComponent>()->set_scale(glm::vec3(1));
		//curtains->get_component<MaterialComponent>()->set_mesh_material(mesh_curtains_material);
		//scene.add_entity(curtains);

		//std::shared_ptr<Entity> entity_p_light = std::make_shared<Entity>();
		//entity_p_light->add_component<TransformComponent>();
		//entity_p_light->add_component<LightComponent>(LightComponent::point, glm::vec3(1, 0, 0)*30.0f);
		//entity_p_light->get_component<TransformComponent>()->set_position(glm::vec3(0, 4, 0));
		//scene.add_entity(entity_p_light);

		std::shared_ptr<Entity> entity_d_light = std::make_shared<Entity>();
		entity_d_light->add_component<TransformComponent>();
		entity_d_light->add_component<LightComponent>(LightComponent::directional, glm::vec3(1) * 30.0f);
		entity_d_light->get_component<TransformComponent>()->set_z_direction(glm::vec3(-0.5, -1, -0.5));
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
		//glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glDepthMask(GL_TRUE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

		Framebuffer fb;

		while (true) {

			double deltatime = default_window->handle_events(true);
			camera.handle_movements((GLFWwindow*)default_window->get_handle(), deltatime);

			pipeline.render(scene, camera);
			
			pipeline.framebuffer->blit_to_screen(
				0, 0, resolution.x, resolution.y,
				0, 0, resolution.x, resolution.y,
				Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR
			);

			default_window->swap_buffers();
			//glFinish();
		}
	}

};