#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "GUI/CortexGUI.h"

class CortexGUITest: public TestBench {
public:

	bool run() {

		default_window_desc = new WindowDescription();
		default_window_desc->w_resolution = glm::ivec2(1920, 1080);
		default_window_desc->f_multisample_count = 16;
		default_window_desc->f_swap_interval = 0;

		default_init();

		font_id font = FontBank::get().load_font("../GraphicsCortex/Fonts/Roboto-Regular.ttf", 64);
		//Text text(U"Hello There");
		//text.set_font(font);
		//text.set_scale(1.0f);
		//Program text_program = default_program::text_program();
		//
		//float ratio = (float)default_window->get_framebuffer_resolution().y / default_window->get_framebuffer_resolution().x;
		//text_program.update_uniform("atlas", *FontBank::get().get_font(font).atlas);
		//text_program.update_uniform("model", glm::identity<glm::mat4>());
		//text_program.update_uniform("view", glm::identity<glm::mat4>());
		//text_program.update_uniform("projection", glm::ortho(-0.5f, 0.5f, -ratio / 2, ratio / 2));
		//text_program.update_uniform("color", glm::vec4(1, 0, 0, 1));
		

		Widget widget = GUI::get().create_widget();
		widget.style().color = glm::vec4(1, 0, 1, 1);
		widget.size() = glm::vec2(1920, 1080) / 2.0f;
		widget.position() = glm::vec2(200, 100);
		widget.z() = 0;

		Widget child = widget.create_child();
		child.style().color = glm::vec4(0, 0, 1, 1);
		child.position() = glm::vec2(100, 100);
		child.size() = glm::vec2(1920, 1080) / 2.0f;
		child.z() = 1;

		child.style().texture = FontBank::get().get_font(font).atlas;
		child.size() = child.style().texture->get_size();

		while (true) {
			double deltatime = default_window->handle_events(true);
			primitive_renderer::clear(1, 1, 1, 1);

			//text_program.update_uniform("model", glm::identity<glm::mat4>());
			//camera.update_default_uniforms(text_program);

			//text_program.update_uniform("view", glm::identity<glm::mat4>());
			//text_program.update_uniform("projection", glm::ortho(-0.5f, 0.5f, -ratio / 2, ratio / 2));


			//primitive_renderer::render(text_program, text, RenderParameters(true));

			GUI::get().render(widget);

			default_window->swap_buffers();
		}
	}
};