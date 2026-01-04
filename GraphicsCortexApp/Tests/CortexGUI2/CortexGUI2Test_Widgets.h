#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "GUI2/GUI2.h"
#include "GUI2/GUI2Dynamic.h"
#include "GUI2/GUI2Widgets.h"

class CortexGUITest2_Widgets : public TestBench {
public:

	bool run() {

		constexpr auto avail = GUI2Dynamic::avail;
		constexpr auto fit = GUI2Dynamic::fit;
		constexpr auto invalid_id = GUI2Dynamic::invalid_id;

		GUI2 gui;
		GUI2Dynamic gui_d;

		widget2::Window window0;
		window0.draggable = true;

		widget2::Box style_box;
		style_box.target_size = glm::vec2(avail, 32);
		style_box.color = glm::vec4(1, 1, 1, 1);
		style_box.on_hover_color = glm::vec4(1, 0, 1, 1);
		style_box.on_hover_color_transition = std::chrono::milliseconds(100);
		style_box.on_hold_color = glm::vec4(0, 1, 0, 1);
		style_box.on_hold_color_transition = std::chrono::milliseconds(200);
		style_box.on_hold_margin = glm::vec4(20, 0, 20, 0);
		style_box.on_hold_margin_transition = std::chrono::milliseconds(85);

		widget2::Image image;
		image.target_size = glm::vec2(avail, 64);
		image.type = widget2::Image::Fit;
		image.on_hover_color = glm::vec4(1, 0, 0, 1);
		image.on_hover_color_transition = std::chrono::milliseconds(1);

		std::shared_ptr<Texture2D> texture = nullptr;

		widget2::Label label;
		label.text = U"Hello World";
		label.text_color = glm::vec4(0, 0, 0, 1);
		label.on_hover_text_color = glm::vec4(1, 0, 0, 1);
		label.on_hover_text_color_transition = std::chrono::milliseconds(100);

		widget2::TextArea text_area;
		text_area.text = U"HERE";

		while (true) {

			gui_d.new_frame(gui);

			window0.publish(gui_d);
			
			//gui_d.stack_begin()
			//	.set_target_size(glm::vec2(600, fit));

			//style_box.publish(gui_d);
			
			//image.publish(gui_d);
			
			//label.publish(gui_d);

			text_area.publish(gui_d);

			//gui_d.stack_end();
			gui_d.window_end();
			
			gui_d.publish(gui);
			gui.render();

			static bool once = true;
			if (once) {
				gui_d.print_layout();
				FontBank::get().load_font("../GraphicsCortex/Fonts/Roboto-Regular.ttf", 64);
				once = false;
			}

			if (texture == nullptr) {
				texture = std::make_shared<Texture2D>(
					256, 256,
					Texture2D::ColorTextureFormat::RGBA8,
					1, 0, 0
				);
				texture->is_bindless = true;
				//texture.clear(glm::vec4(1, 0, 0, 1), 0);
				texture->load_data(
					Image("../GraphicsCortex/Images/orange.png", 256, 256, 1, 4, 1, true),
					Texture2D::ColorFormat::RGBA,
					Texture2D::Type::UNSIGNED_BYTE,
					0
				);
				image.texture = texture;
			}
		}
		return true;
	}
};