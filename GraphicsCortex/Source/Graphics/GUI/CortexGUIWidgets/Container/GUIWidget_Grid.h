#pragma once
#include "GUI/CortexGUI.h"

namespace widget {

	class Grid : public Widget {
	public:

		Grid();

		void add_row(float height);
		void add_column(float width);

		void add(widget_t widget, int32_t row_index, int32_t column_index);
		void remove(widget_t widget);
		
		Element& get_element(glm::vec2 allocated_size);

	private:

		//glm::vec2 compute_slot_physical_size(int32_t row, int32_t column, glm::vec2 min_content_size, glm::vec2 total_relative_size);
		void update_row_and_column_fit_size();
		void update_row_and_column_positions();

		glm::vec2 update_rows_and_column_physical_size();
		
		glm::vec2 compute_widget_total_known_size(Widget& widget);
		glm::vec2 compute_widget_total_relative_size(Widget& widget);

		void update_slot_element_count();

		void lay_widgets();

		std::vector<float> row_positions;
		std::vector<float> column_positions;

		std::vector<float> rows_physical_size;
		std::vector<float> columns_physical_size;

		std::vector<float> rows;
		std::vector<float> columns;

		std::vector<Element> slot_elements;

		struct widget_info {
		public:
			widget_info() = default;
			widget_info(widget_t widget, int32_t row_id, int32_t column_id);

			widget_t widget = invalid_widget;
			int32_t row_id = 0;
			int32_t column_id = 0;
		};

		std::vector<widget_info> widgets;
	};
}
