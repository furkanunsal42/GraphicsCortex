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

class TextInput : public GuiWidget {
public:

	Style style;
	Style default_text_style;

	vec2 size;
	std::u32string default_text;
	std::u32string current_text;

	TextInput(const std::string& name, vec2 size = vec2(120, 25), std::u32string default_text = U"") {
		this->name = name;
		this->size = size;
		this->default_text = default_text;

		default_text_style.text_color = gui::colorcode(0x9c9c9c);
		default_text_style.text_size = 10;
		
		style.text_color = default_text_style.text_color;
		style.text_allign_type = default_text_style.text_allign_type;
		style.text_color_change = default_text_style.text_color_change;
		style.text_color_interpolation = default_text_style.text_color_interpolation;
		style.text_size = default_text_style.text_size;
		style.text_size_change = default_text_style.text_size_change;
		style.text_size_interpolation = default_text_style.text_size_interpolation;

		style.cursor_type = Frame::CursorType::IBeam;
		style.color = gui::colorcode(0xf0f0f0);
		style.corner_rounding = vec4(20, 20, 20, 20);
		style.on_hover.corner_rounding = vec4(0, 0, 0, 0);
		style.corner_rounding_change = 0.4;
		style.corner_rounding_interpolation = Interpolation::polynomial(4);
		style.on_hover.color = gui::colorcode(0xf2f2f2);
	}

	void on_render(Gui& gui) {
		
		if (!is_focusing)
			gui.content(name, size, style, default_text);
		else 
			gui.content(name, size, style, U"Writing...");


	}
};