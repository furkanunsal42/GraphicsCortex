#pragma once

#include "Gui.h"

class DropBox : public CustomWidget{
private:
	bool panel_show = false;
	AABB2 button_aabb;
	vec2 option_size;
	Gui& gui_ref;
	Style button_style;
	Style option_style;
public:

	DropBox(vec2 button_position, vec2 button_size, vec2 option_size, const Style& button_style, const Style& option_style, Gui& gui) :
		button_aabb(button_position, button_size), option_size(option_size), gui_ref(gui), button_style(button_style), option_style(option_style) {}

	void render() {
		auto& dropbox = gui_ref.box(button_aabb, button_style, U"", false);
		dropbox.overwrite_style.clear();
		if (dropbox.click_holding())
			dropbox.overwrite_style.on_hover.color = gui::colorcode(0x494398);
		if (dropbox.click_released()) {
			panel_show = !panel_show;
		}
		dropbox.render();
		for (int i = 0; i < 5; i) {
			auto& drop_option = gui_ref.box(AABB2(button_aabb.position + vec2(0, button_aabb.size.y) + vec2(0, option_size.y * i), option_size), option_style, U"", false);
			if (panel_show)
				drop_option.render();
		}

	}

};