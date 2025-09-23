#pragma once
#include "GUI/CortexGUI.h"

namespace widget {

	class Stack : public Widget {
	public:

		Stack() {
			color = glm::vec4(0, 0, 0, 0);
		}

		enum Alignment {
			Vertical,
			Horizontal
		};

		enum Positioning {
			Begin,
			End,
			Center,
		};

		Alignment alignment = Horizontal;
		Positioning positioning = Begin;

		Element& get_element(glm::vec2 allocated_size) {
			apply_properties_to_element(element, allocated_size);
			lay_widgets();
			return element;
		}

		void push_back(widget_t widget);
		void remove(widget_t widget);

	private:

		glm::vec2 compute_min_content_size();
		float compute_total_relative_size_main_axis();
		float compute_total_relative_size_secondary_axis(int32_t widget_index);

		void lay_widgets();

		std::vector<widget_t> widgets;
	};
}
