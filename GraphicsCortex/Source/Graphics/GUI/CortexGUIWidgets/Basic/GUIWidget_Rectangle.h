#pragma once
#include "GUI/CortexGUI.h"

namespace widget {

	class Rectangle : public Widget {
	public:

		Element& get_element() {
			apply_properties_to_element(element);
			return element;
		}

	private:
	};
}
