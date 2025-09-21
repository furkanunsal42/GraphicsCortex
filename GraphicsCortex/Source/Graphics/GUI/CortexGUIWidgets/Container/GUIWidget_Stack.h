#pragma once
#include "GUI/CortexGUIWidget.h"

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
			return element;
		}

		void push_back(Widget widgets);

	private:
		std::vector<Widget> widgets;
	};
}
