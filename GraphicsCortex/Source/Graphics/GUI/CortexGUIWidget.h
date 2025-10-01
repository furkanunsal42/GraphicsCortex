#pragma once
#include <string>

#include "glm.hpp"
#include "Font.h"

#include "GUI/CortexGUIWidgetStyle.h"
#include "GUI/CortexGUIElement.h"

#include "Newsletter.h"
#include "Window.h"

// Weak EventType variants doesn't require the element to be top-most widget
enum GUIEvent {
	HoverBegin,
	HoverEnd,
	HoldBegin,
	HoldEnd,
	Clicked,
	DoubleClicked,
	RightClicked,
};

class GUI;

typedef uint32_t widget_t;
constexpr widget_t invalid_widget = -1;

class Widget {
public:

	Widget();

	glm::vec2 target_size						= glm::vec2(0);
	glm::vec4 margin							= glm::vec4(0);
	glm::vec4 padding							= glm::vec4(0);
	glm::vec4 color								= glm::vec4(1, 1, 1, 1);	/*optional*/			
	glm::vec4 border_rounding					= glm::vec4(0);				/*optional*/
	glm::vec4 border_thickness					= glm::vec4(0);				/*optional*/
	glm::vec4 border_color0						= glm::vec4(0, 0, 0, 1);	/*optional*/			
	glm::vec4 border_color1						= glm::vec4(0);				/*optional*/
	glm::vec4 border_color2						= glm::vec4(0);				/*optional*/
	glm::vec4 border_color3						= glm::vec4(0);				/*optional*/
	glm::vec4 shadow_thickness					= glm::vec4(0);				/*optional*/
	glm::vec4 shadow_color						= glm::vec4(0);				/*optional*/
	int32_t z = 0;
	widget_style_t style = invalid_widget_style;
	//glm::vec2 min_size;
	//glm::vec2 max_size;
	
	///////////////////////////////////////////////////////////////////
	//	on hover
	
	std::optional<glm::vec4> on_hover_color				= std::nullopt;
	std::optional<glm::vec4> on_hover_border_rounding	= std::nullopt;
	std::optional<glm::vec4> on_hover_border_thickness	= std::nullopt;
	std::optional<glm::vec4> on_hover_border_color0		= std::nullopt;
	std::optional<glm::vec4> on_hover_border_color1		= std::nullopt;
	std::optional<glm::vec4> on_hover_border_color2		= std::nullopt;
	std::optional<glm::vec4> on_hover_border_color3		= std::nullopt;
	std::optional<glm::vec4> on_hover_shadow_thickness	= std::nullopt;
	std::optional<glm::vec4> on_hover_shadow_color		= std::nullopt;
	widget_style_t on_hover_style						= invalid_widget_style;

	std::chrono::system_clock::duration on_hover_color_transition				= std::chrono::system_clock::duration(0);
	std::chrono::system_clock::duration on_hover_border_rounding_transition		= std::chrono::system_clock::duration(0);
	std::chrono::system_clock::duration on_hover_border_thickness_transition	= std::chrono::system_clock::duration(0);
	std::chrono::system_clock::duration on_hover_border_color0_transition		= std::chrono::system_clock::duration(0);
	std::chrono::system_clock::duration on_hover_border_color1_transition		= std::chrono::system_clock::duration(0);
	std::chrono::system_clock::duration on_hover_border_color2_transition		= std::chrono::system_clock::duration(0);
	std::chrono::system_clock::duration on_hover_border_color3_transition		= std::chrono::system_clock::duration(0);
	std::chrono::system_clock::duration on_hover_shadow_thickness_transition	= std::chrono::system_clock::duration(0);
	std::chrono::system_clock::duration on_hover_shadow_color_transition		= std::chrono::system_clock::duration(0);
	///////////////////////////////////////////////////////////////////
	//	on hold
	
	std::optional<glm::vec4> on_hold_color				= std::nullopt;
	std::optional<glm::vec4> on_hold_border_rounding	= std::nullopt;
	std::optional<glm::vec4> on_hold_border_thickness	= std::nullopt;
	std::optional<glm::vec4> on_hold_border_color0		= std::nullopt;
	std::optional<glm::vec4> on_hold_border_color1		= std::nullopt;
	std::optional<glm::vec4> on_hold_border_color2		= std::nullopt;
	std::optional<glm::vec4> on_hold_border_color3		= std::nullopt;
	std::optional<glm::vec4> on_hold_shadow_thickness	= std::nullopt;
	std::optional<glm::vec4> on_hold_shadow_color		= std::nullopt;
	widget_style_t on_hold_style						= invalid_widget_style;

	std::chrono::system_clock::duration on_hold_color_transition				= std::chrono::system_clock::duration(0);
	std::chrono::system_clock::duration on_hold_border_rounding_transition		= std::chrono::system_clock::duration(0);
	std::chrono::system_clock::duration on_hold_border_thickness_transition		= std::chrono::system_clock::duration(0);
	std::chrono::system_clock::duration on_hold_border_color0_transition		= std::chrono::system_clock::duration(0);
	std::chrono::system_clock::duration on_hold_border_color1_transition		= std::chrono::system_clock::duration(0);
	std::chrono::system_clock::duration on_hold_border_color2_transition		= std::chrono::system_clock::duration(0);
	std::chrono::system_clock::duration on_hold_border_color3_transition		= std::chrono::system_clock::duration(0);
	std::chrono::system_clock::duration on_hold_shadow_thickness_transition		= std::chrono::system_clock::duration(0);
	std::chrono::system_clock::duration on_hold_shadow_color_transition			= std::chrono::system_clock::duration(0);
	///////////////////////////////////////////////////////////////////

	Newsletter<GUIEvent> events;

	virtual void apply_properties_to_element(Element& element, glm::vec2 allocated_size);
	virtual Element& get_element(glm::vec2 allocated_size) = 0;
	virtual void poll_events(glm::vec2 absolute_position);

	constexpr static std::chrono::system_clock::time_point invalid_time = std::chrono::system_clock::time_point::max();

	float get_t(
		std::chrono::system_clock::time_point last_begin,
		std::chrono::system_clock::time_point last_end,
		std::chrono::system_clock::duration transition_time
		);
	
	glm::vec4 interpolate(glm::vec4 a, glm::vec4 b, float t);

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
		);

protected:

	Element element = Element::null_element;

	std::chrono::system_clock::time_point last_hover_begin		= invalid_time;
	std::chrono::system_clock::time_point last_hover_end		= invalid_time;
	std::chrono::system_clock::time_point last_hold_begin		= invalid_time;
	std::chrono::system_clock::time_point last_hold_end			= invalid_time;
	std::chrono::system_clock::time_point last_right_hold_begin = invalid_time;
	std::chrono::system_clock::time_point last_right_hold_end	= invalid_time;
	std::chrono::system_clock::time_point last_left_click		= invalid_time;
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
