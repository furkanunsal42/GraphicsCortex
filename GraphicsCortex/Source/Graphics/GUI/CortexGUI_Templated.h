#pragma once
#include "CortexGUI.h"

template<typename T>
inline WidgetHandle<T> GUI::create_widget() {
	widget_t id = _create_widget<T>();
	return WidgetHandle<T>(attached_window_handle, id);
}

template<typename T>
inline void GUI::release_widget(WidgetHandle<T>& widget)
{
	_release_widget(widget.id);
}

template<typename T>
inline bool GUI::does_widget_exist(WidgetHandle<T>& widget)
{
	return _does_widget_exist(widget.id);
}

template<typename T>
inline void GUI::render(WidgetHandle<T>& widget)
{
	if (!does_widget_exist(widget))
		return;

	render(widgets[widget.id]->get_element());
}

template<typename T>
inline widget_t GUI::_create_widget()
{
	widget_t id = _generate_widget_id();
	widgets[id] = std::make_shared<T>();
	return id;
}

namespace widget {
	template<typename T> 
	inline WidgetHandle<T> create() {
		return GUI::get().create_widget<T>();
	}
	
	template<typename T> 
	inline void release(WidgetHandle<T>& widget){
		return GUI::get().release_widget(widget);
	}
	
	template<typename T> 
	inline bool does_exist(WidgetHandle<T>& widget){
		return GUI::get().does_widget_exist(widget);
	}
}
