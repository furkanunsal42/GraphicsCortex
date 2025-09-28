#include "CortexGUIWidget.h"
#include "CortexGUI.h"
#include "Window.h"
#include "AABB.h"

Widget::Widget() 
{
	element = GUI::get().create_element();
}

namespace {

	float get_t(std::chrono::system_clock::time_point begin, std::chrono::system_clock::duration transition_time) {
		if (transition_time.count() == 0)
			return 1;

		if (begin == Widget::invalid_time)
			return 0;

		std::chrono::system_clock::duration time_passed = std::chrono::system_clock::now() - begin;
		float t = (time_passed - transition_time).count() / (float)transition_time.count();

		return t;
	}

	glm::vec4 interpolate(glm::vec4 a, glm::vec4 b, float t) {
		t = glm::clamp(t, 0.0f, 1.0f);
		return a * (1 - t) + b * t;
	}
}

void Widget::apply_properties_to_element(Element& element, glm::vec2 allocated_size)
{
	bool hovering = current_hover_begin != invalid_time;

	glm::vec4 _color				= hovering ? on_hover.color.			value_or(color)					: color;
	glm::vec4 _border_rounding		= hovering ? on_hover.border_rounding.	value_or(border_rounding)		: border_rounding;
	glm::vec4 _border_thickness		= hovering ? on_hover.border_thickness.	value_or(border_thickness)		: border_thickness;
	glm::vec4 _border_color0		= hovering ? on_hover.border_color0.	value_or(border_color0)			: border_color0;
	glm::vec4 _border_color1		= hovering ? on_hover.border_color1.	value_or(border_color1)			: border_color1;
	glm::vec4 _border_color2		= hovering ? on_hover.border_color2.	value_or(border_color2)			: border_color2;
	glm::vec4 _border_color3		= hovering ? on_hover.border_color3.	value_or(border_color3)			: border_color3;
	glm::vec4 _shadow_thickness		= hovering ? on_hover.shadow_thickness.	value_or(shadow_thickness)		: shadow_thickness;
	glm::vec4 _shadow_color			= hovering ? on_hover.shadow_color.		value_or(shadow_color)			: shadow_color;

	_color				= interpolate(color				, _color				, get_t(current_hover_begin, on_hover.color_transition));
	_border_rounding	= interpolate(border_rounding	, _border_rounding		, get_t(current_hover_begin, on_hover.border_rounding_transition));
	_border_thickness	= interpolate(border_thickness	, _border_thickness		, get_t(current_hover_begin, on_hover.border_thickness_transition));
	_border_color0		= interpolate(border_color0		, _border_color0		, get_t(current_hover_begin, on_hover.border_color0_transition));
	_border_color1		= interpolate(border_color1		, _border_color1		, get_t(current_hover_begin, on_hover.border_color1_transition));
	_border_color2		= interpolate(border_color2		, _border_color2		, get_t(current_hover_begin, on_hover.border_color2_transition));
	_border_color3		= interpolate(border_color3		, _border_color3		, get_t(current_hover_begin, on_hover.border_color3_transition));
	_shadow_thickness	= interpolate(shadow_thickness	, _shadow_thickness		, get_t(current_hover_begin, on_hover.shadow_thickness_transition));
	_shadow_color		= interpolate(shadow_color		, _shadow_color			, get_t(current_hover_begin, on_hover.shadow_color_transition));

	element.size() = allocated_size;
	element.color()				= _color;
	element.border_rounding()	= _border_rounding;
	element.border_thickness()	= _border_thickness;
	element.border_color0()		= _border_color0;
	element.border_color1()		= _border_color1;
	element.border_color2()		= _border_color2;
	element.border_color3()		= _border_color3;
	element.shadow_thickness()	= _shadow_thickness;
	element.shadow_color()		= _shadow_color;
	element.z() = z;
}

void Widget::poll_events(glm::vec2 absolute_position)
{
	AABB2 self_aabb(absolute_position, absolute_position + element.size());
	glm::vec2 cursor_pos = GUI::get().get_window()->get_cursor_position();

	if (!self_aabb.does_contain(cursor_pos) && current_hover_begin != invalid_time) {
		events.publish(GUIEvent::HoverEnd_Weak);
		current_hover_begin = invalid_time;
		return;
	}
	else if (!self_aabb.does_contain(cursor_pos)) {
		current_hover_begin = invalid_time;
		return;
	}
	else if (self_aabb.does_contain(cursor_pos) && current_hover_begin == invalid_time) {
		events.publish(GUIEvent::HoverBegin_Weak);
		current_hover_begin = std::chrono::system_clock::now();
		return;
	}
}
