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

	element.border_thickness() = border_thickness;
	element.border_color0() = border_color0;
	element.border_color1() = border_color1;
	element.border_color2() = border_color2;
	element.border_color3() = border_color3;
	
	element.shadow_color() = shadow_color;
	element.shadow_thickness() = shadow_thickness;
};