#pragma once
#include "GUI/CortexGUI.h"

#include "Font.h"

namespace widget {

	class Label : public Widget {
	public:

		Label() {
			color = glm::vec4(0, 0, 0, 0);
		}

		font_id font = FontBank::not_a_font;
		float text_height = 16;
		std::u32string text = U"";
		glm::vec4 text_color = glm::vec4(0, 0, 0, 1);

		Element& get_element(glm::vec2 allocated_size) {
			apply_properties_to_element(element, allocated_size);
			update_glyphs();
			return element;
		}

		//void poll_events(glm::vec2 absolute_position) override {
		//	AABB2 self_aabb(absolute_position, absolute_position + element.size());
		//	glm::vec2 cursor_pos = GUI::get().get_window()->get_cursor_position();
		//
		//	//std::cout << "label pos: " << self_aabb.low.x << ", " << self_aabb.low.y << "   end_pos: " << self_aabb.high.x << ", " << self_aabb.high.y << std::endl;
		//	//std::cout << "cursor pos: " << cursor_pos.x << ", " << cursor_pos.y << std::endl;
		//
		//	if (self_aabb.does_contain(cursor_pos) && last_hover_begin == invalid_time) {
		//		events.publish(GUIEvent::Hovered_Weak);
		//		last_hover_begin = std::chrono::system_clock::now();
		//	}
		//	else if (!self_aabb.does_contain(cursor_pos)) {
		//		last_hover_begin = invalid_time;
		//	}
		//}

	private:

		void update_glyphs() {
			if (!FontBank::get().does_font_exist(font))
				return;
			
			//static bool once = true;
			//if (!once)
			//	return;
			//once = false;

			glyphs.resize(text.size());
				
			float advance = 0;

			glm::vec2 text_size(0);

			for (int32_t i = 0; i < text.size(); i++) {
				//if (glyphs[i].first == text[i] && glyphs[i].second != Element::null_element)
				//	continue;

				if (glyphs[i].second == Element::null_element)
					glyphs[i].second = element.create_child();

				//if (glyphs[i].first != text[i]) {
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
				
					text_size.y = glm::max(text_size.y, g.size().y);
					text_size.x = g.position().x + g.size().x;
				
					advance += table.advance * atlas_size.x / font_size * text_height;
					glyphs[i].first = text[i];
				//}
			}

			for (int32_t i = 0; i < text.size(); i++) {
				Element& g = glyphs[i].second;
				text_size.y = glm::max(text_size.y, g.size().y);
				text_size.x = g.position().x + g.size().x;
			}

			if (element.size().x == 0)	element.size().x = text_size.x;
			if (element.size().y == 0)	element.size().y = text_size.y;
		}

		

		std::vector<std::pair<uint32_t, Element>> glyphs;
	};
}
