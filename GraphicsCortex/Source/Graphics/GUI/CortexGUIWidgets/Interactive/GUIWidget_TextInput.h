#pragma once
#include "GUI/CortexGUI.h"

#include "CortexGUIWidgets/Container/GUIWidget_Grid.h"
#include "CortexGUIWidgets/Container/GUIWidget_Stack.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Rectangle.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Label.h"

namespace widget {

	class TextInput : public Grid {
	public:

		WidgetHandle<widget::Label> label = widget::create<widget::Label>();
		WidgetHandle<widget::Rectangle> selector = widget::create<widget::Rectangle>();

		glm::vec4 on_focus_border_color = glm::vec4(0, 0, 0, 1);
		int32_t cursor_position = 0;
		bool focused = false;

		TextInput() {
			color = glm::vec4(1, 1, 1, 1);
			border_thickness = glm::vec4(2);
			border_color0 = glm::vec4(0.82, 0.82, 0.82, 1);
			border_color1 = glm::vec4(0.82, 0.82, 0.82, 1);
			border_color2 = glm::vec4(0.74, 0.74, 0.74, 1);
			border_color3 = glm::vec4(0.82, 0.82, 0.82, 1);
			target_size = glm::vec2(200, 40);

			add_column(target_size.x);
			add_row(target_size.y);

			label->text = U"Enter Text Here";
			label->font = 1;
			label->margin = glm::vec4(8, -1, -1, -1);
			label->text_color = glm::vec4(0, 0, 0, 1);
			label->z = z;
			add(label, 0, 0);

			selector->color = glm::vec4(0, 0, 0, 1);
			selector->margin = glm::vec4(0, -1, 0, -1);

			add(selector, 0, 0);

			events.subscribe([&](GUIEvent e) {
				if (e == GUIEvent::Clicked)
					this->focused = true;
				});
			
		}

		Element& get_element(glm::vec2 allocated_size) {

			if (label->get_glyph_count() > 3)
				selector->margin.x = label->get_glyph_position(3).x + label->margin.x - 2;

			selector->target_size = glm::vec2(2, std::min(label->text_height + 8, allocated_size.y));
			selector->color.a = focused ? 1 : 0;

			set_column_size(target_size.x, 0);
			set_row_size(target_size.y, 0);

			return Grid::get_element(allocated_size);
		}

	private:
	};
}
