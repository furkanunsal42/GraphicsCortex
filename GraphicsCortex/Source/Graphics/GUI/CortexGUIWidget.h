#pragma once
#include <string>

#include "glm.hpp"
#include "Font.h"

#include "GUI/CortexGUI.h"

typedef uint32_t widget_t;
constexpr widget_t invalid_widget = -1;

typedef uint32_t widget_style_t;
constexpr widget_style_t invalid_widget_style = -1;

enum Alignment {
	Center,
	Left,
	Top,
	Right,
	Bottom
};

struct ControlStyleInfo {
	glm::vec4 margin;
	glm::vec4 padding;
	Alignment content_alignment;

	glm::vec4 border_rounding;
	glm::vec4 border_thickness;
	glm::vec4 border_color;

	glm::vec2 target_size;
	//glm::vec2 min_size;
	//glm::vec2 max_size;
	
	glm::vec4 color;
	
	font_id font;
	float text_height;
	std::string text;
	glm::vec4 text_color;

};

class ControlStyle {
public:
	widget_style_t id = invalid_widget_style;

private:

};

class Widget {
public:

	Widget() {
		element = GUI::get().create_element();
	};

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
	
	void apply_properties_to_element(Element& element) {
		element.size() = target_size;
		element.color() = color;
		element.z() = z;
	};

	virtual Element& get_element() = 0;
	//virtual void update_before_render();
protected:

	Element element = Element::null_element;

};
