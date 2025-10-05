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
#include "CortexGUIWidgets/Window/GUIWidget_GUIWindow.h"

#include <chrono>

class CortexGUITest_Docking : public TestBench {
public:

	bool run() {

		using namespace std::chrono_literals;

		auto gui_window = widget::create<widget::GUIWindow>();
		gui_window->target_size = glm::vec2(1000, 1000);

		font_id font = FontBank::get().load_font("../GraphicsCortex/Fonts/Roboto-Regular.ttf", 64);



		gui_window->run();

		return true;
	}
};