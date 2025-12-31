#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "Tools/ImmediateRendering/ImmediateRenderer.h"

class ImmediateRendererTest : public TestBench {
public:

	bool run() {

		default_window_desc = new WindowDescription();
		default_window_desc->w_resolution = glm::ivec2(1920, 1080);
		default_window_desc->f_swap_interval = 0;
		default_window_desc->f_multisample_count = 8;

		default_init();

		FontBank& fontbank = FontBank::get();

		ImmediateRenderer immidiate;

		Texture2D texture = Texture2D(
			256, 256,
			Texture2D::ColorTextureFormat::RGBA8,
			1, 0, 0
		);
		texture.is_bindless = true;
		//texture.clear(glm::vec4(1, 0, 0, 1), 0);
		texture.load_data(
			Image("../GraphicsCortex/Images/orange.png", 256, 256, 1, 4, 1, true),
			Texture2D::ColorFormat::RGBA,
			Texture2D::Type::UNSIGNED_BYTE,
			0
		);

		std::cout << texture.texture_handle << std::endl;

		while (!default_window->should_close()) {
			double deltatime = default_window->handle_events(true);
			primitive_renderer::clear(1, 1, 1, 1);
			
			immidiate.draw_rectangle(glm::vec2(0, 0), glm::vec2(0.1, 0.1), 0, texture.texture_handle);

			immidiate.draw_line(glm::vec3(0.3, 0.3, 0), glm::vec3(0.7, 0.7, 0));
			immidiate.draw_line(glm::vec3(0.7, 0.3, 0), glm::vec3(0.3, 0.7, 0));

			immidiate.set_fill_color(glm::vec4(1, 0, 0, 1));
			immidiate.draw_triangle(glm::vec3(0, 0, 0), glm::vec3(-0.5, 0, 0), glm::vec3(-0.5, -0.5, 0));

			immidiate.set_fill_color(glm::vec4(1, 0, 1, 1));
			immidiate.draw_quad(glm::vec3(-0.8, -0.8, 0), glm::vec3(0.8, -0.8, 0), glm::vec3(0.8, -0.6, 0), glm::vec3(-0.8, -0.6, 0));

			immidiate.render();
			default_window->swap_buffers();
		}

		return true;
	}
};