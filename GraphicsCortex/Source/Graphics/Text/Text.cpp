#include "Text.h"
#include <codecvt>
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

Text::Text(const std::u32string& text)
{
	set_text(text);
}

Text::Text(const std::string& text) :
	Text(to_utf32(text)) {}

void Text::set_font(font_id font)
{
	this->font = font;
}

font_id Text::get_font()
{
	return font;
}

void Text::set_text(const std::u32string& text)
{
	this->_text = text;
	_mesh_needs_update = true;
}

void Text::set_text(const std::string& text)
{
	set_text(to_utf32(text));
}

void Text::_update_mesh() {
	
	if (!_mesh_needs_update)
		return;

	if (!FontBank::get().does_font_exist(font)) {
		std::cout << "Text::_update_mesh() is called but font_id(" << font << ") does not exist" << std::endl;
		ASSERT(false);
	}

	FontBank::Font active_font = FontBank::get().get_font(font);

	if (active_font.atlas == nullptr) {
		std::cout << "Text::_update_mesh() is called but font_id(" << font << ") is not loaded" << std::endl;
		ASSERT(false);
	}

	SingleModel model;

	float pen_x = 0;
	float pen_y = 0;

	int32_t char_count = 0;

	bool next_line_at_next_space = false;
	
	for (const uint32_t& character : _text) {

		if (_text_max_width) {
			if (pen_x > _text_max_width)
				next_line_at_next_space = true;
		}

		const FontBank::glyph_info& character_info = active_font.glyph_table[character];

		float uv_width = character_info.coords_hi.x - character_info.coords_low.x;
		float uv_height = character_info.coords_hi.y - character_info.coords_low.y;

		float xpos = pen_x + character_info.offset.x * _scale;
		float ypos = pen_y - (uv_height - character_info.offset.y) * _scale;

		float w = uv_width * _scale;
		float h = uv_height * _scale;

		if (character == ' ') {
			if (next_line_at_next_space) {
				pen_x = 0;
				pen_y -= active_font.glyph_table['\n'].coords_hi.y * _scale;
				next_line_at_next_space = false;
				continue;
			}
		}

		if (character == '\n') {
			pen_x = 0;
			pen_y -= uv_height * _scale;
			next_line_at_next_space = false;
			continue;
		}

		glm::vec3 new_verticies[] = {
			glm::vec3(xpos,		ypos,		0),
			glm::vec3(xpos + w,	ypos,		0),
			glm::vec3(xpos,		ypos + h,	0),
			glm::vec3(xpos + w,	ypos + h,	0),
		};

		glm::vec2 new_uv[] = {
			glm::vec2(character_info.coords_low.x, 1 - character_info.coords_hi.y),
			glm::vec2(character_info.coords_hi.x, 1 - character_info.coords_hi.y),
			glm::vec2(character_info.coords_low.x, 1 - character_info.coords_low.y),
			glm::vec2(character_info.coords_hi.x, 1 - character_info.coords_low.y),
		};

		uint32_t new_indicies[] = {
			0 + char_count * 4, 1 + char_count * 4, 3 + char_count * 4,
			0 + char_count * 4, 3 + char_count * 4, 2 + char_count * 4,
		};

		for (glm::vec3 vert : new_verticies)
			model.verticies.push_back(vert);
		
		for (glm::vec2 uv : new_uv)
			model.texture_coordinates_0.push_back(uv);

		for (uint32_t value : new_indicies)
			model.indicies.push_back(value);

		pen_x += character_info.advance * _scale;
		char_count++;
	}

	_width = pen_x;
	_height = pen_y;
	
	if (mesh == nullptr)
		mesh = std::make_shared<Mesh>();
	mesh->load_model(model);
	
	_mesh_needs_update = false;
}

std::u32string Text::to_utf32(const std::string& string_utf8) {
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
	return conv.from_bytes(string_utf8);
}
