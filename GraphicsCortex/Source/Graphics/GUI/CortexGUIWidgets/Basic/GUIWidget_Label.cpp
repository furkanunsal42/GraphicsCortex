#include "GUIWidget_Label.h"

widget::Label::Label()
{
	color = glm::vec4(0, 0, 0, 0);
}

Element& widget::Label::get_element(glm::vec2 allocated_size) {
	apply_properties_to_element(element, allocated_size);
	update_glyphs();
	return element;
}

int32_t widget::Label::get_glyph_count()
{
	return glyphs.size();
}

glm::vec2 widget::Label::get_glyph_size(int32_t index)
{
	if (index < 0 || index >= get_glyph_count()) {
		std::cout << "[GUI Error] widget::Label::get_glyph_size() is called for a glyph that doesn't exist" << std::endl;
		ASSERT(false);
	}
	
	return glyphs[index].second.size();
}

glm::vec2 widget::Label::get_glyph_position(int32_t index)
{
	if (index < 0 || index >= get_glyph_count()) {
		std::cout << "[GUI Error] widget::Label::get_glyph_size() is called for a glyph that doesn't exist" << std::endl;
		ASSERT(false);
	}

	return glyphs[index].second.position();
}

void widget::Label::update_glyphs()
{
	if (!FontBank::get().does_font_exist(font))
		return;

	while (glyphs.size() > text.size()) {
		auto& g = glyphs.back();
		GUI::get().release_element(g.second);
		glyphs.pop_back();
	}
	while (glyphs.size() < text.size())
		glyphs.push_back(std::pair<uint32_t, Element>(0, element.create_child()));


	float advance = 0;
	glm::vec2 text_size(0);

	for (int32_t i = 0; i < text.size(); i++) {
	
		Element& g = glyphs[i].second;
		FontBank::get().get_font(font);
		g.texture() = FontBank::get().get_font(font).atlas;
		float font_size = FontBank::get().get_font(font).font_size;
		glm::vec2 atlas_size = FontBank::get().get_font(font).atlas->get_size();
		auto& table = FontBank::get().get_font(font).glyph_table.at(text[i]);

		g.texcoord_min() = table.coords_low;
		g.texcoord_max() = table.coords_hi;
		std::swap(g.texcoord_min().y, g.texcoord_max().y);
		g.texcoord_min().y = 1 - g.texcoord_min().y;
		g.texcoord_max().y = 1 - g.texcoord_max().y;

		glm::vec2 inverted_offset = glm::vec2(table.offset.x, -table.offset.y);

		g.position() = glm::vec2(advance, 0) + inverted_offset * atlas_size / font_size * text_height + glm::vec2(0, text_height);
		g.size() = (table.coords_hi - table.coords_low) * atlas_size / font_size * text_height;
		g.z() = z;
		g.color() = text_color;

		text_size.y = glm::max(text_size.y, g.position().y + g.size().y);
		text_size.x = g.position().x + g.size().x;

		advance += table.advance * atlas_size.x / font_size * text_height;
		glyphs[i].first = text[i];
	}

	for (int32_t i = 0; i < text.size(); i++) {
		Element& g = glyphs[i].second;
		text_size.y = glm::max(text_size.y, g.size().y);
		text_size.x = g.position().x + g.size().x;
	}

	if (element.size().x == 0)	element.size().x = text_size.x;
	if (element.size().y == 0)	element.size().y = text_size.y;
}
