#pragma once
#include "GUI/CortexGUI.h"

#include "CortexGUIWidgets/Container/GUIWidget_Grid.h"
#include "CortexGUIWidgets/Container/GUIWidget_Stack.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Rectangle.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Label.h"

namespace widget {

	class Button : public Widget {
	public:

		WidgetHandle<widget::Grid> grid				= widget::create<widget::Grid>();
		WidgetHandle<widget::Label> label			= widget::create<widget::Label>();
		WidgetHandle<widget::Rectangle> rectangle	= widget::create<widget::Rectangle>();

		Button() {
			target_size = glm::vec2(100, 40);
		}

		Element& get_element(glm::vec2 allocated_size) {

			label->font = 1;
			label->text = "Button";
			label->text_color = glm::vec4(0, 0, 0, 1);
			label->margin = glm::vec4(-1);
			label->z = z;
			
			rectangle->color = glm::vec4(1, 1, 1, 1);
			rectangle->z = z;
			rectangle->margin = glm::vec4(0);
			rectangle->target_size = glm::vec2(-1);
			
			grid->add_column(target_size.x);
			grid->add_row(target_size.y);
			
			grid->add(label, 0, 0);
			grid->add(rectangle, 0, 0);

			return grid->get_element(allocated_size);
		}

	private:
	};
}
