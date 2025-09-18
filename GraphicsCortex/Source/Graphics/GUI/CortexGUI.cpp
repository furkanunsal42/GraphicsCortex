#include "CortexGUI.h"

#include <queue>

#include "PrimitiveRenderer.h"

GUI& GUI::get()
{
	static GUI gui;
	return gui;
}

ImmediateRenderer& GUI::get_immediate_renderer()
{
	if (immediate_renderer == nullptr)
		immediate_renderer = std::make_shared<ImmediateRenderer>(1024);
	return *immediate_renderer;
}

Widget GUI::create_widget()
{
	widget_id id = _create_widget(invalid_widget);
	return Widget(attached_window_handle, id);
}

Widget GUI::create_widget(Widget& parent_widget)
{
	widget_id id = _create_widget(parent_widget.id);
	return Widget(attached_window_handle, id);
}

bool GUI::does_widget_exist(Widget& widget)
{
	return _does_widget_exist(widget.id) && widget.owner_gui_identifier == attached_window_handle;
}

void GUI::render(Widget& root_widget)
{
	_render(root_widget.id);
}

GUI::GUI()
{
}

widget_id GUI::_generate_widget_id()
{
	widget_id id = next_widget_id;
	next_widget_id++;
	return id;
}

widget_id GUI::_create_widget(widget_id parent_id)
{
	if (!_does_widget_exist(parent_id) && parent_id != invalid_widget) {
		std::cout << "[GUI Error] GUI::_create_widget() is called with improper parent_id: " << parent_id << std::endl;
		ASSERT(false);
	}

	widget_id id = _generate_widget_id();
	
	WidgetInfo info;
	info.id = id;
	info.parent_id = parent_id;

	widgets[id] = info;
	widgets[parent_id].children.push_back(id);
	
	return id;
}

void GUI::_release_widget(widget_id id)
{
	if (!_does_widget_exist(id))
		return;

	widget_id parent = widgets[id].parent_id;
	widgets.erase(id);

	if (_does_widget_exist(parent)) {
		std::vector<widget_id>& children_list = widgets[parent].children;
		auto iterator = std::find(children_list.begin(), children_list.end(), id);
		widgets[parent].children.erase(iterator);
	}
}

bool GUI::_does_widget_exist(widget_id id)
{
	return widgets.find(id) != widgets.end();
}

void GUI::_traverse_children(widget_id root_id, std::function<void(widget_id, glm::vec2)> lambda)
{
	if (!_does_widget_exist(root_id)) {
		std::cout << "[GUI Error] GUI::_traverse_children() is called with invalid root_id" << std::endl;
		ASSERT(false);
	}
	
	std::queue<widget_id> nodes;
	std::queue<glm::vec2> positions;
	nodes.push(root_id);
	positions.push(widgets[root_id].position);

	while (nodes.size() != 0) {
		
		widget_id node = nodes.front();
		glm::vec2 position = positions.front();

		lambda(node, position);

		nodes.pop();
		positions.pop();

		for (widget_id child : widgets[node].children) {
			nodes.push(child);
			positions.push(position + widgets[child].position);
		}
	}
}

void GUI::_traverse_parents(widget_id root_id, std::function<void(widget_id)> lambda)
{
	if (!_does_widget_exist(root_id)) {
		std::cout << "[GUI Error] GUI::_traverse_parents() is called with invalid root_id" << std::endl;
		ASSERT(false);
	}


	widget_id node = root_id;

	while (node != invalid_widget) {
		lambda(node);
		node = widgets[node].parent_id;
	}
}

void GUI::_render(widget_id id)
{
	if (!_does_widget_exist(id)) {
		std::cout << "[GUI Error] GUI::_render() is called with improper widget_id : " << id << std::endl;
		ASSERT(false);
	}

	ImmediateRenderer& immediate = get_immediate_renderer();
	glm::vec2 total_viewport = primitive_renderer::get_viewport_size();

	glm::vec4 parent_viewport = glm::vec4(
		(widgets[id].position.x / 2.0f + 0.5) * total_viewport.x,
		(widgets[id].position.y / 2.0f + 0.5) * total_viewport.y,
		(widgets[id].actual_size.x / 2.0f + 0.5) * total_viewport.x,
		(widgets[id].actual_size.y / 2.0f + 0.5) * total_viewport.y
	);
	
	_traverse_children(id, [&](widget_id child_id, glm::vec2 merged_position) {
		
		int32_t z = widgets[child_id].z;

		immediate.set_fill_color(widgets[child_id].style.color);
		immediate.draw_quad(
			glm::vec3(merged_position + glm::vec2(0, 0),								-z / 1024.0f * 32),
			glm::vec3(merged_position + glm::vec2(widgets[child_id].actual_size.x, 0),	-z / 1024.0f * 32),
			glm::vec3(merged_position + glm::vec2(widgets[child_id].actual_size),		-z / 1024.0f * 32),
			glm::vec3(merged_position + glm::vec2(0, widgets[child_id].actual_size.y),	-z / 1024.0f * 32)
		);

		RenderParameters params(true);
		params.scissor_test = true;

		//parent_viewport = glm::vec4(
		//	1920/2, 1080/2, 1920/4, 1080/4
		//	//(widgets[child_id].position.x / 2.0f + 0.5) * total_viewport.x,
		//	//(widgets[child_id].position.y / 2.0f + 0.5) * total_viewport.y,
		//	//(widgets[child_id].actual_size.x / 2.0f + 0.5) * total_viewport.x,
		//	//(widgets[child_id].actual_size.y / 2.0f + 0.5) * total_viewport.y
		//);

		params.scissor_viewport = parent_viewport;
		immediate.render(params);

		});
}

Widget::~Widget()
{
	GUI::get()._release_widget(id);
}

Widget::Widget(void* owner_gui_identifier, widget_id id) :
	owner_gui_identifier(owner_gui_identifier), id(id)
{
	
}

Widget Widget::create_child() {
	if (!GUI::get().does_widget_exist(*this)) {
		std::cout << "[GUI Error] Widget::create_child() is called on invalid Widget" << std::endl;
		ASSERT(false);
	}

	return GUI::get().create_widget(*this);
}


WidgetStyle& Widget::style(){
	if (!GUI::get().does_widget_exist(*this)) {
		std::cout << "[GUI Error] Widget::style() is called on invalid Widget" << std::endl;
		ASSERT(false);
	}

	return GUI::get().widgets[id].style;
}

glm::vec2& Widget::position(){
	if (!GUI::get().does_widget_exist(*this)) {
		std::cout << "[GUI Error] Widget::position() is called on invalid Widget" << std::endl;
		ASSERT(false);
	}

	return GUI::get().widgets[id].position;
}

glm::vec2& Widget::size(){
	if (!GUI::get().does_widget_exist(*this)) {
		std::cout << "[GUI Error] Widget::size() is called on invalid Widget" << std::endl;
		ASSERT(false);
	}

	return GUI::get().widgets[id].actual_size;
}

int32_t& Widget::z(){
	if (!GUI::get().does_widget_exist(*this)) {
		std::cout << "[GUI Error] Widget::z() is called on invalid Widget" << std::endl;
		ASSERT(false);
	}
	
	return GUI::get().widgets[id].z;
}

std::string& Widget::text()
{
	if (!GUI::get().does_widget_exist(*this)) {
		std::cout << "[GUI Error] Widget::z() is called on invalid Widget" << std::endl;
		ASSERT(false);
	}

	return GUI::get().widgets[id].text;
}
