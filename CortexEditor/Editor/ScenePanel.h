#pragma once
#include "Editor.h"
#include <string>
#include <unordered_set>

class ScenePanel : public UILayer {

	Style layout_style;
	Style object_style;
	Style selected_object_style;
	Style button_style;
	Style searchbar_style;

	vec2 position = vec2(0, 30);

	void init() {
		layout_style.text_color = vec3(1, 1, 1);
		layout_style.text_size = 10;
		layout_style.color = gui::colorcode(0x242424);

		object_style.text_color = vec3(1, 1, 1);
		object_style.text_size = 10;
		object_style.color = gui::colorcode(0x242424);
		object_style.on_hover.color = gui::colorcode(0x303030);
		object_style.margin = vec4(0, 10, 0, 10);
		object_style.color_change = 0.1;
		object_style.color_interpolation = Interpolation::polynomial(0.5);
	
		selected_object_style = object_style;
		selected_object_style.color = gui::colorcode(0x006AAB);
		selected_object_style.on_hover.color.disable();

		button_style.text_color = vec3(1, 1, 1);
		button_style.text_size = 10;
		button_style.color = gui::colorcode(0x242424);
		button_style.on_hover.color = gui::colorcode(0x303030);
		button_style.color_change = 0.1;
		button_style.color_interpolation = Interpolation::polynomial(0.5);
		button_style.margin = vec4(10, 10, 10, 10);

		searchbar_style.text_color = vec3(1, 1, 1);
		searchbar_style.text_size = 10;
		searchbar_style.color = gui::colorcode(0x181818);
		searchbar_style.on_hover.color = gui::colorcode(0x303030);
		searchbar_style.color_change = 0.1;
		searchbar_style.color_interpolation = Interpolation::polynomial(0.5);
		searchbar_style.corner_rounding = vec4(10, 10, 10, 10);
		searchbar_style.on_hover.corner_rounding = vec4(0, 0, 0, 0);

		searchbar_style.margin = vec4(10, 10, 10, 10);

	}

	std::unordered_set<std::shared_ptr<void>> selected_pointers;

	void on_gui_render() {
		Editor& editor = Editor::get();
			
		editor.gui->layout(vec2(position.x, position.y), vec2(200, 200), layout_style, U"");
		editor.gui->content(vec2(200, 30), layout_style, U"ScenePanel");
		editor.gui->content(vec2(200, 30), searchbar_style, U"Search");
		
		bool clicked_on_button = false;

		for (std::shared_ptr<Graphic> graphic : editor.get_current_scene()->_graphics) {
			Style& style_to_use = (selected_pointers.find(graphic) != selected_pointers.end()) ? selected_object_style : object_style;
			if (editor.gui->content(vec2(200, 20), style_to_use, U"Graphic").click_released()) {
				if (!editor.frame->get_key_press(Frame::Key::LEFT_CONTROL)) selected_pointers.clear();
				selected_pointers.insert(graphic);
				clicked_on_button = true;
			}
		}

		for (std::shared_ptr<Light> light : editor.get_current_scene()->_lights) {
			Style& style_to_use = (selected_pointers.find(light) != selected_pointers.end()) ? selected_object_style : object_style;
			if (editor.gui->content(vec2(200, 20), style_to_use, U"Light").click_released()) {
				if (!editor.frame->get_key_press(Frame::Key::LEFT_CONTROL)) selected_pointers.clear();
				selected_pointers.insert(light);
				clicked_on_button = true;
			}
		}

		for (std::shared_ptr<Object> object : editor.get_current_scene()->_objects) {
			Style& style_to_use = (selected_pointers.find(object) != selected_pointers.end()) ? selected_object_style : object_style;
			if (editor.gui->content(vec2(200, 20), style_to_use, U"Object").click_released()) {
				if (!editor.frame->get_key_press(Frame::Key::LEFT_CONTROL)) selected_pointers.clear();
				selected_pointers.insert(object);
				clicked_on_button = true;
			}
		}

		for (std::shared_ptr<Vehicle> vehicle : editor.get_current_scene()->_vehicles) {
			Style& style_to_use = (selected_pointers.find(vehicle) != selected_pointers.end()) ? selected_object_style : object_style;
			if (editor.gui->content(vec2(200, 20), style_to_use, U"Vehicle").click_released()) {
				if (!editor.frame->get_key_press(Frame::Key::LEFT_CONTROL)) selected_pointers.clear();
				selected_pointers.insert(vehicle);
				clicked_on_button = true;
			}
		}

		for (std::shared_ptr<Text> text : editor.get_current_scene()->_texts) {
			Style& style_to_use = (selected_pointers.find(text) != selected_pointers.end()) ? selected_object_style : object_style;
			if (editor.gui->content(vec2(200, 20), style_to_use, U"Text").click_released()) {
				if (!editor.frame->get_key_press(Frame::Key::LEFT_CONTROL)) selected_pointers.clear();
				selected_pointers.insert(text);
				clicked_on_button = true;
			}
		}

		bool clicked_on_panel = editor.gui->layout_end().click_released();

		if (clicked_on_panel && !clicked_on_button)
			selected_pointers.clear();
	}
};