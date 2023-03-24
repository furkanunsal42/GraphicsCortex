#pragma once

#include "SharedPtr_Program.h"
#include "SharedPtr_Font.h"

class Text {
public:
	Text(Font_s font, const std::string& text = "");
	Text(Font_s font, Program_s custom_renderer, const std::string& text = "");

	void set_text(const std::string& text);
	std::string get_text();
	void set_scale(float scale);
	float get_scale();
	void set_max_width(float max_width);
	float get_max_width();
	void set_wait_for_words(bool wait_for_words);
	bool get_wait_for_words();
	void set_color(const vec4& color);
	vec4 get_color();

	void render();

	Graphic_s graphic;
private:

	bool _graphic_needs_update = true;
	void _update_graphic();

	std::string _text = "Hello World";
	float _scale = 1;
	float _text_max_width = 0;
	bool _wait_for_words = true;
	vec4 _color = vec4(1);
	
	Font_s _font;

	bool _custom_renderer_loaded = false;

	static Program_s _default_text_renderer;
	static bool _default_renderer_initialized;
	static void _initialize_default_renderer();
};