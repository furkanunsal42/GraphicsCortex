#pragma once
#include "GUI/CortexGUI.h"

namespace widget {

	class Grid : public Widget {
	public:

		void define_layout(int32_t row_count, int32_t column_count);
		void define_row_size(int32_t row_index, float height);
		void define_column_size(int32_t column_index, float width);

		void push_back(widget_t widget, int32_t row_index, glm::ivec2 column_index);

		Element& get_element() {
			apply_properties_to_element(element);
			lay_widgets();
			return element;
		}

	private:
		void lay_widgets() {
			std::vector<glm::vec2> positions(widgets.size());
			std::vector<glm::vec2> sizes(widgets.size());

			glm::vec2 current_position(0);
			glm::vec2 content_size(0);

			current_position += glm::vec2(padding.x, padding.y);

			for (int32_t i = 0; i < widgets.size(); i++) {
				Widget& child_widget = GUI::get().get_widget_data(this->widgets[i]);
				Element& child_element = child_widget.get_element();
				if (child_element.get_parent() != Element::null_element && child_element.get_parent() != element) {
					std::cout << "[GUI Error] Widgets cannot be part of multiple container Widgets" << std::endl;
					ASSERT(false);
				}

				child_element.set_parent(element);

				child_element.position() = current_position + glm::vec2(child_widget.margin.x, child_widget.margin.y);
				glm::vec2 total_child_size = child_element.size() + glm::vec2(child_widget.margin.x + child_widget.margin.z, child_widget.margin.y + child_widget.margin.w);
				if (alignment == Vertical) {
					content_size.x = glm::max(content_size.x, total_child_size.x);
					content_size.y = current_position.y + total_child_size.y;
					current_position.y += total_child_size.y;
				}
				else if (alignment == Horizontal) {
					content_size.y = glm::max(content_size.y, total_child_size.y);
					content_size.x = current_position.x + total_child_size.x;
					current_position.x += total_child_size.x;
				}

			}

			element.size() = content_size + glm::vec2(padding.x + padding.z, padding.y + padding.w);
		}

		std::vector<widget_t> widgets;
	};
}
