#pragma once
#include <string>

#include "glm.hpp"
#include "Font.h"

#include "GUI/CortexGUI.h"

typedef uint32_t control_style_t;
constexpr control_style_t invalid_control_style = -1;

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
	control_style_t id = invalid_control_style;

private:

};

class GUIControl {
public:

	GUIControl() {
		widget = GUI::get().create_widget();
	};

	glm::vec4 margin;
	glm::vec4 padding;
	Alignment content_alignment;

	glm::vec4 border_rounding;
	glm::vec4 border_thickness;
	glm::vec4 border_color;

	glm::vec2 target_size;
	//glm::vec2 min_size;
	//glm::vec2 max_size;

	int32_t z;

	glm::vec4 color;

	font_id font;
	float text_height;
	std::string text;
	glm::vec4 text_color;

	control_style_t style;
	
	void apply_properties_to_widget(Widget& widget) {
		widget.size() = target_size;
		widget.color() = color;
		widget.z() = z;
	};

	virtual Widget& get_widget() = 0;
	//virtual void update_before_render();
protected:

	Widget widget = Widget::null_widget;

};
