#include "GUIWidget_Stack.h"
#include "Debuger.h"
#include "GUIWidget_Grid.h"

void widget::Stack::poll_events(glm::vec2 absolute_position)
{
	Widget::poll_events(absolute_position);

	AABB2 self_aabb(absolute_position, absolute_position + element.size());
	glm::vec2 cursor_pos = GUI::get().get_window()->get_cursor_position();

	//if (self_aabb.does_contain(cursor_pos)) {
		for (int32_t i = 0; i < widgets.size(); i++)
			GUI::get().get_widget_data(widgets[i]).poll_events(absolute_position + positions[i]);
	//}
}

void widget::Stack::add(widget_t widget)
{
	if (std::find(widgets.begin(), widgets.end(), widget) != widgets.end()) {
		std::cout << "[GUI Error] Stack::push_back() is called with a widget already part of it (id: " << widget << ")" << std::endl;
		ASSERT(false)
	}

	widgets.push_back(widget);
	positions.resize(widgets.size());
}

void widget::Stack::remove(widget_t widget)
{
	std::erase(widgets, widget);
	positions.resize(widgets.size());
}

glm::vec2 widget::Stack::compute_min_content_size() {
	glm::vec2 content_size(0);

	for (int32_t i = 0; i < widgets.size(); i++) {
		Widget& child_widget = GUI::get().get_widget_data(this->widgets[i]);

		if (alignment == Horizontal) {

			if (child_widget.target_size.x > 0)		content_size.x += child_widget.target_size.x;
			if (child_widget.target_size.x == 0)	content_size.x += child_widget.get_element(glm::vec2(0)).size().x;
			if (child_widget.margin.x > 0)			content_size.x += child_widget.margin.x;
			if (child_widget.margin.z > 0)			content_size.x += child_widget.margin.z;

			float child_widget_known_height = 0;
			if (child_widget.target_size.y > 0)		child_widget_known_height += child_widget.target_size.y;
			if (child_widget.target_size.y == 0)	child_widget_known_height += child_widget.get_element(glm::vec2(0)).size().y;
			if (child_widget.margin.y > 0)			child_widget_known_height += child_widget.margin.y;
			if (child_widget.margin.w > 0)			child_widget_known_height += child_widget.margin.w;

			if (child_widget_known_height > 0)		content_size.y = glm::max(content_size.y, child_widget_known_height);
		}
		else if (alignment == Vertical) {

			float child_widget_known_width = 0;
			if (child_widget.target_size.x > 0)		child_widget_known_width += child_widget.target_size.x;
			if (child_widget.target_size.x == 0)	child_widget_known_width += child_widget.get_element(glm::vec2(0)).size().x;
			if (child_widget.margin.x > 0)			child_widget_known_width += child_widget.margin.x;
			if (child_widget.margin.z > 0)			child_widget_known_width += child_widget.margin.z;
			
			if (child_widget_known_width > 0)		content_size.x = glm::max(content_size.x, child_widget_known_width);
			
			if (child_widget.target_size.y > 0)		content_size.y += child_widget.target_size.y;
			if (child_widget.target_size.y == 0)	content_size.y += child_widget.get_element(glm::vec2(0)).size().y;
			if (child_widget.margin.y > 0)			content_size.y += child_widget.margin.y;
			if (child_widget.margin.w > 0)			content_size.y += child_widget.margin.w;
		}
	}
	return content_size;
}

float widget::Stack::compute_total_relative_size_secondary_axis(int32_t widget_index)
{
	if (widgets.size() <= widget_index || widget_index < 0) {
		std::cout << "[GUI Error] widget::Stack::compute_total_relative_size_secondary_axis() is called with invalid widget_index" << std::endl;
		ASSERT(false);
	}

	Widget& child_widget = GUI::get().get_widget_data(this->widgets[widget_index]);

	float total_relative_size = 0;

	if (alignment == Horizontal) {
		if (child_widget.target_size.y < 0)	total_relative_size += child_widget.target_size.y;
		if (child_widget.margin.y < 0)		total_relative_size += child_widget.margin.y;
		if (child_widget.margin.w < 0)		total_relative_size += child_widget.margin.w;
	}
	else if (alignment == Vertical) {
		if (child_widget.target_size.x < 0)	total_relative_size += child_widget.target_size.x;
		if (child_widget.margin.x < 0)		total_relative_size += child_widget.margin.x;
		if (child_widget.margin.z < 0)		total_relative_size += child_widget.margin.z;
	}

	return total_relative_size;
}

float widget::Stack::compute_total_relative_size_main_axis()
{
	float total_relative_size = 0;

	for (int32_t i = 0; i < widgets.size(); i++) {
		Widget& child_widget = GUI::get().get_widget_data(this->widgets[i]);

		if (alignment == Horizontal) {
			if (child_widget.target_size.x < 0)	total_relative_size += child_widget.target_size.x;
			if (child_widget.margin.x < 0)		total_relative_size += child_widget.margin.x;
			if (child_widget.margin.z < 0)		total_relative_size += child_widget.margin.z;
		}
		else if (alignment == Vertical) {
			if (child_widget.target_size.y < 0)	total_relative_size += child_widget.target_size.y;
			if (child_widget.margin.y < 0)		total_relative_size += child_widget.margin.y;
			if (child_widget.margin.w < 0)		total_relative_size += child_widget.margin.w;
		}
	}

	return total_relative_size;
}

void widget::Stack::lay_widgets() {
	positions.resize(widgets.size());

	//std::vector<glm::vec2> positions();
	//std::vector<glm::vec2> sizes(widgets.size()); 

	glm::vec2 min_content_size		= compute_min_content_size();
	float total_relative_size_main	= compute_total_relative_size_main_axis();

	glm::vec2 current_position		= glm::vec2(padding.x, padding.y);

	if (element.size().x == 0)	element.size().x = min_content_size.x + padding.x + padding.z;
	if (element.size().y == 0)	element.size().y = min_content_size.y + padding.y + padding.w;

	for (int32_t i = 0; i < widgets.size(); i++) {
		Widget& child_widget = GUI::get().get_widget_data(this->widgets[i]);
		
		float total_relative_size_secondary = compute_total_relative_size_secondary_axis(i);
		glm::vec2 size_per_negative(0);

		if (alignment == Horizontal) {
			float child_known_horizontal_size = 0;
			if (child_widget.target_size.x > 0) child_known_horizontal_size += child_widget.target_size.y;
			if (child_widget.margin.y > 0)		child_known_horizontal_size += child_widget.margin.y;
			if (child_widget.margin.w > 0)		child_known_horizontal_size += child_widget.margin.w;

			size_per_negative.x = glm::max(element.size().x - min_content_size.x, 0.0f) / total_relative_size_main;
			size_per_negative.y = glm::max(element.size().y - child_known_horizontal_size, 0.0f) / total_relative_size_secondary;
		}
		else if (alignment == Vertical) {
			float child_known_verical_size = 0;
			if (child_widget.target_size.x > 0) child_known_verical_size += child_widget.target_size.x;
			if (child_widget.margin.x > 0)		child_known_verical_size += child_widget.margin.x;
			if (child_widget.margin.z > 0)		child_known_verical_size += child_widget.margin.z;

			size_per_negative.x = glm::max(element.size().x - child_known_verical_size, 0.0f) / total_relative_size_secondary;
			size_per_negative.y = glm::max(element.size().y - min_content_size.y, 0.0f) / total_relative_size_main;
		}

		glm::vec2 child_physical_size		= child_widget.target_size;
		glm::vec4 child_physical_margin		= child_widget.margin;

		if (child_physical_size.x < 0)		child_physical_size.x *= size_per_negative.x;
		if (child_physical_size.y < 0)		child_physical_size.y *= size_per_negative.y;

		if (child_physical_margin.x < 0)	child_physical_margin.x *= size_per_negative.x;
		if (child_physical_margin.y < 0)	child_physical_margin.y *= size_per_negative.y;
		if (child_physical_margin.z < 0)	child_physical_margin.z *= size_per_negative.x;
		if (child_physical_margin.w < 0)	child_physical_margin.w *= size_per_negative.y;

		Element& child_element = child_widget.get_element(child_physical_size);

		if (child_element.get_parent() != Element::null_element && child_element.get_parent() != element) {
			std::cout << "[GUI Error] Widgets cannot be part of multiple container Widgets" << std::endl;
			ASSERT(false);
		}

		child_element.set_parent(element);

		child_element.position() = current_position + glm::vec2(child_physical_margin.x, child_physical_margin.y);
		positions[i] = child_element.position();
		
		glm::vec2 total_child_size = child_element.size() + glm::vec2(child_physical_margin.x + child_physical_margin.z, child_physical_margin.y + child_physical_margin.w);
		if (alignment == Vertical) {
			current_position.y += total_child_size.y;
		}
		else if (alignment == Horizontal) {
			current_position.x += total_child_size.x;
		}
	}
}
