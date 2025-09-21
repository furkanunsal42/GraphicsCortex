#pragma once
#include "GUI/CortexGUIWidget.h"

namespace widget {

	class Rectangle : public Widget {
	public:

		Element& get_widget() {
			apply_properties_to_element(element);
			return element;
		}

	private:
	};
}
