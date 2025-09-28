#include "GUIWidget_Grid.h"

widget::Grid::widget_info::widget_info(widget_t widget, int32_t row_id, int32_t column_id) :
	widget(widget), row_id(row_id), column_id(column_id) {}

widget::Grid::Grid() {
	color = glm::vec4(0, 0, 0, 0);
}

void widget::Grid::add_row(float height)
{
	rows.push_back(height);
}

void widget::Grid::set_row_size(float height, int32_t row_index)
{
	if (row_index >= rows.size() || row_index < 0) {
		std::cout << "[GUI Error] widget::Grid::set_row_size() is called with an invalid row index";
		ASSERT(false);
	}

	rows[row_index] = height;
}

void widget::Grid::add_column(float width)
{

	columns.push_back(width);
}

void widget::Grid::set_column_size(float width, int32_t column_index)
{
	if (column_index >= columns.size() || column_index < 0) {
		std::cout << "[GUI Error] widget::Grid::set_column_size() is called with an invalid column index";
		ASSERT(false);
	}

	columns[column_index] = width;
}

void widget::Grid::add(widget_t widget, int32_t row_index, int32_t column_index)
{
	if (widget == invalid_widget) {
		std::cout << "[GUI Error] widget::Grid::add() is called with an invalid widget";
		ASSERT(false);
	}

	if (row_index >= rows.size() || row_index < 0 ||
		column_index >= columns.size() || column_index < 0) {
		std::cout << "[GUI Error] widget::Grid::add() is called with an invalid row or column index";
		ASSERT(false);
	}

	widgets.push_back(widget_info(widget, row_index, column_index));
}

void widget::Grid::remove(widget_t widget)
{
	std::erase_if(widgets, [&](const widget_info& info) { return info.widget == widget; });
}

Element& widget::Grid::get_element(glm::vec2 allocated_size) {
	apply_properties_to_element(element, allocated_size);
	lay_widgets();
	return element;
}

void widget::Grid::poll_events(glm::vec2 absolute_position)
{
	Widget::poll_events(absolute_position);

	AABB2 self_aabb(absolute_position, absolute_position + element.size());
	glm::vec2 cursor_pos = GUI::get().get_window()->get_cursor_position();

	if (self_aabb.does_contain(cursor_pos)) {
		for (widget_info info : widgets)
			GUI::get().get_widget_data(info.widget).poll_events(absolute_position + slot_elements[info.column_id * rows.size() + info.row_id].position());
	}
}

void widget::Grid::update_row_and_column_positions()
{
	row_positions.resize(rows.size());
	column_positions.resize(columns.size());

	float running_width = 0;
	for (int32_t i = 0; i < columns.size(); i++) {
		column_positions[i] = running_width;
		running_width += columns_physical_size[i];
	}

	float running_height = 0;
	for (int32_t i = 0; i < rows.size(); i++) {
		row_positions[i] = running_height;
		running_height += rows_physical_size[i];
	}
}

void widget::Grid::update_row_and_column_fit_size()
{
	rows_physical_size = rows;
	columns_physical_size = columns;

	for (widget_info& widget : widgets) {

		Widget& widget_data = GUI::get().get_widget_data(widget.widget);

		glm::vec2 min_child_content_size(0);
		glm::vec2 allocated_size(columns[widget.column_id], rows[widget.row_id]);

		if (glm::any(glm::equal(allocated_size, glm::vec2(0)))) {
			glm::vec4 positive_margin = glm::max(widget_data.margin, glm::vec4(0));
			
			min_child_content_size = widget_data.get_element(allocated_size).size();
			min_child_content_size += glm::vec2(positive_margin.x + positive_margin.z, positive_margin.y + positive_margin.w);
		}

		if (allocated_size.x == 0)
			columns_physical_size[widget.column_id] = glm::max(columns_physical_size[widget.column_id], min_child_content_size.x);

		if (allocated_size.y == 0)
			rows_physical_size[widget.row_id] = glm::max(rows_physical_size[widget.row_id], min_child_content_size.y);
	}
}

glm::vec2 widget::Grid::update_rows_and_column_physical_size() {
	
	update_row_and_column_fit_size();

	glm::vec2 min_content_size(0);
	glm::vec2 total_relative_size(0);

	for (float width : columns_physical_size) {
		if (width > 0) min_content_size.x += width;
		if (width < 0) total_relative_size.x += width;
	}

	for (float height : rows_physical_size) {
		if (height > 0) min_content_size.y += height;
		if (height < 0) total_relative_size.y += height;
	}

	if (element.size().x == 0) element.size().x = min_content_size.x + padding.x + padding.z;
	if (element.size().y == 0) element.size().y = min_content_size.y + padding.y + padding.w;
	
	for (float& width : columns_physical_size)
		if (width < 0) width / total_relative_size.x * (element.size().x - min_content_size.x);

	for (float& height : rows_physical_size)
		if (height < 0) height / total_relative_size.y * (element.size().y - min_content_size.y);

	return min_content_size;
}

glm::vec2 widget::Grid::compute_widget_total_known_size(Widget& widget)
{
	glm::vec2 total_known_size(0);

	if (widget.target_size.x > 0)	total_known_size.x += widget.target_size.x;
	if (widget.target_size.x == 0)	total_known_size.x += widget.get_element(glm::vec2(0)).size().x;
	if (widget.margin.x > 0)		total_known_size.x += widget.margin.x;
	if (widget.margin.z > 0)		total_known_size.x += widget.margin.z;

	if (widget.target_size.y > 0)	total_known_size.y += widget.target_size.y;
	if (widget.target_size.y == 0)	total_known_size.y += widget.get_element(glm::vec2(0)).size().y;
	if (widget.margin.y > 0)		total_known_size.y += widget.margin.y;
	if (widget.margin.w > 0)		total_known_size.y += widget.margin.w;

	return total_known_size;
}

glm::vec2 widget::Grid::compute_widget_total_relative_size(Widget& widget)
{
	glm::vec2 total_relative_size(0);

	if (widget.target_size.x < 0)	total_relative_size.x += widget.target_size.x;
	if (widget.margin.x < 0)		total_relative_size.x += widget.margin.x;
	if (widget.margin.z < 0)		total_relative_size.x += widget.margin.z;

	if (widget.target_size.y < 0)	total_relative_size.y += widget.target_size.y;
	if (widget.margin.y < 0)		total_relative_size.y += widget.margin.y;
	if (widget.margin.w < 0)		total_relative_size.y += widget.margin.w;

	return total_relative_size;
}

void widget::Grid::update_slot_element_count()
{
	int32_t slot_count = rows.size() * columns.size();

	while (slot_elements.size() > slot_count) {
		GUI::get().release_element(slot_elements.back());
		slot_elements.pop_back();
	}

	while (slot_elements.size() < slot_count) {
		slot_elements.push_back(element.create_child());
		slot_elements.back().color() = glm::vec4(0, 0, 0, 0);
	}

	for (int32_t x = 0; x < columns.size(); x++) {
		for (int32_t y = 0; y < rows.size(); y++) {

			glm::vec2 slot_physical_size = glm::vec2(columns_physical_size[x], rows_physical_size[y]);

			glm::vec2 position = glm::vec2(column_positions[x], row_positions[y]) + glm::vec2(padding.x, padding.y);

			Element& slot = slot_elements[x * rows.size() + y];
			slot.size() = slot_physical_size;
			slot.position() = position;

		}
	}
}

void widget::Grid::lay_widgets() {
	
	glm::vec2 min_content_size	= update_rows_and_column_physical_size();

	update_row_and_column_positions();
	update_slot_element_count();

	for (int32_t i = 0; i < widgets.size(); i++) {
		int32_t row = widgets[i].row_id;
		int32_t column = widgets[i].column_id;

		Widget& child_widget = GUI::get().get_widget_data(widgets[i].widget);

		if (row >= rows.size() || row < 0 ||
			column >= columns.size() || column < 0) {
			std::cout << "[GUI Error] widgets::Grid::lay_widget() is called but a widget's row or column id is invalid" << std::endl;
			ASSERT(false);
		}
		glm::vec2 slot_physical_size = glm::vec2(columns_physical_size[column], rows_physical_size[row]);
		
		glm::vec2 child_known_size		= compute_widget_total_known_size(child_widget);
		glm::vec2 child_relative_size	= compute_widget_total_relative_size(child_widget);

		glm::vec2 child_size_per_negative = (slot_physical_size - child_known_size) / child_relative_size;

		glm::vec2 child_physical_size	= child_widget.target_size;
		glm::vec4 child_physical_margin = child_widget.margin;

		if (child_physical_size.x < 0) child_physical_size.x *= child_size_per_negative.x;
		if (child_physical_size.y < 0) child_physical_size.y *= child_size_per_negative.y;

		if (child_physical_margin.x < 0) child_physical_margin.x *= child_size_per_negative.x;
		if (child_physical_margin.y < 0) child_physical_margin.y *= child_size_per_negative.y;
		if (child_physical_margin.z < 0) child_physical_margin.z *= child_size_per_negative.x;
		if (child_physical_margin.w < 0) child_physical_margin.w *= child_size_per_negative.y;

		Element& child_element = child_widget.get_element(child_physical_size);

		//if (child_element.get_parent() != Element::null_element && child_element.get_parent() != element) {
		//	std::cout << "[GUI Error] Widgets cannot be part of multiple container Widgets" << std::endl;
		//	ASSERT(false);
		//}

		child_element.set_parent(slot_elements[column * rows.size() + row]);

		child_element.position() = glm::vec2(child_physical_margin.x, child_physical_margin.y);
	}
}