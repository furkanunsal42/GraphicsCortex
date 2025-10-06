#pragma once
#include "GUI/CortexGUI.h"

#include "CortexGUIWidgets/Container/GUIWidget_Grid.h"
#include "Window.h"
#include "WindowBoundGlobalResources.h"

class GUIWindowHierarchy;

typedef int32_t guiwindow_t;
constexpr guiwindow_t invalid_guiwindow = -1;

class GUIWindow {
public:

	GUIWindow(Window& window);
	GUIWindow();

	std::unique_ptr<Window> window = nullptr;
	WidgetHandle<widget::Grid> grid;

	virtual void run();
	virtual void handle_events();
	virtual void render();

	bool visible = true;

private:
	friend GUIWindowHierarchy;
	guiwindow_t id = invalid_guiwindow;
};

