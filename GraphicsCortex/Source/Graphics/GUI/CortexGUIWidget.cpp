#include "CortexGUIWidget.h"
#include "CortexGUI.h"

Widget::Widget() 
{
	element = GUI::get().create_element();
}

void Widget::apply_properties_to_element(Element& element, glm::vec2 allocated_size)
{
	element.size() = allocated_size;
	element.color() = color;
	element.z() = z;
};