#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "GUI/CortexGUI.h"
#include "Tools/GraphicsOperation/GraphicsOperation.h"

#include "CortexGUIControls/Basic/GUIControl_Rectangle.h"

class CortexGUITest: public TestBench {
public:

	bool run() {

		default_window_desc = new WindowDescription();
		default_window_desc->w_resolution = glm::ivec2(1920, 1080);
		default_window_desc->f_multisample_count = 16;
		default_window_desc->f_swap_interval = 0;

		default_init();

		//font_id font = FontBank::get().load_font("../GraphicsCortex/Fonts/Roboto-Regular.ttf", 64);
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
		widget.color() = glm::vec4(1, 0, 1, 1);
		widget.size() = glm::vec2(1920, 1080) / 2.0f;
		widget.position() = glm::vec2(200, 100);
		widget.z() = 0;

		Widget child = widget.create_child();
		child.color() = glm::vec4(0, 0, 1, 1);
		child.position() = glm::vec2(100, 100);
		child.size() = glm::vec2(1920, 1080) / 2.0f;
		child.z() = 1;

		Image orange("../GraphicsCortex/Images/orange.png", 4, true);
		
		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(orange.get_width(), orange.get_height(), Texture2D::ColorTextureFormat::RGBA8, 1, 0, 0);
		texture->load_data(orange, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE, 0);

		GraphicsOperation op;
		op.compute(
			*texture,
			"vec4(target.xyz, all(greaterThan(target.xyz, vec3(0.95))) ? 0 : 1)"
		);

		child.texture() = texture;
		child.size() = child.texture()->get_size() / 8;

		gui_controls::Rectangle rect;
		rect.color = glm::vec4(0, 0, 1, 1);
		rect.target_size = glm::vec2(1920, 1080) / 2.0f;
		rect.z = 1;

		while (true) {
			double deltatime = default_window->handle_events(true);
			primitive_renderer::clear(1, 1, 1, 1);

			//text_program.update_uniform("model", glm::identity<glm::mat4>());
			//camera.update_default_uniforms(text_program);

			//text_program.update_uniform("view", glm::identity<glm::mat4>());
			//text_program.update_uniform("projection", glm::ortho(-0.5f, 0.5f, -ratio / 2, ratio / 2));


			//primitive_renderer::render(text_program, text, RenderParameters(true));

			//GUI::get().render(widget);

			GUI::get().render(rect.get_widget());

			default_window->swap_buffers();
		}
	}
};