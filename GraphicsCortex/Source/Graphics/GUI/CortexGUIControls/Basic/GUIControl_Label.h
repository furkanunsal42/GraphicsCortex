#pragma once
#include "GUI/CortexGUIControl.h"

#include "Font.h"

namespace gui_controls {

	class Label : public GUIControl {
	public:

		font_id font = FontBank::not_a_font;
		float text_height = 16;
		std::string text = "";
		glm::vec4 text_color = glm::vec4(0, 0, 0, 1);

		Widget& get_widget() {
			apply_properties_to_widget(widget);
			update_glyphs();
			return widget;
		}

	private:

		void update_glyphs() {
			if (!FontBank::get().does_font_exist(font))
				return;
			
			glyphs.resize(text.size());
				
			float advance = 0;

			for (int32_t i = 0; i < text.size(); i++) {
				if (glyphs[i].first == text[i] && glyphs[i].second != Widget::null_widget)
					continue;

				if (glyphs[i].second == Widget::null_widget)
					glyphs[i].second = GUI::get().create_widget(widget);

				if (glyphs[i].first != text[i]) {
					Widget& g = glyphs[i].second;

					g.texture() = FontBank::get().get_font(font).atlas;
					float font_size = FontBank::get().get_font(font).font_size;
					glm::vec2 atlas_size = FontBank::get().get_font(font).atlas->get_size();
					auto& table = FontBank::get().get_font(font).glyph_table[text[i]];
					
					g.texcoord_min() = table.coords_low;
					g.texcoord_max() = table.coords_hi;
					std::swap(g.texcoord_min().y, g.texcoord_max().y);
					g.texcoord_min().y = 1 - g.texcoord_min().y;
					g.texcoord_max().y = 1 - g.texcoord_max().y;

					glm::vec2 inverted_offset = glm::vec2(table.offset.x, -table.offset.y);

					g.position() = glm::vec2(advance, 0) + inverted_offset * atlas_size / font_size * text_height + glm::vec2(0, text_height);
					g.size() = (table.coords_hi - table.coords_low) * atlas_size / font_size * text_height;
					g.z() = z;

					advance += table.advance * atlas_size.x / font_size * text_height;
					glyphs[i].first = text[i];
				}
			}
		}

		std::vector<std::pair<uint32_t, Widget>> glyphs;
	};
}
