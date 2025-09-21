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
