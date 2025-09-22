#pragma once
#include "GUI/CortexGUI.h"

namespace widget {

	class Rectangle : public Widget {
	public:

		Element& get_element(glm::vec2 allocated_size) {
			apply_properties_to_element(element, allocated_size);
			return element;
		}

	private:
	};
}
