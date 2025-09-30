#include "CortexGUIWidget.h"
#include "CortexGUI.h"
#include "Window.h"
#include "AABB.h"

Widget::Widget() 
{
	element = GUI::get().create_element();
}

namespace {

	float get_t(
		std::chrono::system_clock::time_point last_begin,
		std::chrono::system_clock::time_point last_end,
		std::chrono::system_clock::duration transition_time) 
	{
		if (transition_time.count() == 0)
			return 1;

		auto now = std::chrono::system_clock::now();

		bool happening =
			(last_begin != Widget::invalid_time && last_end == Widget::invalid_time) ||
			(last_begin != Widget::invalid_time && last_end != Widget::invalid_time && last_begin > last_end);
		
		bool recovering =
			(last_begin != Widget::invalid_time && last_end != Widget::invalid_time && last_begin < last_end && now - last_end < std::min(transition_time, last_end - last_begin));

		//if (hover_happening)
		//	std::cout << "hovering" << std::endl;
		//if (hover_recovering)
		//	std::cout << "recovering" << std::endl;

		float t = 0;
		if (happening) {
			std::chrono::system_clock::duration time_passed = std::min(std::chrono::system_clock::now() - last_begin, transition_time);
			t = 1 - (transition_time - time_passed).count() / (float)transition_time.count();
		}
		if (recovering) {
			std::chrono::system_clock::duration time_passed_hovering	= std::min(last_end - last_begin, transition_time);
			std::chrono::system_clock::duration time_passed_recovering	= std::min(std::chrono::system_clock::now() - last_end, transition_time);
			t = 1 - (transition_time - time_passed_hovering + time_passed_recovering).count() / (float)transition_time.count();
		}

		return t;
	}

	glm::vec4 interpolate(glm::vec4 a, glm::vec4 b, float t) {
		t = glm::clamp(t, 0.0f, 1.0f);
		return a * (1 - t) + b * t;
	}

	glm::vec4 get_property(
		glm::vec4 default_property,
		std::optional<glm::vec4>  on_hover,
		std::chrono::system_clock::time_point last_hover_begin,
		std::chrono::system_clock::time_point last_hover_end,
		std::chrono::system_clock::duration hover_transition_time,
		std::optional<glm::vec4> on_hold,
		std::chrono::system_clock::time_point last_hold_begin,
		std::chrono::system_clock::time_point last_hold_end,
		std::chrono::system_clock::duration hold_transition_time
	) {
		glm::vec4 property = interpolate(default_property, on_hover.value_or(default_property), get_t(last_hover_begin, last_hover_end, hover_transition_time));
		property = interpolate(property, on_hold.value_or(default_property), get_t(last_hold_begin, last_hold_end, hold_transition_time));
		return property;
	}
}

void Widget::apply_properties_to_element(Element& element, glm::vec2 allocated_size)
{
	element.size()				= allocated_size;
	element.color()				= get_property(color,				on_hover_color,				last_hover_begin, last_hover_end, on_hover_color_transition,			on_hold_color,				last_hold_begin, last_hold_end, on_hold_color_transition			);
	element.border_rounding()	= get_property(border_rounding,		on_hover_border_rounding,	last_hover_begin, last_hover_end, on_hover_border_rounding_transition,	on_hold_border_rounding,	last_hold_begin, last_hold_end, on_hold_border_rounding_transition	); 
	element.border_thickness()	= get_property(border_thickness,	on_hover_border_thickness,	last_hover_begin, last_hover_end, on_hover_border_thickness_transition, on_hold_border_thickness,	last_hold_begin, last_hold_end, on_hold_border_thickness_transition	); 
	element.border_color0()		= get_property(border_color0,		on_hover_border_color0,		last_hover_begin, last_hover_end, on_hover_border_color0_transition,	on_hold_border_color0,		last_hold_begin, last_hold_end, on_hold_border_color0_transition	); 
	element.border_color1()		= get_property(border_color1,		on_hover_border_color1,		last_hover_begin, last_hover_end, on_hover_border_color1_transition,	on_hold_border_color1,		last_hold_begin, last_hold_end, on_hold_border_color1_transition	); 
	element.border_color2()		= get_property(border_color2,		on_hover_border_color2,		last_hover_begin, last_hover_end, on_hover_border_color2_transition,	on_hold_border_color2,		last_hold_begin, last_hold_end, on_hold_border_color2_transition	); 
	element.border_color3()		= get_property(border_color3,		on_hover_border_color3,		last_hover_begin, last_hover_end, on_hover_border_color3_transition,	on_hold_border_color3,		last_hold_begin, last_hold_end, on_hold_border_color3_transition	); 
	element.shadow_thickness()	= get_property(shadow_thickness,	on_hover_shadow_thickness,	last_hover_begin, last_hover_end, on_hover_shadow_thickness_transition, on_hold_shadow_thickness,	last_hold_begin, last_hold_end, on_hold_shadow_thickness_transition	); 
	element.shadow_color()		= get_property(shadow_color,		on_hover_shadow_color,		last_hover_begin, last_hover_end, on_hover_shadow_color_transition,		on_hold_shadow_color,		last_hold_begin, last_hold_end, on_hold_shadow_color_transition		); 
	element.z() = z;
}

void Widget::poll_events(glm::vec2 absolute_position)
{
	AABB2 self_aabb(absolute_position, absolute_position + element.size());
	glm::vec2 cursor_pos = GUI::get().get_window()->get_cursor_position();
	bool cursor_on_widget	= self_aabb.does_contain(cursor_pos);
	
	Window::PressAction cursor_left_action = GUI::get().get_window()->get_mouse_button(Window::MouseButton::LEFT);
	
	bool left_press_impulse = GUI::get().get_window()->get_mouse_press_inpulse(Window::MouseButton::LEFT);
	bool left_release_impulse = GUI::get().get_window()->get_mouse_release_inpulse(Window::MouseButton::LEFT);

	bool hover_happening	= 
		(last_hover_begin != invalid_time && last_hover_end == invalid_time) ||
		(last_hover_begin != Widget::invalid_time && last_hover_end != Widget::invalid_time && last_hover_begin > last_hover_end);

	bool hold_happening =
		(last_hold_begin != invalid_time && last_hold_end == invalid_time) ||
		(last_hold_begin != Widget::invalid_time && last_hold_end != Widget::invalid_time && last_hold_begin > last_hold_end);

	if (!cursor_on_widget && hover_happening) {
		events.publish(GUIEvent::HoverEnd);
		last_hover_end = std::chrono::system_clock::now();
	}
	else if (cursor_on_widget && !hover_happening) {
		events.publish(GUIEvent::HoverBegin);
		last_hover_begin = std::chrono::system_clock::now();
	}
	
	if (cursor_on_widget && left_press_impulse && !hold_happening) {
		events.publish(GUIEvent::HoldBegin);
		last_hold_begin = std::chrono::system_clock::now();
	}
	else if (cursor_on_widget && cursor_left_action == Window::PressAction::RELEASE && hold_happening) {
		events.publish(GUIEvent::Clicked);
		events.publish(GUIEvent::HoldEnd);
		last_hold_end = std::chrono::system_clock::now();
	}
	else if (!cursor_on_widget && cursor_left_action == Window::PressAction::RELEASE && hold_happening) {
		events.publish(GUIEvent::HoldEnd);
		last_hold_end = std::chrono::system_clock::now();
	}
}
