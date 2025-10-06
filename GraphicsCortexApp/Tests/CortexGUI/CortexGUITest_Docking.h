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

class CortexGUITest_Docking : public TestBench {
public:

	bool run() {

		using namespace std::chrono_literals;

		auto gui_window = GUIWindow();
		auto gui_window2 = GUIWindow(gui_window);

		font_id font = FontBank::get().load_font("../GraphicsCortex/Fonts/Roboto-Regular.ttf", 64);

		gui_window.run();
		gui_window2.run();

		return true;
	}
};