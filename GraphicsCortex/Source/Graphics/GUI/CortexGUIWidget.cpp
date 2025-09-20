#include "CortexGUIWidget.h"
#include "CortexGUI.h"
#include "WindowBoundGlobalResources.h"

const Widget Widget::null_widget = Widget(nullptr, invalid_widget);

//Widget::~Widget()
//{
//	if (id == invalid_widget)
//		return;
//
//	if (active_global_resources == nullptr || active_global_resources->GUI == nullptr)
//		return;
//
//	GUI::get()._release_widget(id);
//}

Widget::Widget(void* owner_gui_identifier, widget_t id) :
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

void Widget::set_parent(Widget new_parent)
{
	if (!GUI::get().does_widget_exist(*this))
		return;

	GUI::get().set_widget_parent(*this, new_parent);
}

Widget Widget::get_parent() {
	if (!GUI::get().does_widget_exist(*this))
		return Widget::null_widget;

	return Widget(owner_gui_identifier, GUI::get().widgets[id].parent_id);
}

glm::vec2& Widget::texcoord_min()
{
	if (!GUI::get().does_widget_exist(*this)) {
		std::cout << "[GUI Error] Widget::texcoord_min() is called on invalid Widget" << std::endl;
		ASSERT(false);
	}

	GUI::get().widgets[id].vab_properties_changed = true;

	return GUI::get().widgets[id].texcoord_min;
}

glm::vec2& Widget::texcoord_max()
{
	if (!GUI::get().does_widget_exist(*this)) {
		std::cout << "[GUI Error] Widget::texcoord_max() is called on invalid Widget" << std::endl;
		ASSERT(false);
	}

	GUI::get().widgets[id].vab_properties_changed = true;

	return GUI::get().widgets[id].texcoord_max;
}

glm::vec4& Widget::color()
{
	if (!GUI::get().does_widget_exist(*this)) {
		std::cout << "[GUI Error] Widget::color() is called on invalid Widget" << std::endl;
		ASSERT(false);
	}

	GUI::get().widgets[id].vab_properties_changed = true;

	return GUI::get().widgets[id].color;
}

glm::vec4& Widget::border_color()
{
	if (!GUI::get().does_widget_exist(*this)) {
		std::cout << "[GUI Error] Widget::border_color() is called on invalid Widget" << std::endl;
		ASSERT(false);
	}

	GUI::get().widgets[id].vab_properties_changed = true;

	return GUI::get().widgets[id].border_color;
}

glm::vec4& Widget::border_thickness()
{
	if (!GUI::get().does_widget_exist(*this)) {
		std::cout << "[GUI Error] Widget::border_thickness() is called on invalid Widget" << std::endl;
		ASSERT(false);
	}

	GUI::get().widgets[id].vab_properties_changed = true;

	return GUI::get().widgets[id].border_thickness;

}

glm::vec2& Widget::position() {
	if (!GUI::get().does_widget_exist(*this)) {
		std::cout << "[GUI Error] Widget::position() is called on invalid Widget" << std::endl;
		ASSERT(false);
	}

	GUI::get().widgets[id].vab_properties_changed = true;

	return GUI::get().widgets[id].position;
}

glm::vec2& Widget::size() {
	if (!GUI::get().does_widget_exist(*this)) {
		std::cout << "[GUI Error] Widget::size() is called on invalid Widget" << std::endl;
		ASSERT(false);
	}
	
	GUI::get().widgets[id].vab_properties_changed = true;

	return GUI::get().widgets[id].size;
}

int32_t& Widget::z() {
	if (!GUI::get().does_widget_exist(*this)) {
		std::cout << "[GUI Error] Widget::z() is called on invalid Widget" << std::endl;
		ASSERT(false);
	}

	GUI::get().widgets[id].vab_properties_changed = true;

	return GUI::get().widgets[id].z;
}

std::shared_ptr<Texture2D>& Widget::texture()
{
	if (!GUI::get().does_widget_exist(*this)) {
		std::cout << "[GUI Error] Widget::texture() is called on invalid Widget" << std::endl;
		ASSERT(false);
	}

	GUI::get().widgets[id].texture_changed = true;

	return GUI::get().widgets[id].texture;
}

bool operator==(const Widget& a, const Widget& b)
{
	bool same_id = a.id == b.id;
	bool same_gui = a.owner_gui_identifier == b.owner_gui_identifier;
	bool both_are_null = a.id == invalid_widget && b.id == invalid_widget;

	return (same_gui && same_id) || both_are_null;
}
