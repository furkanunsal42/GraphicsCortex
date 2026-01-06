#pragma once
#include "GUI2Dynamic.h"
#include <chrono>
#include "Font.h"

#define widget2_styled_event(type, name, event_name) \
	std::optional<type> event_name##_##name = std::nullopt; \
	std::chrono::system_clock::duration event_name##_##name##_transition = std::chrono::system_clock::duration(0);

#define widget2_styled_property1(type, name, default_value, event_name0) \
	type name = default_value; \
	widget2_styled_event(type, name, event_name0)
	
#define widget2_styled_property2(type, name, default_value, event_name0, event_name1) \
	type name = default_value; \
	widget2_styled_event(type, name, event_name0) \
	widget2_styled_event(type, name, event_name1)

#define widget2_styled_property3(type, name, default_value, event_name0, event_name1, event_name2) \
	type name = default_value; \
	widget2_styled_event(type, name, event_name0) \
	widget2_styled_event(type, name, event_name1) \
	widget2_styled_event(type, name, event_name2)

#define widget2_styled_property4(type, name, default_value, event_name0, event_name1, event_name2, event_name3) \
	type name = default_value; \
	widget2_styled_event(type, name, event_name0) \
	widget2_styled_event(type, name, event_name1) \
	widget2_styled_event(type, name, event_name2) \
	widget2_styled_event(type, name, event_name3)

#define	widget2_get_property1(name, event_name0) \
	get_property(name, event_name0##_##name##, event_name0##_##name##_transition, on_hold_##name, on_hold_##name##_transition)

#define	widget2_get_property2(name, event_name0, event_name1) \
	get_property(name, event_name0##_##name, event_name0##_##name##_transition, event_name1##_##name, event_name1##_##name##_transition)


namespace widget2 {

	class Widget {
	public:
		GUI2Dynamic::ResolvedProperties get_resolved_properties(GUI2Dynamic& gui_dynamic);
	protected:
		size_t id = GUI2Dynamic::invalid_id;
	};

	struct IOEvent {
		using time_point = std::chrono::system_clock::time_point;
		constexpr static time_point	invalid_time = time_point::max();

		time_point begin	= invalid_time;
		time_point end		= invalid_time;
	};

	class IOWidget : public Widget {
		using duration		= std::chrono::system_clock::duration;
	public:
		constexpr static int32_t mouse_left = 0;
		constexpr static int32_t mouse_right = 1;
		
		enum MouseState {
			
			None		= 0b00000000,
			Enter		= 0b10000001,
			Hover		= 0b00000001,
			Leave		= 0b01000000,

			PressBegin	= 0b10000011,
			Press		= 0b00000011,
			PressEnd	= 0b01000011,

			HoldBegin	= 0b10000111,
			Hold		= 0b00000111,
			HoldEnd		= 0b01000111,

			Click		= 0b00101111,
			DoubleClick	= 0b00111111,

			CarryBegin	= 0b10100111,
			Carry		= 0b00100111,
			CarryEnd	= 0b01100111,

		};

		MouseState	get_mouse_state(int32_t mouse_button = mouse_left);
		glm::vec2	get_cursor_position_when_hold_begin(int32_t mouse_button = mouse_left);
		glm::vec2	get_widget_position_when_hold_begin(int32_t mouse_button = mouse_left);

		duration double_click_max_delay = std::chrono::milliseconds(500);
		float carry_begin_min_offset = 10;

		IOEvent hover;
		IOEvent hold;
		IOEvent click;

	protected:
		
		void resolve_io(GUI2Dynamic& gui_dynamic);
		
	private:

		constexpr static glm::vec2	invalid_position = glm::vec2(-1);
		
		MouseState mouse_states[2] = { None, None };
		glm::vec2 cursor_position_when_hold_begin	= invalid_position;
		glm::vec2 widget_position_when_hold_begin	= invalid_position;
		
		bool was_last_click_double = false;
	};

	template<typename T>
	struct StyleProperty {
		using duration = std::chrono::system_clock::duration;

		StyleProperty() = default;
		StyleProperty(T value) : value(value) { }

		operator T() { return value; }

		T value = T(0);

		StyleProperty& transition(T default_value) {
			value = value;
			return *this;
		}

		StyleProperty& transition(IOEvent event, T transition_value, duration transition_duration = std::chrono::milliseconds(1)) {
			float t = glm::clamp(get_t(event, transition_duration), 0.0f, 1.0f);
			value = value * (1 - t) + transition_value * t;
			return *this;
		}

	private:

		float get_t(IOEvent event, duration transition_duration) {
			
			if (event.begin == IOEvent::invalid_time && event.end == IOEvent::invalid_time)
				return 0;

			if (transition_duration.count() == 0)
				return event.end > event.begin ? 1 : 0;

			auto now = std::chrono::system_clock::now();

			duration time_passed			= std::min(now - event.begin,		transition_duration);
			duration time_passed_hovering	= std::min(event.end - event.begin, transition_duration);
			duration time_passed_recovering = std::min(now - event.end,			transition_duration);

			bool happening =
				(event.begin != IOEvent::invalid_time && event.end == IOEvent::invalid_time) ||
				(event.begin != IOEvent::invalid_time && event.end != IOEvent::invalid_time && event.begin > event.end);

			bool recovering =
				event.begin != IOEvent::invalid_time	&&
				event.end != IOEvent::invalid_time		&&
				event.begin < event.end					&&
				now - event.end < time_passed_hovering;

			duration relevent_duration = happening ? time_passed : time_passed_hovering - time_passed_recovering;

			return 1 - (transition_duration - relevent_duration).count() / (float)transition_duration.count();

		}
	};

	class StyledWidget : public IOWidget {
		using duration		= std::chrono::system_clock::duration;
		using time_point	= std::chrono::system_clock::time_point;
	protected:

		template<typename T>
		T get_property(
			T					default_property,
			std::optional<T>	on_hover,
			duration			hover_transition_time,
			std::optional<T>	on_hold,
			duration			hold_transition_time
		);
		
	private:

		float get_t(
			time_point last_begin,
			time_point last_end,
			duration transition_time
		);
		
		template<typename T>
		T interpolate(T a, T b, float t);

	};

	struct Box : public StyledWidget {
		
		StyleProperty<glm::vec4> margin = glm::vec4(0);
		
		//widget2_styled_property2(glm::vec4,	margin,				glm::vec4(0),					on_hover, on_hold)
		
		widget2_styled_property2(glm::vec2,	target_size,		glm::vec2(128),					on_hover, on_hold)
		widget2_styled_property2(glm::vec2,	min_size,			glm::vec2(GUI2Dynamic::fit),	on_hover, on_hold)
		widget2_styled_property2(glm::vec2,	max_size,			glm::vec2(GUI2Dynamic::avail),	on_hover, on_hold)
		widget2_styled_property2(glm::vec4,	color,				glm::vec4(1, 1, 1, 1),			on_hover, on_hold)
		widget2_styled_property2(glm::vec4,	border_thickness,	glm::vec4(0),					on_hover, on_hold)
		widget2_styled_property2(glm::vec4,	border_rounding, 	glm::vec4(0),					on_hover, on_hold)
		widget2_styled_property2(glm::vec4,	border_color0,	 	glm::vec4(0, 0, 0, 1),			on_hover, on_hold)
		widget2_styled_property2(glm::vec4,	border_color1,	 	glm::vec4(0, 0, 0, 1),			on_hover, on_hold)
		widget2_styled_property2(glm::vec4,	border_color2,	 	glm::vec4(0, 0, 0, 1),			on_hover, on_hold)
		widget2_styled_property2(glm::vec4,	border_color3,	 	glm::vec4(0, 0, 0, 1),			on_hover, on_hold)
		widget2_styled_property2(glm::vec4,	shadow_thickness,	glm::vec4(0),					on_hover, on_hold)
		widget2_styled_property2(glm::vec4,	shadow_color,	 	glm::vec4(0, 0, 0, 1),			on_hover, on_hold)
		
		void publish(GUI2Dynamic& gui_dynamic);
	
	protected:

		void apply_properties_to(GUI2Dynamic::WindowDesc& desc);
		void apply_properties_to(GUI2Dynamic::BoxDesc& desc);
		void apply_properties_to(GUI2Dynamic::GridDesc& desc);
		void apply_properties_to(GUI2Dynamic::StackDesc& desc);
	};

	struct Grid : public StyledWidget {

		widget2_styled_property2(glm::vec4, margin,			glm::vec4(0),						on_hover, on_hold)
		widget2_styled_property2(glm::vec4, padding,		glm::vec4(0),						on_hover, on_hold)
		widget2_styled_property2(glm::vec2, target_size,	glm::vec2(128),						on_hover, on_hold)
		widget2_styled_property2(glm::vec2, min_size,		glm::vec2(GUI2Dynamic::fit),		on_hover, on_hold)
		widget2_styled_property2(glm::vec2, max_size,		glm::vec2(GUI2Dynamic::avail),		on_hover, on_hold)

		void publish(GUI2Dynamic& gui_dynamic);

	};

	struct Stack : public StyledWidget {

		widget2_styled_property2(glm::vec4,	margin,			glm::vec4(0),						on_hover, on_hold)
		widget2_styled_property2(glm::vec4,	padding,		glm::vec4(0),						on_hover, on_hold)
		widget2_styled_property2(glm::vec2,	target_size,	glm::vec2(128),						on_hover, on_hold)
		widget2_styled_property2(glm::vec2,	min_size,		glm::vec2(GUI2Dynamic::fit),		on_hover, on_hold)
		widget2_styled_property2(glm::vec2,	max_size,		glm::vec2(GUI2Dynamic::avail),		on_hover, on_hold)
		widget2_styled_property2(float,		spacing,		10,									on_hover, on_hold)

		void publish(GUI2Dynamic& gui_dynamic);
	};

	struct Window : public IOWidget {

		glm::vec2 drag_area_position;
		glm::vec2 drag_area_size;

		bool draggable				= false;
		bool dockable				= false;
		bool has_default_decoration = false;

		glm::vec2 position			= glm::vec2(100);

		void publish(GUI2Dynamic& gui_dynamic);

	private:

	};

	struct DockSurface {

		DockSurface(std::string idstr);
		void publish(GUI2Dynamic& gui_dynamic);

	private: 

	};

	struct Image : public Box {

		enum Type {
			Fit,
			Stretch,
			Custom,
		};

		std::shared_ptr<Texture2D> texture = nullptr;
		glm::vec2 uv00 = glm::vec2(0);
		glm::vec2 uv11 = glm::vec2(1);
		size_t grid_id = GUI2Dynamic::invalid_id;

		Type type = Fit;
		
		void publish(GUI2Dynamic& gui_dynamic);

	};

	struct Label : public Grid {

		font_id font = 1;
		std::u32string text;
		float text_height = 16;

		widget2_styled_property2(glm::vec4, text_color, glm::vec4(0, 0, 0, 1), on_hover, on_hold)

		Label() {

			target_size	= glm::vec2(GUI2Dynamic::fit);

		}

		void publish(GUI2Dynamic& gui_dynamic);

		void begin(GUI2Dynamic& gui_dynamic);
		bool publish_glyph(GUI2Dynamic& gui_dynamic, size_t end_index);
		float get_current_advance();
		float compute_advance(size_t end_index);
		void end(GUI2Dynamic& gui_dynamic);

	private:
		float		advance					= 0;
		glm::vec2	text_size				= glm::vec2(0);
		int32_t		last_published_index	= 0; 
	};

	struct TextInput : public Grid {

		Box	  background;
		Label label;

		std::u32string placeholder_text = U"Placeholder";
		std::u32string text = U"";

		glm::vec4 placeholder_text_color    = glm::vec4(0.3, 0.3, 0.3, 1);
		glm::vec4 selected_text_color		= glm::vec4(1, 1, 1, 1);
		glm::vec4 selected_background_color = glm::vec4(0.23, 0.48, 0.72, 1);
		glm::vec4 text_color				= glm::vec4(0.2, 0.2, 0.2, 1);
		
		std::optional<glm::vec4> on_focus_text_color					= glm::vec4(0, 0, 0, 1);
		std::optional<glm::vec4> on_focus_background_color				= std::nullopt;
		std::optional<glm::vec4> on_focus_background_border_thickness	= std::nullopt;
		std::optional<glm::vec4> on_focus_background_border_rounding	= std::nullopt;
		std::optional<glm::vec4> on_focus_background_border_color0		= glm::vec4(0.50, 0.50, 0.56, 1);
		std::optional<glm::vec4> on_focus_background_border_color1		= glm::vec4(0.50, 0.50, 0.56, 1);
		std::optional<glm::vec4> on_focus_background_border_color2		= glm::vec4(0.50, 0.50, 0.56, 1);
		std::optional<glm::vec4> on_focus_background_border_color3		= glm::vec4(0.50, 0.50, 0.56, 1);
		std::optional<glm::vec4> on_focus_background_shadow_thickness	= std::nullopt;
		std::optional<glm::vec4> on_focus_background_shadow_color		= std::nullopt;

		std::chrono::system_clock::duration text_cursor_timer_blink_period = std::chrono::milliseconds(500);
		glm::vec4 text_cursor_color = glm::vec4(0, 0, 0, 1);

		static constexpr int32_t invalid_selection_index = -1;
		int32_t selection_index_begin	= invalid_selection_index;
		int32_t selection_index_end		= invalid_selection_index;
		int32_t text_cursor_position	= 0;

		bool can_aquire_keyboard_focus = true;
		std::chrono::system_clock::time_point keyboard_focus_begin = IOEvent::invalid_time;

		TextInput() {

			target_size					= glm::vec2(400, 40);
			padding						= glm::vec4(0);

			label.target_size			= glm::vec2(GUI2Dynamic::fit);
			label.margin				= glm::vec4(8, GUI2Dynamic::avail, 8, GUI2Dynamic::avail);
			label.text_color			= glm::vec4(0.2, 0.2, 0.2, 1);

			background.color			= glm::vec4(1, 1, 1, 1);
			background.border_thickness = glm::vec4(2);
			background.border_color0	= glm::vec4(0.68, 0.71, 0.75, 1);
			background.border_color1	= glm::vec4(0.68, 0.71, 0.75, 1);
			background.border_color2	= glm::vec4(0.68, 0.71, 0.75, 1);
			background.border_color3	= glm::vec4(0.68, 0.71, 0.75, 1);
			background.target_size		= glm::vec2(GUI2Dynamic::avail, GUI2Dynamic::avail);
			background.margin			= glm::vec4(0);
		}

		void publish(GUI2Dynamic& gui_dynamic);
	
	private:
		void resolve_keyboard_io(GUI2Dynamic& gui_dynamic);
	};

	struct Slider : public Grid {

		Box head;

		glm::vec4 color;
		widget2_styled_property2(glm::vec4, filled_color, glm::vec4(0.13, 0.57, 0.59, 1), on_hover, on_hold)
		
		float value = 0.5;
		float min_value = 0;
		float max_value = 1;

		void publish(GUI2Dynamic& gui_dynamic);

	};

	struct DragFloat {

		void publish(GUI2Dynamic& gui_dynamic);

	};

	struct Button {

		void publish(GUI2Dynamic& gui_dynamic);

	};

	struct CheckBox {

		void publish(GUI2Dynamic& gui_dynamic);

	};

	struct ComboBox {

		void publish(GUI2Dynamic& gui_dynamic);

	};

	struct Menu {

		void publish(GUI2Dynamic& gui_dynamic);

	};

	struct Tab {

		void publish(GUI2Dynamic& gui_dynamic);

	};

	struct Collapsible {

		void publish(GUI2Dynamic& gui_dynamic);

	};

}

#include "GUI2Widgets_Templated.h"