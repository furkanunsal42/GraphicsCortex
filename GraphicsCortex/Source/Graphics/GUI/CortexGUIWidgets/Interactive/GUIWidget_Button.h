#pragma once
#include "GUI/CortexGUI.h"

#include "CortexGUIWidgets/Container/GUIWidget_Grid.h"
#include "CortexGUIWidgets/Container/GUIWidget_Stack.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Rectangle.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Label.h"

namespace widget {

	class Button : public Grid {
	public:

		WidgetHandle<widget::Label> label = widget::create<widget::Label>();

		Button() {
			color = glm::vec4(1, 1, 1, 1);
			target_size = glm::vec2(0);
			padding = glm::vec4(20, 20, 20, 20);

			add_column(target_size.x);
			add_row(target_size.y);

			label->text = U"Button";
			label->font = 1;
			label->margin = glm::vec4(-1);
			label->text_color = glm::vec4(0, 0, 0, 1);
			label->z = z;

			add(label, 0, 0);
		}

		Element& get_element(glm::vec2 allocated_size) {
			
			set_column_size(target_size.x, 0);
			set_row_size(target_size.y, 0);

			return Grid::get_element(allocated_size);
		}

	private:
	};
}
