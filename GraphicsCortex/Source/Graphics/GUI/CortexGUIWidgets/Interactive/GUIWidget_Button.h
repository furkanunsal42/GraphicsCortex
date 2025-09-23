#pragma once
#include "GUI/CortexGUI.h"

#include "CortexGUIWidgets/Container/GUIWidget_Stack.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Rectangle.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Label.h"

namespace widget {

	class Button : public Stack {
	public:

		WidgetHandle<widget::Label> label			= widget::create<widget::Label>();
		WidgetHandle<widget::Rectangle> rectangle	= widget::create<widget::Rectangle>();

		Button() {
			color = glm::vec4(1, 1, 1, 1);

			push_back(label);
			push_back(rectangle);
		}

	private:
	};
}
