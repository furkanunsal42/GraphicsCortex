#pragma once

#include "CortexGUIWindowHierarchy.h"

template<typename W>
inline GUIWindowHandle<W> GUIWindowHierarchy::create_window()
{
	guiwindow_t id = generate_id();
	windows[id].id = id;
	windows[id].parent_window = invalid_guiwindow;
	windows[id].window = std::make_shared<W>();
	windows[id].window->id = id;
	return GUIWindowHandle<W>(id);
}

template<typename W>
inline GUIWindowHandle<W> GUIWindowHierarchy::create_window(guiwindow_t parent_window)
{
	if (parent_window == invalid_guiwindow)
		return create_window<W>();

	if (!does_window_exist(parent_window)) {
		std::cout << "[GUI Error] GUIWindowHierarchy::create_window() is called with a parent window that doesn't exist" << std::endl;
		ASSERT(false);
	}

	guiwindow_t id = generate_id();
	windows[id].id = id;
	windows[id].parent_window = parent_window;
	windows[id].window = std::make_shared<W>(*windows[parent_window].window->window);
	windows[id].window->id = id;
	
	windows[parent_window].children.push_back(id);
	//windows[parent_window].window->window->context_make_current();

	return GUIWindowHandle<W>(id);
}

template<typename W>
inline GUIWindowHandle<W>::operator guiwindow_t()
{
	return id;
}

template<typename W>
inline W* GUIWindowHandle<W>::operator->() const {
	return &GUIWindowHierarchy::get().get_window_data(id);
}

template<typename W>
inline GUIWindowHandle<W>::GUIWindowHandle<W>(guiwindow_t handle) :
	id(handle)
{

}