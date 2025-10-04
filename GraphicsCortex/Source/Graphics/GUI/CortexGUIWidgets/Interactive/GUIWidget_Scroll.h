#pragma once
#include "GUI/CortexGUI.h"

#include "CortexGUIWidgets/Container/GUIWidget_Grid.h"
#include "CortexGUIWidgets/Container/GUIWidget_Stack.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Rectangle.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Label.h"

namespace widget {

	class Scroll : public Grid {
	public:

		WidgetHandle<widget::Rectangle> grip = widget::create<widget::Rectangle>();
		WidgetHandle<widget::Button> back_button	= widget::create<widget::Button>();
		WidgetHandle<widget::Button> forward_button	= widget::create<widget::Button>();

		bool holding = false;
		float grip_holding_position_x = 0;

		Scroll() {
			using namespace std::chrono_literals;

			back_button->target_size = glm::vec2(30);
			forward_button->target_size = glm::vec2(30);

			add_column(back_button->target_size.x);
			add_column(0);
			add_column(forward_button->target_size.x);
			add_row(target_size.y);

			color = glm::vec4(0.91f, 0.91f, 0.93, 1);
			target_size = glm::vec2(-1, 30);

			grip->margin = glm::vec4(0, -1, 0, -1);
			grip->target_size = glm::vec2(60, 20);
			grip->z = z;

			//grip->border_rounding = glm::vec4(10);
			grip->color = glm::vec4(0.76, 0.76, 0.78, 1);
			grip->on_hover_color = glm::vec4(0.70, 0.70, 0.75, 1);
			grip->on_hover_color_transition = 50ms;

			grip->on_hold_color = glm::vec4(0.70, 0.70, 0.75, 1);
			grip->on_hold_color_transition = 50ms;

			add(grip, 0, 1);

			back_button->color = color;
			back_button->padding = glm::vec4(0);
			back_button->label->text = U" < ";
			back_button->border_color0 = glm::vec4(0);
			back_button->border_color1 = glm::vec4(0);
			back_button->border_color2 = glm::vec4(0);
			back_button->border_color3 = glm::vec4(0);
			back_button->border_thickness = glm::vec4(0);
			back_button->border_rounding = glm::vec4(0);
			back_button->on_hover_color = glm::vec4(0.85, 0.85, 0.88, 1);
			back_button->on_hover_color_transition = 10ms;
			back_button->on_hold_color = glm::vec4(0.80, 0.80, 0.85, 1);
			back_button->on_hold_color_transition = 10ms;

			forward_button->color = color;
			forward_button->padding = glm::vec4(0);
			forward_button->label->text = U" > ";
			forward_button->border_color0 = glm::vec4(0);
			forward_button->border_color1 = glm::vec4(0);
			forward_button->border_color2 = glm::vec4(0);
			forward_button->border_color3 = glm::vec4(0);
			forward_button->border_thickness = glm::vec4(0);
			forward_button->border_rounding = glm::vec4(0);
			forward_button->on_hover_color = glm::vec4(0.85, 0.85, 0.88, 1);
			forward_button->on_hover_color_transition = 10ms;
			forward_button->on_hold_color = glm::vec4(0.80, 0.80, 0.85, 1);
			forward_button->on_hold_color_transition = 10ms;


			add(back_button, 0, 0);
			add(forward_button, 0, 2);

			events.subscribe([&](GUIEvent event, glm::vec2 absolute_position, glm::vec2 cursor_position) {
				float local_cursor_position = cursor_position.x - absolute_position.x;
				float grip_cursor_position = local_cursor_position - back_button->target_size.x - grip->margin.x;
				
				if (event == GUIEvent::HoldBegin) {
					if (local_cursor_position < back_button->target_size.x ||
						local_cursor_position > element.size().x - back_button->target_size.x
						)
						return;

					holding = true;
					grip_holding_position_x = grip_cursor_position;
				}
				if (event == GUIEvent::HoldEnd) {
					holding = false;
					grip_holding_position_x = 0;
				}
				});
		}

		Element& get_element(glm::vec2 allocated_size) override {

			set_column_size(back_button->target_size.x, 0);
			set_column_size(allocated_size.x - (back_button->target_size.x + forward_button->target_size.x), 1);
			set_column_size(forward_button->target_size.x, 2);
			set_row_size(allocated_size.y, 0);

			return Grid::get_element(allocated_size);
		}

		void poll_events(glm::vec2 absolute_position) override {

			if (holding) {
				if (grip_holding_position_x > grip->target_size.x ||
					grip_holding_position_x < 0
				) {
					grip_holding_position_x = grip->target_size.x / 2;
				}

				glm::vec2 cursor_position = GUI::get().get_window()->get_cursor_position();
				glm::vec2 cursor_position_local = cursor_position - absolute_position - glm::vec2(back_button->target_size.x, 0);

				grip->margin.x = cursor_position_local.x - grip_holding_position_x;
				grip->margin.x = std::max(grip->margin.x, 0.0f);
				grip->margin.x = std::min(grip->margin.x, element.size().x - (back_button->target_size.x + forward_button->target_size.x) - grip->target_size.x);
			}

			Grid::poll_events(absolute_position);
		}

	private:
	};
}
