#include "CortexGUIWidget.h"
#include "CortexGUI.h"

const Widget Widget::null_widget = Widget(nullptr, invalid_widget);

Widget::~Widget()
{
	GUI::get()._release_widget(id);
}

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


WidgetStyle& Widget::style() {
	if (!GUI::get().does_widget_exist(*this)) {
		std::cout << "[GUI Error] Widget::style() is called on invalid Widget" << std::endl;
		ASSERT(false);
	}

	GUI::get().widgets[id].vab_properties_changed = true;
	
	return GUI::get().widgets[id].style;
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

std::string& Widget::text()
{
	if (!GUI::get().does_widget_exist(*this)) {
		std::cout << "[GUI Error] Widget::z() is called on invalid Widget" << std::endl;
		ASSERT(false);
	}
	
	GUI::get().widgets[id].text_changed = true;

	return GUI::get().widgets[id].text;
}
