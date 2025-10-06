#include "CortexGUIWindowHierarchy.h"

GUIWindowHierarchy& GUIWindowHierarchy::get() {
	static GUIWindowHierarchy hierarchy;
	return hierarchy;
}

void GUIWindowHierarchy::release() {
	windows.clear();
	next_guiwindow_id = 0;
}

void GUIWindowHierarchy::release_window(guiwindow_t window)
{
	if (window == invalid_guiwindow)
		return;

	for (auto& child : windows[window].children)
		release_window(child);
	
	windows.erase(window);
}

bool GUIWindowHierarchy::does_window_exist(guiwindow_t window)
{
	if (window == invalid_guiwindow)
		return false;

	return windows.find(window) != windows.end();
}

bool GUIWindowHierarchy::is_root_window(guiwindow_t window)
{
	if (!does_window_exist(window)) {
		std::cout << "[GUI Error] GUIWindowHierarchy::is_root_window() is called with a window that doesn't exist" << std::endl;
		ASSERT(false);
	}

	if (!does_window_exist(windows.at(window).parent_window)) {
		std::cout << "[GUI Error] GUIWindowHierarchy::is_root_window() is called with a window that has improper parent_window" << std::endl;
		ASSERT(false);
	}

	return windows.at(window).parent_window == invalid_guiwindow;
}

void GUIWindowHierarchy::set_parent_window(guiwindow_t window, guiwindow_t new_parent)
{
	if (!does_window_exist(window)) {
		std::cout << "[GUI Error] GUIWindowHierarchy::is_root_window() is called with a window that doesn't exist" << std::endl;
		ASSERT(false);
	}

	if (!does_window_exist(new_parent) && new_parent != invalid_guiwindow) {
		std::cout << "[GUI Error] GUIWindowHierarchy::is_root_window() is called with a new_parent that doesn't exist" << std::endl;
		ASSERT(false);
	}

	if (window == new_parent) {
		std::cout << "[GUI Error] GUIWindowHierarchy::is_root_window() is called idential a new_parent and window" << std::endl;
		ASSERT(false);
	}

	if (windows[window].parent_window == new_parent)
		return;

	if (windows[window].parent_window != invalid_guiwindow)
		std::erase(windows[windows[window].parent_window].children, window);
	
	if (new_parent != invalid_guiwindow)
		windows[new_parent].children.push_back(window);
	
	windows[window].parent_window = new_parent;
}

GUIWindow& GUIWindowHierarchy::get_window_data(guiwindow_t window)
{
	if (!does_window_exist(window)) {
		std::cout << "[GUI Error] GUIWindowHierarchy::get_window_data() is called with an window that doesn't exist" << std::endl;
		ASSERT(false);
	}

	return *windows[window].window;
}

const std::vector<guiwindow_t>& GUIWindowHierarchy::get_window_children(guiwindow_t window)
{
	if (!does_window_exist(window)) {
		std::cout << "[GUI Error] GUIWindowHierarchy::get_window_children() is called with an window that doesn't exist" << std::endl;
		ASSERT(false);
	}

	return windows[window].children;
}

guiwindow_t GUIWindowHierarchy::generate_id()
{
	guiwindow_t id = next_guiwindow_id;
	next_guiwindow_id++;
	return id;
}
