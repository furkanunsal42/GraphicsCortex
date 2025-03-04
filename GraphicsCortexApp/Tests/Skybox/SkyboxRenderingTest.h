#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class SkyboxRenderingTest : public TestBench {
public:

	bool run() {

		default_window_desc = new WindowDescription();
		default_window_desc->w_resolution = glm::ivec2(1920, 1080);
		default_window_desc->f_swap_interval = 0;

		default_init();

		Scene scene;

		glm::ivec2 resolution = default_window->get_framebuffer_resolution();
		RenderPipeline pipeline(resolution.x, resolution.y, Texture2D::ColorTextureFormat::RGBA8, Texture2D::DepthStencilTextureFormat::DEPTH24_STENCIL8, 4);
		pipeline.push_render_pass(std::make_shared<RenderPass_Clear>(glm::vec4(0, 0, 0, 1)));
		//pipeline.push_render_pass(std::make_shared<RenderPass_Forward>());
		pipeline.push_render_pass(std::make_shared<RenderPass_Skybox>());

		//Image image_hdr("../GraphicsCortex/Images/HDR/sunflowers_puresky_4k.hdr", 4, true);
		//std::shared_ptr<Texture2D> texture_hdr = std::make_shared<Texture2D>(image_hdr.get_width(), image_hdr.get_height(), Texture2D::ColorTextureFormat::RGBA16F, 1, 0, 0);
		//texture_hdr->load_data(image_hdr, Texture2D::ColorFormat::RGBA, Texture2D::Type::FLOAT, 0);

		Image px("../GraphicsCortex/Images/CubeMap/Sky/px.jpg", 3, false);
		Image py("../GraphicsCortex/Images/CubeMap/Sky/py.jpg", 3, false);
		Image pz("../GraphicsCortex/Images/CubeMap/Sky/pz.jpg", 3, false);
		Image nx("../GraphicsCortex/Images/CubeMap/Sky/nx.jpg", 3, false);
		Image ny("../GraphicsCortex/Images/CubeMap/Sky/ny.jpg", 3, false);
		Image nz("../GraphicsCortex/Images/CubeMap/Sky/nz.jpg", 3, false);
		std::shared_ptr<TextureCubeMap> cubemap = std::make_shared<TextureCubeMap>(px.get_width(), TextureCubeMap::ColorTextureFormat::RGB8, 1, 0);
		cubemap->load_data(TextureCubeMap::RIGHT, px, TextureCubeMap::ColorFormat::RGB, TextureCubeMap::Type::UNSIGNED_BYTE, 0);
		cubemap->load_data(TextureCubeMap::UP, py, TextureCubeMap::ColorFormat::RGB, TextureCubeMap::Type::UNSIGNED_BYTE, 0);
		cubemap->load_data(TextureCubeMap::FRONT, pz, TextureCubeMap::ColorFormat::RGB, TextureCubeMap::Type::UNSIGNED_BYTE, 0);
		cubemap->load_data(TextureCubeMap::LEFT, nx, TextureCubeMap::ColorFormat::RGB, TextureCubeMap::Type::UNSIGNED_BYTE, 0);
		cubemap->load_data(TextureCubeMap::DOWN, ny, TextureCubeMap::ColorFormat::RGB, TextureCubeMap::Type::UNSIGNED_BYTE, 0);
		cubemap->load_data(TextureCubeMap::BACK, nz, TextureCubeMap::ColorFormat::RGB, TextureCubeMap::Type::UNSIGNED_BYTE, 0);

		std::shared_ptr<Entity> sky = std::make_shared<Entity>();
		sky->add_component<SkylightComponent>();
		sky->get_component<SkylightComponent>()->set_sky_texture(cubemap);
		scene.add_entity(sky);

		Camera camera;
		camera.fov = 100;
		camera.max_distance = 1000.0f;
		camera.screen_width = resolution.x;
		camera.screen_height = resolution.y;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

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