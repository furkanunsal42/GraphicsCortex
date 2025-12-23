#pragma once
#include "CortexGUI.h"
#include "PrimitiveRenderer.h"

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
inline void GUI::render(WidgetHandle<T>& widget, glm::vec2 position)
{
	if (!does_widget_exist(widget))
		return;

	glm::vec2 allocated_size = widgets[widget.id]->target_size;

	if (widgets[widget.id]->target_size.x == 0)
		allocated_size.x = widgets[widget.id]->get_element(allocated_size).size().x;
	if (widgets[widget.id]->target_size.y == 0)
		allocated_size.y = widgets[widget.id]->get_element(allocated_size).size().y;

	if (widgets[widget.id]->target_size.x < 0)
		allocated_size.x = primitive_renderer::get_viewport_size().x;
	if (widgets[widget.id]->target_size.y < 0)
		allocated_size.y = primitive_renderer::get_viewport_size().y;

	// TODO: find physical size of the widget from parents

	//if (glm::any(glm::equal(allocated_size, glm::vec2(0))))
	//	allocated_size = widgets[widget.id]->get_element(allocated_size).size();
	//
	//if (glm::any(glm::lessThan(widgets[widget.id]->target_size, glm::vec2(0))))
	//	allocated_size = primitive_renderer::get_viewport_size();

	//render(widgets[widget.id]->get_element(allocated_size));

	Element& element = widgets[widget.id]->get_element(allocated_size);
	glm::vec2 old_position = element.position();
	element.position() = position;
	render(element);
	element.position() = old_position;
}

inline void GUI::render(Widget& widget, glm::vec2 position) {
	//if (!does_widget_exist(widget))
	//	return;

	glm::vec2 allocated_size = widget.target_size;


	if (widget.target_size.x == 0)
		allocated_size.x = widget.get_element(allocated_size).size().x;
	if (widget.target_size.y == 0)
		allocated_size.y = widget.get_element(allocated_size).size().y;

	if (widget.target_size.x < 0)
		allocated_size.x = primitive_renderer::get_viewport_size().x;
	if (widget.target_size.y < 0)
		allocated_size.y = primitive_renderer::get_viewport_size().y;


	//if (glm::any(glm::equal(allocated_size, glm::vec2(0))))
	//	allocated_size = widget.get_element(allocated_size).size();
	//
	//// TODO: find physical size of the widget from parents
	//if (glm::any(glm::lessThan(widget.target_size, glm::vec2(0))))
	//	allocated_size = primitive_renderer::get_viewport_size();
	
	Element& element = widget.get_element(allocated_size);
	glm::vec2 old_position = element.position();
	element.position() = position;
	render(element);
	element.position() = old_position;
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
