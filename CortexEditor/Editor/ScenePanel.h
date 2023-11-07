#pragma once
#include "Editor.h"

class ScenePanel : public UILayer {

	Style layout_style;
	Style object_style;
	Style button_style;
	Style searchbar_style;


	vec2 position = vec2(0, 30);

	void init() {
		layout_style.text_color = vec3(1, 1, 1);
		layout_style.text_size = 12;
		layout_style.color = gui::colorcode(0x242424);

		object_style.text_color = vec3(1, 1, 1);
		object_style.text_size = 12;
		object_style.color = gui::colorcode(0x242424);
		object_style.on_hover.color = gui::colorcode(0x303030);
		object_style.margin = vec4(0, 10, 0, 10);
		object_style.color_change = 0.1;
		object_style.color_interpolation = Interpolation::polynomial(0.5);
	
		button_style.text_color = vec3(1, 1, 1);
		button_style.text_size = 12;
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

	void on_gui_render() {
		Editor& editor = Editor::get();
			
		editor.gui->layout(vec2(position.x, position.y), vec2(200, 200), layout_style, U"");
		editor.gui->content(vec2(200, 30), layout_style, U"ScenePanel");
		editor.gui->content(vec2(200, 30), searchbar_style, U"Search");
		for (int i = 0; i < 10; i++) {
			editor.gui->content(vec2(200, 20), object_style, U"Object (1)");
		}

		editor.gui->layout_end();
	}
};