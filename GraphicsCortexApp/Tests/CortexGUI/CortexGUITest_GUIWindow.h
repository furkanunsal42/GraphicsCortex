#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "GUI/CortexGUI.h"
#include "Tools/GraphicsOperation/GraphicsOperation.h"

#include "CortexGUIWidgets/Basic/GUIWidget_Rectangle.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Label.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Image.h"

#include "CortexGUIWidgets/Container/GUIWidget_Stack.h"
#include "CortexGUIWidgets/Container/GUIWidget_Grid.h"

#include "CortexGUIWidgets/Interactive/GUIWidget_Button.h"
#include "CortexGUIWidgets/Interactive/GUIWidget_TextInput.h"
#include "CortexGUIWidgets/Interactive/GUIWidget_Scroll.h"
#include "CortexGUIWindow.h"

#include <chrono>

class CortexGUITest_GUIWindow : public TestBench {
public:

	bool run() {

		using namespace std::chrono_literals;

		auto gui_window = GUIWindow();
		//gui_window.grid->target_size = glm::vec2(0, 0);
		
		font_id font = FontBank::get().load_font("../GraphicsCortex/Fonts/Roboto-Regular.ttf", 64);
		
		auto stack = widget::create<widget::Stack>();
		stack->alignment = widget::Stack::Vertical;
		//gui_window.grid->add(stack, 0, 0);
		
		auto text_input = widget::create<widget::TextInput>();
		text_input->margin = glm::vec4(10);
		stack->add(text_input);
		
		auto button = widget::create<widget::Button>();
		button->margin = glm::vec4(-1, 10, 10, 10);
		button->label->text = U"Clear";
		button->events.subscribe([&](GUIEvent e, glm::vec2 absolute_position, glm::vec2 cursor_position) {
			if (e == GUIEvent::Clicked)
				text_input->text = U"";
			});
		stack->add(button);
		
		auto scroll = widget::create<widget::Scroll>();
		scroll->margin = glm::vec4(0);
		stack->add(scroll);

		gui_window.run();
		
		return true;
	}
};