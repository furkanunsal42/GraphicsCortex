#pragma once
#include "GUI/CortexGUI.h"

#include "Font.h"

namespace widget {

	class Label : public Widget {
	public:

		Label();

		font_id font = FontBank::not_a_font;
		float text_height = 16;
		std::u32string text = U"";
		glm::vec4 text_color = glm::vec4(0, 0, 0, 1);

		Element& get_element(glm::vec2 allocated_size);

		int32_t get_glyph_count();
		glm::vec2 get_glyph_size(int32_t index);
		glm::vec2 get_glyph_position(int32_t index);
		
		void set_glyph_color(int32_t index, glm::vec4 color);

	private:

		void update_glyphs();
		
		std::vector<glm::vec4> special_colors;

		std::vector<std::pair<glm::vec2, glm::vec2>> position_sizes;
		std::vector<std::pair<uint32_t, Element>> glyphs;
	};
}
