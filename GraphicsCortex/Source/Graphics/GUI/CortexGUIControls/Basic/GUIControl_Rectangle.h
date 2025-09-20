#pragma once
#include "GUI/CortexGUIControl.h"

namespace gui_controls {

	class Rectangle : public GUIControl {
	public:

		Widget& get_widget() {
			apply_properties_to_widget(widget);
			return widget;
		}

	private:
	};
}
