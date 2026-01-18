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

		//Package::load_package("graphics_cortex.hbv");
		//Package::loaded_package->print_headers();

		widget2::DefaultStyle style;

		GUIDynamic gui_d;

		FontBank::get().load_font("../GraphicsCortex/Fonts/Roboto-Regular.ttf", 32 * gui_d.get_gui_scale());

		std::shared_ptr<Texture2D> texture = gui_d.gui_texture_bank.get_texture("../GraphicsCortex/Images/orange.png", Texture2D::ColorTextureFormat::RGBA8, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE, glm::ivec2(860, 829));

		while (true) {			
			
			gui_d.new_frame();

			static widget2::Window window0;
			static widget2::MenuBar menubar;

			style.apply(window0);
			window0.padding = glm::vec4(0);
			
			window0.drag(gui_d, menubar);
			window0.publish_begin(gui_d);
			
			//window0.publish_menubar_begin(gui_d);
			//
			//style.apply(menubar);
			//menubar.window_controls.minimize_button.image.texture = texture;
			//menubar.window_controls.restore_button.image.texture = texture;
			//menubar.window_controls.close_button.image.texture = texture;
			//menubar.publish_begin(gui_d);
			//
			//static widget2::Menu menu;
			//
			//style.apply(menu);
			//menu.publish_begin(gui_d);
			//
			//if (menu.drop.is_active()) {
			//
			//}
			//
			//menu.publish_end(gui_d);
			//
			//menubar.publish_end(gui_d);
			//
			//window0.publish_menubar_end(gui_d);


			gui_d.stack_begin()
				.set_target_size(glm::vec2(360, fit))
				.set_padding(glm::vec4(20, 20, 20, 20));

			static float slider_value = 12;
			static widget2::Label label;

			style.apply(label);
			label.text_height = slider_value;
			label.text_color			= glm::vec4(0, 0, 0, 1);
			label.text_color.transition(label.hover, glm::vec4(1, 0, 0, 1), std::chrono::milliseconds(100));
			label.publish(gui_d, U"Hello World!");

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
			combobox.publish_begin(gui_d);
			
			if (combobox.drop.is_active()) {

				static widget2::ComboBoxItem item0;
				static widget2::ComboBoxItem item1;
				static widget2::ComboBoxItem item2;
				static widget2::ComboBoxItem item3;
				static widget2::ComboBoxItem item4;

				item0.text = U"item0";
				item1.text = U"item1";
				item2.text = U"item2";
				item3.text = U"item3";
				item4.text = U"item4";

				style.apply(item0);
				style.apply(item1);
				style.apply(item2);
				style.apply(item3);
				style.apply(item4);

				item0.publish(gui_d, combobox);
				item1.publish(gui_d, combobox);
				item2.publish(gui_d, combobox);
				item3.publish(gui_d, combobox);
				item4.publish(gui_d, combobox);
			}

			combobox.publish_end(gui_d);

			//static widget2::Window window1;
			//style.apply(window1);
			//window1.draggable = true;
			//window1.publish(gui_d);
			//gui_d.box_begin();
			//gui_d.window_end();


			gui_d.stack_end();

			//gui_d.grid_end();
			//gui_d.window_end();
			window0.publish_end(gui_d);

			gui_d.publish();

			static bool once = true;
			if (once)
				gui_d.print_layout();
			once = false;

		}
		return true;
	}
};