#include "GUIWidget_Grid.h"

widget::Grid::widget_info::widget_info(widget_t widget, int32_t row_id, int32_t column_id) :
	widget(widget), row_id(row_id), column_id(column_id) {}

glm::vec2 widget::Grid::compute_total_relative_size()
{
	return glm::vec2();
}

glm::vec2 widget::Grid::compute_min_content_size() {

}

void widget::Grid::lay_widgets() {
	for (int32_t i = 0; i < widgets.size(); i++) {
		int32_t row = widgets[i].row_id;
		int32_t column = widgets[i].column_id;

		Widget& child_widget = GUI::get().get_widget_data(widgets[i].widget);

		if (row >= rows.size() || row < 0 ||
			column >= columns.size() || column < 0) {
			std::cout << "[GUI Error] widgets::Grid::lay_widget() is called but a widget's row or column id is invalid" << std::endl;
			ASSERT(false);
		}


	}
}