#pragma once

#include<vector>

#include "CortexGUIWidgets/Window/GUIWidget_GUIWindow.h"

class GUIWindowManager {
public:

	void handle();

private:
	std::vector<widget::GUIWindow> gui_windows;
};