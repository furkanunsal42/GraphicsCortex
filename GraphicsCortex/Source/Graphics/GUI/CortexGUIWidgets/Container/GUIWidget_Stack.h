#pragma once
#include "GUI/CortexGUIWidget.h"
#include "GUI/CortexGUI.h"

namespace widget {

	class Stack : public Widget {
	public:

		enum Alignment {
			Vertical,
			Horizontal
		};

		enum Positioning {
			Begin,
			End,
			Center,
		};

		Alignment allignment = Horizontal;
		Positioning positioning = Begin;

		Element& get_element() {
			apply_properties_to_element(element);
			lay_widgets();
			return element;
		}

		void push_back(widget_t widget);

	private:
		void lay_widgets() {
			std::vector<glm::vec2> positions(widgets.size());
			std::vector<glm::vec2> sizes(widgets.size());
			
			glm::vec2 current_position(0);
			glm::vec2 current_size(0);

			for (int32_t i = 0; i < widgets.size(); i++) {
				Widget& data = GUI::get().get_widget_data(this->widgets[i]);
				
				Element& child_element = data.get_element();
				child_element.set_parent(element);

				child_element.position() = current_position;
				
				current_size.x = glm::max(current_size.x, child_element.size().x);
				current_size.y = current_position.y + child_element.size().y;
				
				current_position.y += child_element.size().y;
			}

			element.size() = current_size;
		}

		std::vector<widget_t> widgets;
	};
}
