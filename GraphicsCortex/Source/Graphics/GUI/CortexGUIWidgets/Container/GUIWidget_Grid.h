#pragma once
#include "GUI/CortexGUI.h"

namespace widget {

	class Grid : public Widget {
	public:

		void add_row(float height);
		void add_column(float width);

		void add(widget_t widget, int32_t row_index, glm::ivec2 column_index);
		void remove(widget_t widget);
		
		Element& get_element(glm::vec2 allocated_size) {
			apply_properties_to_element(element, allocated_size);
			lay_widgets();
			return element;
		}

	private:

		glm::vec2 compute_min_content_size();
		glm::vec2 compute_total_relative_size();

		void lay_widgets();

		std::vector<float> rows;
		std::vector<float> columns;

		struct widget_info {
			widget_info() = default;
			widget_info(widget_t widget, int32_t row_id, int32_t column_id);

			widget_t widget = invalid_widget;
			int32_t row_id = 0;
			int32_t column_id = 0;
		};

		std::vector<widget_info> widgets;
	};
}
