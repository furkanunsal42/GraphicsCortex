#include "GUIWidget_Stack.h"
#include "Debuger.h"

void widget::Stack::push_back(widget_t widget)
{
	if (std::find(widgets.begin(), widgets.end(), widget) != widgets.end()) {
		std::cout << "[GUI Error] Stack::push_back() is called with a widget already part of it (id: " << widget << ")" << std::endl;
		ASSERT(false)
	}

	widgets.push_back(widget);
}

void widget::Stack::remove(widget_t widget)
{
	std::erase(widgets, widget);
}

glm::vec2 widget::Stack::compute_min_content_size() {
	glm::vec2 content_size(0);

	for (int32_t i = 0; i < widgets.size(); i++) {
		Widget& child_widget = GUI::get().get_widget_data(this->widgets[i]);

		if (alignment == Horizontal) {

			if (child_widget.target_size.x > 0)
				content_size.x += child_widget.target_size.x;
			else if (child_widget.target_size.x == 0)
				content_size.x += child_widget.get_element(glm::vec2(0)).size().x;
			
			if (child_widget.margin.x > 0)
				content_size.x += child_widget.margin.x;
			if (child_widget.margin.z > 0)
				content_size.x += child_widget.margin.z;

			float child_widget_known_height = 0;
			if (child_widget.target_size.y > 0)
				child_widget_known_height += child_widget.target_size.y;
			else if (child_widget.target_size.y == 0)
				child_widget_known_height += child_widget.get_element(glm::vec2(0)).size().y;

			if (child_widget.margin.y > 0)
				child_widget_known_height += child_widget.margin.y;
			if (child_widget.margin.w > 0)
				child_widget_known_height += child_widget.margin.w;

			if (child_widget_known_height > 0)
				content_size.y = glm::max(content_size.y, child_widget_known_height);

		}
		else if (alignment == Vertical) {

			float child_widget_known_width = 0;
			if (child_widget.target_size.x > 0)
				child_widget_known_width += child_widget.target_size.x;
			else if (child_widget.target_size.x == 0)
				child_widget_known_width += child_widget.get_element(glm::vec2(0)).size().x;

			if (child_widget.margin.x > 0)
				child_widget_known_width += child_widget.margin.x;
			if (child_widget.margin.z > 0)
				child_widget_known_width += child_widget.margin.z;

			if (child_widget_known_width > 0)
				content_size.x = glm::max(content_size.x, child_widget_known_width);

			if (child_widget.target_size.y > 0)
				content_size.y += child_widget.target_size.y;
			else if (child_widget.target_size.y == 0)
				content_size.y += child_widget.get_element(glm::vec2(0)).size().y;
			
			if (child_widget.margin.y > 0)
				content_size.y += child_widget.margin.y;
			if (child_widget.margin.w > 0)
				content_size.y += child_widget.margin.w;
		}
	}
	return content_size;
}

glm::vec2 widget::Stack::compute_relative_divison_size(const glm::vec2& min_content_size)
{
	glm::vec2 total_relative_sizes(0);

	for (int32_t i = 0; i < widgets.size(); i++) {
		Widget& child_widget = GUI::get().get_widget_data(this->widgets[i]);

		glm::vec2 child_relative_sizes(0);

		if (child_widget.target_size.x < 0)
			child_relative_sizes.x += child_widget.target_size.x;
		if (child_widget.target_size.y < 0)
			child_relative_sizes.y += child_widget.target_size.y;

		if (child_widget.margin.x < 0)
			child_relative_sizes.x += child_widget.margin.x;
		if (child_widget.margin.y < 0)
			child_relative_sizes.y += child_widget.margin.y;
		if (child_widget.margin.z < 0)
			child_relative_sizes.x += child_widget.margin.z;
		if (child_widget.margin.w < 0)
			child_relative_sizes.y += child_widget.margin.w;

		if (alignment == Horizontal) {
			if (child_relative_sizes.x < 0)
				total_relative_sizes.x += child_relative_sizes.x;
			if (child_relative_sizes.y < 0)
				total_relative_sizes.y = glm::min(total_relative_sizes.y, child_relative_sizes.y);
		}
		else if (alignment == Vertical) {
			if (child_relative_sizes.x < 0)
				total_relative_sizes.x = glm::min(total_relative_sizes.x, child_relative_sizes.x);
			if (child_relative_sizes.y < 0)
				total_relative_sizes.y += child_relative_sizes.y;
		}
	}

	return total_relative_sizes;
}

void widget::Stack::lay_widgets() {
	std::vector<glm::vec2> positions(widgets.size());
	std::vector<glm::vec2> sizes(widgets.size());

	glm::vec2 min_content_size = compute_min_content_size();
	glm::vec2 total_relative_sizes = compute_relative_divison_size(min_content_size);

	glm::vec2 current_position(0);
	current_position += glm::vec2(padding.x, padding.y);

	for (int32_t i = 0; i < widgets.size(); i++) {
		Widget& child_widget = GUI::get().get_widget_data(this->widgets[i]);

		glm::vec2 size_per_negative(0);

		if (alignment == Horizontal) {
			size_per_negative.x = (glm::max(element.size().x - min_content_size.x, 0.0f)) / total_relative_sizes.x;
			if (child_widget.target_size.y > 0)
				size_per_negative.y = (glm::max(element.size().y - child_widget.target_size.y, 0.0f)) / total_relative_sizes.y;
			else 
				size_per_negative.y = element.size().y / total_relative_sizes.y;
		}
		else if (alignment == Vertical) {
			if (child_widget.target_size.x > 0)
				size_per_negative.x = (glm::max(element.size().x - child_widget.target_size.x, 0.0f)) / total_relative_sizes.x;
			else
				size_per_negative.x = element.size().x / total_relative_sizes.x;

			size_per_negative.y = (glm::max(element.size().y - min_content_size.y, 0.0f)) / total_relative_sizes.y;
		}

		glm::vec2 child_physical_size = child_widget.target_size;
		glm::vec4 child_physical_margin = child_widget.margin;

		if (child_physical_size.x < 0)
			child_physical_size.x *= size_per_negative.x;
		if (child_physical_size.y < 0)
			child_physical_size.y *= size_per_negative.y;

		if (child_physical_margin.x < 0)
			child_physical_margin.x *= size_per_negative.x;
		if (child_physical_margin.y < 0)
			child_physical_margin.y *= size_per_negative.y;
		if (child_physical_margin.z < 0)
			child_physical_margin.z *= size_per_negative.x;
		if (child_physical_margin.w < 0)
			child_physical_margin.w *= size_per_negative.y;

		//std::cout << child_physical_margin.x << ", " << child_physical_margin.y << ", " << child_physical_margin.z << ", " << child_physical_margin.w << std::endl;

		Element& child_element = child_widget.get_element(child_physical_size);

		if (child_element.get_parent() != Element::null_element && child_element.get_parent() != element) {
			std::cout << "[GUI Error] Widgets cannot be part of multiple container Widgets" << std::endl;
			ASSERT(false);
		}

		child_element.set_parent(element);

		child_element.position() = current_position + glm::vec2(child_physical_margin.x, child_physical_margin.y);
		glm::vec2 total_child_size = child_element.size() + glm::vec2(child_physical_margin.x + child_physical_margin.z, child_physical_margin.y + child_physical_margin.w);
		if (alignment == Vertical) {
			current_position.y += total_child_size.y;
		}
		else if (alignment == Horizontal) {
			current_position.x += total_child_size.x;
		}
	}

	if (target_size.x == 0)
		element.size().x = min_content_size.x + padding.x + padding.z;

	if (target_size.y == 0)
		element.size().y = min_content_size.y + padding.y + padding.w;
}
