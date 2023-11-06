#pragma once

#include "SharedPtr_Program.h"
#include "SharedPtr_Font.h"

class Text : public UpdatesDefaultUniforms {
public:
	Text(Font_s font, const std::string& text = u8"");
	Text(Font_s font, const std::u16string& text);
	Text(Font_s font, const std::u32string& text);

	Text(Font_s font, Program_s custom_renderer, const std::string& text = u8"");
	Text(Font_s font, Program_s custom_renderer, const std::u16string& text);
	Text(Font_s font, Program_s custom_renderer, const std::u32string& text);

	~Text();

	void set_text(const std::u32string& text);
	void set_text(const std::u16string& text);
	void set_text(const std::string& text);

	std::string get_text8();
	std::u16string get_text16();
	std::u32string get_text32();

	enum string_type {
		ASCI,
		UTF_16,
		UTF_32,
		NON_INITIALIZED,
	};

	string_type get_string_type();

	void set_scale(float scale);
	float get_scale();
	void set_max_width(float max_width);
	float get_max_width();
	void set_wait_for_words(bool wait_for_words);
	bool get_wait_for_words();
	void set_color(const vec4& color);
	vec4 get_color();

	void render();

	void update_default_uniforms(Program& program);

	Graphic_s graphic;

private:

	bool _graphic_needs_update = true;
	void _update_graphic();

	union {
		std::string text8 = "";
		std::u16string text16;
		std::u32string text32;
	};
	void _deconstruct_text();

	string_type _string_type = NON_INITIALIZED;

	float _scale = 1;
	float _text_max_width = 0;
	bool _wait_for_words = true;
	
	Font_s _font;
	vec4 _color = vec4(0.5, 0.5, 0.5, 1);

	bool _custom_renderer_loaded = false;

	static Program_s _default_text_renderer;
	static bool _default_renderer_initialized;
	static void _initialize_default_renderer();

};