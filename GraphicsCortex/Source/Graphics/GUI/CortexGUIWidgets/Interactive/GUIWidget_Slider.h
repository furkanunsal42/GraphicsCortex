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
			using namespace std::chrono_literals;

			add_column(target_size.x);
			add_row(target_size.y);

			color = glm::vec4(0.93, 0.93, 0.93, 1);
			target_size = glm::vec2(0);
			padding = glm::vec4(43, 13, 43, 13);

			border_thickness = glm::vec4(1);
			border_color0 = glm::vec4(0.0, 0.47, 0.84, 1);
			border_color1 = glm::vec4(0.0, 0.47, 0.84, 1);
			border_color2 = glm::vec4(0.0, 0.47, 0.84, 1);
			border_color3 = glm::vec4(0.0, 0.47, 0.84, 1);

			on_hover_border_color0 = glm::vec4(0.0, 0.47, 0.84, 1);
			on_hover_border_color1 = glm::vec4(0.0, 0.47, 0.84, 1);
			on_hover_border_color2 = glm::vec4(0.0, 0.47, 0.84, 1);
			on_hover_border_color3 = glm::vec4(0.0, 0.47, 0.84, 1);
			on_hover_border_color0_transition = 250ms;
			on_hover_border_color1_transition = 250ms;
			on_hover_border_color2_transition = 250ms;
			on_hover_border_color3_transition = 250ms;
			on_hover_color = glm::vec4(0.9, 0.95, 0.98, 1);
			on_hover_color_transition = 250ms;
			on_hover_border_thickness = glm::vec4(1);
			on_hover_border_thickness_transition = 250ms;

			on_hold_color = glm::vec4(0.8, 0.89, 0.97, 1);
			on_hold_color_transition = 50ms;

			label->text = U"Button";
			label->font = 1;
			label->margin = glm::vec4(-1);
			label->text_color = glm::vec4(0.2, 0.2, 0.2, 1);
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
