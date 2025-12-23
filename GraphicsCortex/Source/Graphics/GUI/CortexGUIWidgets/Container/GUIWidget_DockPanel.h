#pragma once
#include "GUI/CortexGUI.h"
#include "GUI/CortexGUIWidgets/Container/GUIWidget_Grid.h"

#include "GUI/CortexGUIWindowHierarchy.h"

namespace widget {

	class DockPanel : public Grid {
	public:

		guiwindow_t window;

		DockPanel();

		Element& get_element(glm::vec2 allocated_size);
		void poll_events(glm::vec2 absolute_position) override;



	private:
	};
}
