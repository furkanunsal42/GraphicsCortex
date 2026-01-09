#pragma once
#include "GUI2Dynamic.h"
#include <chrono>
#include "Font.h"

#include <any>

namespace widget2 {

	class Widget {
	public:
		GUI2Dynamic::ResolvedProperties get_resolved_properties(GUI2Dynamic& gui_dynamic);
	protected:
		size_t id = GUI2Dynamic::invalid_id;
	};

	struct IOEvent {
		
		GUI2Dynamic::time_point begin_time	= GUI2Dynamic::invalid_time;
		GUI2Dynamic::time_point end_time	= GUI2Dynamic::invalid_time;

		void impulse(GUI2Dynamic& gui_dynamic);
		void start(GUI2Dynamic& gui_dynamic);
		void finish(GUI2Dynamic& gui_dynamic);

		bool is_active();
		bool is_activated_now(GUI2Dynamic& gui_dynamic);
		bool is_deactivated_now(GUI2Dynamic& gui_dynamic);
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
		bool		is_topmost_widget();

		duration double_click_max_delay = std::chrono::milliseconds(500);
		float carry_begin_min_offset = 10;
		bool ignore_mouse_if_not_topmost_widget = true;

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
		bool is_top_most = false;
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

		StyleProperty& transition(const IOEvent& event, T transition_value, duration transition_duration = std::chrono::milliseconds(1)) {
			float t = glm::clamp(get_t(event, transition_duration), 0.0f, 1.0f);
			value = value * (1 - t) + transition_value * t;
			return *this;
		}

	private:

		float get_t(const IOEvent& event, duration transition_duration) {
			
			if (event.begin_time == GUI2Dynamic::invalid_time && event.end_time == GUI2Dynamic::invalid_time)
				return 0;

			if (transition_duration.count() == 0)
				return event.end_time > event.begin_time ? 1 : 0;

			auto now = std::chrono::system_clock::now();

			duration time_passed			= std::min(now - event.begin_time,				transition_duration);
			duration time_passed_hovering	= std::min(event.end_time - event.begin_time,	transition_duration);
			duration time_passed_recovering = std::min(now - event.end_time,				transition_duration);

			bool happening =
				(event.begin_time != GUI2Dynamic::invalid_time && event.end_time == GUI2Dynamic::invalid_time) ||
				(event.begin_time != GUI2Dynamic::invalid_time && event.end_time != GUI2Dynamic::invalid_time && event.begin_time > event.end_time);

			bool recovering =
				event.begin_time != GUI2Dynamic::invalid_time	&&
				event.end_time != GUI2Dynamic::invalid_time		&&
				event.begin_time < event.end_time				&&
				now - event.end_time < time_passed_hovering;

			duration relevent_duration = happening ? time_passed : time_passed_hovering - time_passed_recovering;

			return 1 - (transition_duration - relevent_duration).count() / (float)transition_duration.count();

		}
	};

	class DefaultStyle {
	public:
		
		template<typename T>
		void apply(T& widget) = delete;

	};

	struct Window : public IOWidget {

		glm::vec2 drag_area_position;
		glm::vec2 drag_area_size;

		bool draggable;
		bool dockable;
		bool has_default_decoration;

		glm::vec2 position = glm::vec2(100);

		void publish(GUI2Dynamic& gui_dynamic);

	private:

	};

	template<>
	inline void DefaultStyle::apply<Window>(Window& widget) {
		widget.draggable = true;
		widget.dockable = false;
		widget.has_default_decoration = false;
	}

	//struct DockSurface {
	//
	//	DockSurface(std::string idstr);
	//	void publish(GUI2Dynamic& gui_dynamic);
	//
	//private:
	//
	//};

	struct Box : public IOWidget {
		
		StyleProperty<glm::vec4> margin;
		StyleProperty<glm::vec2> target_size;
		StyleProperty<glm::vec2> min_size;
		StyleProperty<glm::vec2> max_size;
		StyleProperty<glm::vec4> color;
		StyleProperty<glm::vec4> border_thickness;
		StyleProperty<glm::vec4> border_rounding;
		StyleProperty<glm::vec4> border_color0;
		StyleProperty<glm::vec4> border_color1;
		StyleProperty<glm::vec4> border_color2;
		StyleProperty<glm::vec4> border_color3;
		StyleProperty<glm::vec4> shadow_thickness;
		StyleProperty<glm::vec4> shadow_color;
		
		void publish(GUI2Dynamic& gui_dynamic);
	
	protected:

		void apply_properties_to(GUI2Dynamic::WindowDesc& desc);
		void apply_properties_to(GUI2Dynamic::BoxDesc& desc);
		void apply_properties_to(GUI2Dynamic::GridDesc& desc);
		void apply_properties_to(GUI2Dynamic::StackDesc& desc);
	};

	template<>
	inline void DefaultStyle::apply<Box>(Box& widget) {
		widget.margin			= glm::vec4(0);
		widget.target_size		= glm::vec2(128);	
		widget.min_size			= glm::vec2(GUI2Dynamic::fit);	
		widget.max_size			= glm::vec2(GUI2Dynamic::avail);	
		widget.color			= glm::vec4(1, 1, 1, 1);
		widget.border_thickness	= glm::vec4(0);			
		widget.border_rounding	= glm::vec4(0);		
		widget.border_color0	= glm::vec4(0, 0, 0, 1);		
		widget.border_color1	= glm::vec4(0, 0, 0, 1);		
		widget.border_color2	= glm::vec4(0, 0, 0, 1);		
		widget.border_color3	= glm::vec4(0, 0, 0, 1);		
		widget.shadow_thickness	= glm::vec4(0);			
		widget.shadow_color		= glm::vec4(0, 0, 0, 1);	
	}

	struct Grid : public IOWidget {

		StyleProperty<glm::vec4>	margin;
		StyleProperty<glm::vec4>	padding;
		StyleProperty<glm::vec2>	target_size;
		StyleProperty<glm::vec2>	min_size;
		StyleProperty<glm::vec2>	max_size;

		void publish(GUI2Dynamic& gui_dynamic);

	};

	template<>
	inline void DefaultStyle::apply<Grid>(Grid& widget) {
		widget.margin		= glm::vec4(0);	
		widget.padding		= glm::vec4(0);
		widget.target_size	= glm::vec2(GUI2Dynamic::fit);
		widget.min_size		= glm::vec2(GUI2Dynamic::fit);	
		widget.max_size		= glm::vec2(GUI2Dynamic::avail);		
	}

	struct Stack : public IOWidget {

		StyleProperty<glm::vec4>	margin;
		StyleProperty<glm::vec4>	padding;
		StyleProperty<glm::vec2>	target_size;
		StyleProperty<glm::vec2>	min_size;
		StyleProperty<glm::vec2>	max_size;
		StyleProperty<float>		spacing;

		void publish(GUI2Dynamic& gui_dynamic);
	};

	template<>
	inline void DefaultStyle::apply<Stack>(Stack& widget) {
		widget.margin		= glm::vec4(0);
		widget.padding		= glm::vec4(0);
		widget.target_size	= glm::vec2(128);	
		widget.min_size		= glm::vec2(GUI2Dynamic::fit);	
		widget.max_size		= glm::vec2(GUI2Dynamic::avail);	
		widget.spacing		= 10;
	}

	struct Image : public Box {

		enum Type {
			Fit,
			Stretch,
			Custom,
		};

		std::shared_ptr<Texture2D> texture = nullptr;
		glm::vec2 uv00;
		glm::vec2 uv11;
		Type type;
		
		void publish(GUI2Dynamic& gui_dynamic);

	private:
		size_t grid_id = GUI2Dynamic::invalid_id;
	};

	template<>
	inline void DefaultStyle::apply<Image>(Image& widget) {
		apply<Box>(widget);
		widget.uv00 = glm::vec2(0);
		widget.uv11 = glm::vec2(1);
		widget.type = Image::Fit;
	}

	struct Label : public Grid {

		font_id font;
		float text_height;
		StyleProperty<glm::vec4> text_color;

		void publish(GUI2Dynamic& gui_dynamic, const std::u32string& text);

		void begin(GUI2Dynamic& gui_dynamic, const std::u32string& text);
		bool publish_glyph(GUI2Dynamic& gui_dynamic, size_t end_index, const std::u32string& text);
		float get_current_advance();
		float compute_advance(size_t end_index, const std::u32string& text);
		void end(GUI2Dynamic& gui_dynamic, const std::u32string& text);

	private:
		float		advance					= 0;
		glm::vec2	text_size				= glm::vec2(0);
		int32_t		last_published_index	= 0; 
	};

	template<>
	inline void DefaultStyle::apply<Label>(Label& widget) {
		apply<Grid>(widget);
		widget.font = 1;
		widget.text_height = 16;
		widget.text_color = glm::vec4(0, 0, 0, 1);
		widget.target_size = glm::vec2(GUI2Dynamic::fit);
	}

	struct TextInput : public Grid {

		Box	  background;
		Label label;

		StyleProperty<glm::vec4> placeholder_text_color;
		StyleProperty<glm::vec4> selected_text_color;
		StyleProperty<glm::vec4> selected_background_color;
		StyleProperty<glm::vec4> text_color;
		StyleProperty<glm::vec4> text_cursor_color;
		std::chrono::system_clock::duration text_cursor_timer_blink_period;

		std::u32string placeholder_text = U"Placeholder";

		static constexpr int32_t invalid_selection_index = -1;
		int32_t selection_index_begin = invalid_selection_index;
		int32_t selection_index_end = invalid_selection_index;
		int32_t text_cursor_position = 0;

		bool can_aquire_keyboard_focus = true;
		IOEvent focus;

		void publish(GUI2Dynamic& gui_dynamic, std::u32string& text);
	
	private:
		void resolve_keyboard_io(GUI2Dynamic& gui_dynamic, std::u32string& text);
	};


	template<>
	inline void DefaultStyle::apply<TextInput>(TextInput& widget) {
		apply<Grid>(widget);
		apply(widget.background);
		apply(widget.label);

		widget.placeholder_text_color 			= glm::vec4(0.3, 0.3, 0.3, 1);
		widget.selected_text_color 				= glm::vec4(1, 1, 1, 1);
		widget.selected_background_color 		= glm::vec4(0.23, 0.48, 0.72, 1);
		widget.text_color 						= glm::vec4(0.2, 0.2, 0.2, 1);
		widget.text_cursor_color 				= glm::vec4(0, 0, 0, 1);
		widget.text_cursor_timer_blink_period	= std::chrono::milliseconds(500);

		widget.target_size 						= glm::vec2(400, 40);
		widget.padding 							= glm::vec4(0);

		widget.label.target_size 				= glm::vec2(GUI2Dynamic::fit);
		widget.label.margin 					= glm::vec4(8, GUI2Dynamic::avail, 8, GUI2Dynamic::avail);
		widget.label.text_color 				= glm::vec4(0.2, 0.2, 0.2, 1);
		
		widget.background.color 				= glm::vec4(1, 1, 1, 1);
		widget.background.border_thickness 		= glm::vec4(2);
		widget.background.border_color0 		= glm::vec4(0.68, 0.71, 0.75, 1);
		widget.background.border_color1 		= glm::vec4(0.68, 0.71, 0.75, 1);
		widget.background.border_color2 		= glm::vec4(0.68, 0.71, 0.75, 1);
		widget.background.border_color3 		= glm::vec4(0.68, 0.71, 0.75, 1);
		widget.background.target_size 			= glm::vec2(GUI2Dynamic::avail, GUI2Dynamic::avail);
		widget.background.margin				= glm::vec4(0);

		widget.label.text_color.transition(widget.focus, glm::vec4(0, 0, 0, 1));
		widget.background.border_color0.transition(widget.focus, glm::vec4(0.50, 0.50, 0.56, 1));
		widget.background.border_color1.transition(widget.focus, glm::vec4(0.50, 0.50, 0.56, 1));
		widget.background.border_color2.transition(widget.focus, glm::vec4(0.50, 0.50, 0.56, 1));
		widget.background.border_color3.transition(widget.focus, glm::vec4(0.50, 0.50, 0.56, 1));
	}


	struct Slider : public Grid {

		Box head;
		Box background;
		Box filled_bar;

		float min_value = 0;
		float max_value = 1;

		IOEvent grab;

		void publish(GUI2Dynamic& gui_dynamic, float& value);

	};

	template<>
	inline void DefaultStyle::apply<Slider>(Slider& widget) {
		apply<Grid>(widget);
		apply(widget.head);
		apply(widget.background);
		apply(widget.filled_bar);

		widget.target_size	= glm::vec2(400, 26);

		widget.head.target_size			= glm::vec2(widget.target_size.value.y, widget.target_size.value.y);
		widget.head.color				= glm::vec4(0.88, 0.88, 0.88, 1);
		widget.head.border_rounding		= glm::vec4(16);
		widget.head.color.transition(widget.head.hover, glm::vec4(0.78, 0.78, 0.92, 1), std::chrono::milliseconds(100));
		widget.head.color.transition(widget.head.hold,	glm::vec4(0.82, 0.82, 0.82, 1), std::chrono::milliseconds(100));

		widget.background.target_size	= glm::vec2(GUI2Dynamic::avail, 4);
		widget.background.color			= glm::vec4(0.90, 0.90, 0.90, 1);
		widget.background.margin		= glm::vec4(0, GUI2Dynamic::avail, 0, GUI2Dynamic::avail);

		widget.filled_bar.target_size	= glm::vec2(0, widget.background.target_size.value.y);
		widget.filled_bar.color			= glm::vec4(0.45f, 0.59f, 0.65f, 1);
		widget.filled_bar.margin		= glm::vec4(0, GUI2Dynamic::avail, 0, GUI2Dynamic::avail);
	}

	struct DragFloat : public TextInput {

		float sensitivity	= 0.01;
		float min_value		= std::numeric_limits<float>::min();
		float max_value		= std::numeric_limits<float>::max();

		void publish(GUI2Dynamic& gui_dynamic, float& value);

		IOEvent grab;

	protected:
		
		std::u32string string = U"";
		void publish(GUI2Dynamic& gui_dynamic, std::u32string& text);

		constexpr static glm::vec2 invalid_cursor_position = glm::vec2(-1);
		glm::vec2 cursor_position_when_grabbed_publish = invalid_cursor_position;
	};

	template<>
	inline void DefaultStyle::apply<DragFloat>(DragFloat& widget) {
		apply<TextInput>(widget);
		widget.target_size = glm::vec2(120, 30);
		
	}

	struct Button : public Grid {

		Box background;
		Label label;

		std::u32string text = U"button";

		void publish(GUI2Dynamic& gui_dynamic);

	};

	template<>
	inline void DefaultStyle::apply<Button>(Button& widget) {
		apply(widget.background);
		apply(widget.label);

		widget.target_size					= glm::vec2(GUI2Dynamic::fit);

		widget.background.target_size		= glm::vec2(GUI2Dynamic::avail);
		
		widget.label.margin					= glm::vec4(GUI2Dynamic::avail);

	}

	struct ImageButton : public Grid {

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
