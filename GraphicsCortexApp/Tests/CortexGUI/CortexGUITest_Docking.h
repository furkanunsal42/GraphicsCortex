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

#include "CortexGUIWindowHierarchy.h"

#include <chrono>

class CortexGUITest_Docking : public TestBench {
public:

	bool run() {

		using namespace std::chrono_literals;
		
		auto gui_window1 = GUIWindowHierarchy::get().create_window<GUIWindow>();
		auto gui_window2 = GUIWindowHierarchy::get().create_window<GUIWindow>(gui_window1);
		auto gui_window3 = GUIWindowHierarchy::get().create_window<GUIWindow>(gui_window1);
		auto gui_window4 = GUIWindowHierarchy::get().create_window<GUIWindow>(gui_window1);
		auto gui_window5 = GUIWindowHierarchy::get().create_window<GUIWindow>(gui_window1);

		gui_window1->window->set_window_name("1");
		gui_window2->window->set_window_name("2");
		gui_window3->window->set_window_name("3");
		gui_window4->window->set_window_name("4");
		gui_window5->window->set_window_name("5");

		font_id font = FontBank::get().load_font("../GraphicsCortex/Fonts/Roboto-Regular.ttf", 64);

		gui_window1->run();

		return true;
	}
};