#pragma once
#include "GUI/CortexGUI.h"

namespace widget {

	class DockArea : public Widget {
	public:

		DockArea();

		enum Seperation {
			Horizontal,
			Vertical
		};

		void add(widget_t widget, int32_t region_id, Seperation seperation);
		void remove(widget_t widget);

		Element& get_element(glm::vec2 allocated_size);
		void poll_events(glm::vec2 absolute_position) override;

	private:

		void lay_widgets();

		static constexpr int32_t invalid_region = -1;
		struct region_info {
		public:

			int32_t region_id	= invalid_region;
			int32_t child0		= invalid_region;
			int32_t child1		= invalid_region;
			Seperation seperation = Vertical;

			widget_t widget		= invalid_widget;
			glm::vec2 widget_position = glm::vec2(0);

			Element region_element = Element::null_element;
		};

		std::map<int32_t, region_info> id_to_region;
	};
}
