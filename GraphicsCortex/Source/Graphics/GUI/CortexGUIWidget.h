#pragma once
#include <string>

#include "glm.hpp"
#include "Font.h"

#include "GUI/CortexGUIWidgetStyle.h"
#include "GUI/CortexGUIElement.h"

class GUI;

typedef uint32_t widget_t;
constexpr widget_t invalid_widget = -1;

class Widget {
public:

	Widget();

	glm::vec4 margin;
	glm::vec4 padding;

	glm::vec4 border_rounding;
	glm::vec4 border_thickness;
	glm::vec4 border_color;

	glm::vec2 target_size;
	//glm::vec2 min_size;
	//glm::vec2 max_size;

	int32_t z;

	glm::vec4 color;

	widget_style_t style;
	
	void apply_properties_to_element(Element& element);

	virtual Element& get_element() = 0;
	//virtual void update_before_render();

protected:

	Element element = Element::null_element;
};

template<typename W>
class WidgetHandle {
public:
	
	WidgetHandle() = default;

	W& properties() {
		return *std::reinterpret_pointer_cast<W, Widget>(GUI::get().widgets[id]);
	}

	W* operator->() const {
		return std::reinterpret_pointer_cast<W, Widget>(GUI::get().widgets[id]).get();
	}
	
	template <typename W0, typename W1>
	friend bool operator==(const WidgetHandle<W0>& a, const WidgetHandle<W1>& b);

	operator widget_t();

private:
	friend GUI;
	WidgetHandle(void* owner_gui_identifier, element_t id);
	widget_t id = invalid_widget;
	void* owner_gui_identifier = nullptr;
};

template<typename W>
inline WidgetHandle<W>::operator widget_t()
{
	return id;
}

template<typename W>
inline WidgetHandle<W>::WidgetHandle(void* owner_gui_identifier, element_t id) :
	owner_gui_identifier(owner_gui_identifier), id(id)
{
}
