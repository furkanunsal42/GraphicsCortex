#pragma once
#include "GUI/CortexGUIControl.h"

namespace gui_controls {

	class Stack : public GUIControl {
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

		Widget& get_widget() {
			apply_properties_to_widget(widget);
			return widget;
		}

		void push_back(GUIControl control);

	private:
		std::vector<GUIControl> controls;
	};
}
