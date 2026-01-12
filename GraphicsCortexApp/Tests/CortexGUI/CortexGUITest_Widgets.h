#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "GUI/GUI.h"
#include "GUI/GUIDynamic.h"
#include "GUI/GUIWidgets.h"
#include "GUI/GUIWidgetStandardStyle.h"

class CortexGUITest_Widgets : public TestBench {
public:

	bool run() {

		constexpr auto avail		= GUIDynamic::avail;
		constexpr auto fit			= GUIDynamic::fit;
		constexpr auto invalid_id	= GUIDynamic::invalid_id;

		widget2::DefaultStyle style;

		GUI gui;
		GUIDynamic gui_d;

		FontBank::get().load_font("../GraphicsCortex/Fonts/Roboto-Regular.ttf", 100);

		
		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(
				256, 256,
				Texture2D::ColorTextureFormat::RGBA8,
				1, 0, 0
			);

		texture->is_bindless = true;
			
		texture->load_data(
				Image("../GraphicsCortex/Images/orange.png", 256, 256, 1, 4, 1, true),
				Texture2D::ColorFormat::RGBA,
				Texture2D::Type::UNSIGNED_BYTE,
				0
			);
		
		while (true) {			
			gui_d.new_frame(gui);

			static widget2::Window window0;
			
			style.apply(window0);
			window0.draggable = true;
			window0.publish(gui_d);
			
			gui_d.stack_begin()
				.set_target_size(glm::vec2(600, fit))
				.set_padding(glm::vec4(0, 40, 0, 0));

			static widget2::Box style_box;
			
			style.apply(style_box);
			style_box.border_color0		= glm::vec4(0, 0, 1, 1);
			style_box.border_thickness	= glm::vec4(2);
			style_box.border_rounding	= glm::vec4(10);
			style_box.target_size		= glm::vec2(avail, 32);
			style_box.color				= glm::vec4(1, 1, 1, 1);
			style_box.color.transition(style_box.hover, glm::vec4(1, 0, 1, 1), std::chrono::milliseconds(100));
			style_box.color.transition(style_box.hold, glm::vec4(0, 1, 0, 1), std::chrono::milliseconds(200));
			style_box.margin			= glm::vec4(0);
			style_box.margin.transition(style_box.hold, glm::vec4(20, 0, 20, 0), std::chrono::milliseconds(85));
			style_box.publish(gui_d);

			static widget2::Image image;
			
			style.apply(image);
			image.texture				= texture;
			image.target_size			= glm::vec2(avail, 64);
			image.type					= widget2::Image::Fit;
			image.color.transition(image.hover, glm::vec4(1, 0, 0, 1));
			image.publish(gui_d);

			static widget2::Label label;
			static float slider_value = 16;

			style.apply(label);
			label.text_height = slider_value;
			label.text_color			= glm::vec4(0, 0, 0, 1);
			label.text_color.transition(label.hover, glm::vec4(1, 0, 0, 1), std::chrono::milliseconds(100));
			label.publish(gui_d, U"Büyüt de Götüne Girsin");

			static widget2::TextInput text_input;
			static std::u32string text_input_str;

			style.apply(text_input);
			text_input.publish(gui_d, text_input_str);
			
			static widget2::Slider slider;

			style.apply(slider);
			slider.max_value = 32;
			slider.min_value = 8;
			slider.publish(gui_d, slider_value);

			static widget2::DragFloat drag_float;

			style.apply(drag_float);
			drag_float.publish(gui_d, slider_value);
			
			static widget2::Button button;

			style.apply(button);
			button.publish(gui_d);

			if (button.click.is_activated_now(gui_d)) {
				text_input_str = U"";
			}

			static widget2::ImageButton image_button;

			style.apply(image_button);
			image_button.image.texture = texture;
			image_button.publish(gui_d);

			static widget2::CheckBox checkbox;
			static bool checkbox_value = false;

			style.apply(checkbox);
			checkbox.image.texture = texture;
			checkbox.publish(gui_d, checkbox_value);

			static widget2::ComboBox combobox;
			
			style.apply(combobox);
			combobox.publish(gui_d);
			
			if (combobox.drop.is_active()) {

				gui_d.box_begin()
					.set_color(glm::vec4(1, 1, 1, 1))
					.set_target_size(glm::vec2(400, 200));

			}

			combobox.end(gui_d);

			//static widget2::Window window1;
			//style.apply(window1);
			//window1.draggable = true;
			//window1.publish(gui_d);
			//gui_d.box_begin();
			//gui_d.window_end();

			gui_d.stack_end();
			gui_d.window_end();
			
			gui_d.publish(gui);
			gui.render();

			static bool once = true;
			if (once)
				gui_d.print_layout();
			once = false;

		}
		return true;
	}
};