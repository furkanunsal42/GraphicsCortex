#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class EquirectangularProjectionTest : public TestBench {
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

		Image image_hdr("../GraphicsCortex/Images/HDR/sunflowers_puresky_4k.hdr", 4, true);
		std::shared_ptr<Texture2D> texture_hdr = std::make_shared<Texture2D>(image_hdr.get_width(), image_hdr.get_height(), Texture2D::ColorTextureFormat::RGB16F, 1, 0, 0);
		texture_hdr->load_data(image_hdr, Texture2D::ColorFormat::RGBA, Texture2D::Type::FLOAT, 0);
		image_hdr.release();

		std::shared_ptr<Entity> sky = std::make_shared<Entity>();
		sky->add_component<SkylightComponent>();
		sky->get_component<SkylightComponent>()->set_sky_texture(*texture_hdr, 2048, TextureCubeMap::ColorTextureFormat::RGB16F);
		scene.add_entity(sky);

		texture_hdr->release();

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