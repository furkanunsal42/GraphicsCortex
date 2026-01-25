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

		Package::load_package("graphics_cortex.hbv");
		Package::loaded_package->print_headers();

		widget2::DefaultStyle style;

		GUIDynamic gui_d(true);

		FontBank::get().load_font("../GraphicsCortex/Fonts/Roboto-Regular.ttf", 32 * gui_d.get_gui_scale());

		while (true) {			
			static bool once = true;

			gui_d.new_frame();

			static widget2::Window window0;
			static widget2::MenuBar menubar;

			style.apply(window0);
			window0.padding = glm::vec4(0);

			if (once) {
				window0.target_size.value.x = 600;
				window0.target_size.value.y = 400;
			}
			
			//window0.drag(gui_d, window0);
			window0.publish_begin(gui_d);
			
			if (gui_d.get_window_handle(window0.id) != nullptr && gui_d.get_window_handle(window0.id)->should_close())
				exit(0);
			
			window0.publish_menubar_begin(gui_d);
			
			style.apply(menubar);
			menubar.publish_begin(gui_d);

			window0.drag(gui_d, menubar);

			if (menubar.window_controls.minimize_button.click.is_activated_now(gui_d))
				window0.desire_iconify();
			
			if (menubar.window_controls.restore_button.click.is_activated_now(gui_d))
				window0.desire_maximal_restore_swap();
			
			if (menubar.window_controls.close_button.click.is_activated_now(gui_d) && gui_d.get_window_handle(window0.id) != nullptr)
				gui_d.get_window_handle(window0.id)->set_should_close(true);
			
			static widget2::Menu menu_file;
			menu_file.text = U"File";
			
			style.apply(menu_file);
			menu_file.publish_begin(gui_d);
			
			if (menu_file.drop.is_active()) {
				static widget2::MenuItem menu_item_new;
				static widget2::MenuItem menu_item_open;
				static widget2::MenuItem menu_item_close;
				static widget2::MenuItem menu_item_save;
				static widget2::MenuItem menu_item_save_all;
			
				menu_item_new.text = U"New";
				menu_item_open.text = U"Open";
				menu_item_close.text = U"Close";
				menu_item_save.text = U"Save";
				menu_item_save_all.text = U"Save All";
			
				style.apply(menu_item_new);
				style.apply(menu_item_open);
				style.apply(menu_item_close);
				style.apply(menu_item_save);
				style.apply(menu_item_save_all);
			
				menu_item_new.publish(gui_d,		menu_file);
				menu_item_open.publish(gui_d,		menu_file);
				menu_item_close.publish(gui_d,		menu_file);
				menu_item_save.publish(gui_d,		menu_file);
				menu_item_save_all.publish(gui_d,	menu_file);
			}
			
			menu_file.publish_end(gui_d);
			
			
			
			static widget2::Menu menu_edit;
			menu_edit.text = U"Edit";
			
			style.apply(menu_edit);
			menu_edit.publish_begin(gui_d);
			
			if (menu_edit.drop.is_active()) {
				static widget2::MenuItem menu_item_undo;
				static widget2::MenuItem menu_item_redo;
				static widget2::MenuItem menu_item_cut;
				static widget2::MenuItem menu_item_copy;
				static widget2::MenuItem menu_item_paste;
			
				menu_item_undo.text = U"Undo";
				menu_item_redo.text = U"Redo";
				menu_item_cut.text = U"Cut";
				menu_item_copy.text = U"Copy";
				menu_item_paste.text = U"Paste";
			
				style.apply(menu_item_undo);
				style.apply(menu_item_redo);
				style.apply(menu_item_cut);
				style.apply(menu_item_copy);
				style.apply(menu_item_paste);
			
				menu_item_undo.publish(gui_d, menu_edit);
				menu_item_redo.publish(gui_d, menu_edit);
				menu_item_cut.publish(gui_d, menu_edit);
				menu_item_copy.publish(gui_d, menu_edit);
				menu_item_paste.publish(gui_d, menu_edit);
			}
			
			menu_edit.publish_end(gui_d);
			
			
			menubar.publish_end(gui_d);
			
			window0.publish_menubar_end(gui_d);


			gui_d.stack_begin()
				.set_target_size(glm::vec2(avail, fit))
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
			image_button.image.texture = gui_d.gui_texture_bank.get_texture("play.svg");
			image_button.image.color = glm::vec4(0.13f, 0.51f, 0.58f, 1);
			image_button.publish(gui_d);

			static widget2::CheckBox checkbox;
			static bool checkbox_value = false;

			style.apply(checkbox);

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
			//window1.drag(gui_d, window1);
			//window1.publish_begin(gui_d);
			//gui_d.box_begin();
			//window1.publish_end(gui_d);
			
			gui_d.stack_end();

			window0.publish_end(gui_d);

			gui_d.publish();

			if (once)
				gui_d.print_layout();

			once = false;

		}
		return true;
	}
};