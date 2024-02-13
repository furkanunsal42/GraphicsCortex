#pragma once
#include "Gui.h"

class Button : public GuiWidget {
public:

	Style button_style;
	vec2 size;
	std::u32string text;

	Button(const std::string& name, vec2 size = vec2(120, 25), std::u32string text = U"") {
		this->name = name;
		this->size = size;
		this->text = text;

		button_style.cursor_type = Frame::Hand;
		button_style.color = gui::colorcode(0x313547);
		button_style.text_color = gui::colorcode(0xffffff);
		button_style.text_size = 10;
		button_style.on_hover.color = gui::colorcode(0x464c69);
	}

	void on_render(Gui& gui) {
		gui.content(name, size, button_style, text);
	}
};