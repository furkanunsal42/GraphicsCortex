#pragma once
#include "GUI2Dynamic.h"
#include <chrono>

namespace widget2 {

	class Widget {
	protected:
		size_t id = GUI2Dynamic::invalid_id;
		GUI2Dynamic::ResolvedProperties get_resolved_properties(GUI2Dynamic& gui_dynamic);
	};

	class IOWidget : protected Widget {
		using duration		= std::chrono::system_clock::duration;
		using time_point	= std::chrono::system_clock::time_point;
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

	protected:
		constexpr static time_point	invalid_time = time_point::max();
		
		void resolve_io(GUI2Dynamic& gui_dynamic);

		time_point last_hover_begin	= invalid_time;
		time_point last_hover_end	= invalid_time;
		time_point last_hold_begin	= invalid_time;
		time_point last_click		= invalid_time;
		time_point last_hold_end	= invalid_time;

	private:
		constexpr static glm::vec2	invalid_position = glm::vec2(-1);
		
		MouseState mouse_states[2] = { None, None };
		glm::vec2 cursor_position_when_hold_begin	= invalid_position;
		glm::vec2 widget_position_when_hold_begin	= invalid_position;
		
		bool was_last_click_double = false;
	};


	class StyledWidget : public IOWidget {
		using duration		= std::chrono::system_clock::duration;
		using time_point	= std::chrono::system_clock::time_point;
		using vec4			= glm::vec4;
		using optvec4		= std::optional<glm::vec4>;
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

		using duration	= std::chrono::system_clock::duration;
		using vec4		= glm::vec4;
		using vec2		= glm::vec2;
		using optvec4	= std::optional<glm::vec4>;
		using optvec2	= std::optional<glm::vec2>;

		vec4		margin									= vec4(0);
		vec2		target_size								= vec2(128);
		vec2		min_size								= vec2(GUI2Dynamic::fit);
		vec2		max_size								= vec2(GUI2Dynamic::avail);

		vec4		color									= vec4(1, 1, 1, 1);
		vec4		border_thickness						= vec4(0);
		vec4		border_rounding							= vec4(0);
		vec4		border_color0							= vec4(0, 0, 0, 1);
		vec4		border_color1							= vec4(0, 0, 0, 1);
		vec4		border_color2							= vec4(0, 0, 0, 1);
		vec4		border_color3							= vec4(0, 0, 0, 1);
		vec4		shadow_thickness						= vec4(0);
		vec4		shadow_color							= vec4(0, 0, 0, 1);
		
		optvec4		on_hover_margin							= std::nullopt;
		optvec2		on_hover_target_size					= std::nullopt;
		optvec4		on_hover_color							= std::nullopt;
		optvec4		on_hover_border_thickness				= std::nullopt;
		optvec4		on_hover_border_rounding				= std::nullopt;
		optvec4		on_hover_border_color0					= std::nullopt;
		optvec4		on_hover_border_color1					= std::nullopt;
		optvec4		on_hover_border_color2					= std::nullopt;
		optvec4		on_hover_border_color3					= std::nullopt;
		optvec4		on_hover_shadow_thickness				= std::nullopt;
		optvec4		on_hover_shadow_color					= std::nullopt;
		
		duration	on_hover_margin_transition				= duration(0);
		duration	on_hover_target_size_transition			= duration(0);
		duration	on_hover_color_transition				= duration(0);
		duration	on_hover_border_thickness_transition	= duration(0);
		duration	on_hover_border_rounding_transition		= duration(0);
		duration	on_hover_border_color0_transition		= duration(0);
		duration	on_hover_border_color1_transition		= duration(0);
		duration	on_hover_border_color2_transition		= duration(0);
		duration	on_hover_border_color3_transition		= duration(0);
		duration	on_hover_shadow_thickness_transition	= duration(0);
		duration	on_hover_shadow_color_transition		= duration(0);

		optvec4		on_hold_margin							= std::nullopt;
		optvec2		on_hold_target_size						= std::nullopt;
		optvec4		on_hold_color							= std::nullopt;
		optvec4		on_hold_border_thickness				= std::nullopt;
		optvec4		on_hold_border_rounding					= std::nullopt;
		optvec4		on_hold_border_color0					= std::nullopt;
		optvec4		on_hold_border_color1					= std::nullopt;
		optvec4		on_hold_border_color2					= std::nullopt;
		optvec4		on_hold_border_color3					= std::nullopt;
		optvec4		on_hold_shadow_thickness				= std::nullopt;
		optvec4		on_hold_shadow_color					= std::nullopt;

		duration	on_hold_margin_transition				= duration(0);
		duration	on_hold_target_size_transition			= duration(0);
		duration	on_hold_color_transition				= duration(0);
		duration	on_hold_border_thickness_transition		= duration(0);
		duration	on_hold_border_rounding_transition		= duration(0);
		duration	on_hold_border_color0_transition		= duration(0);
		duration	on_hold_border_color1_transition		= duration(0);
		duration	on_hold_border_color2_transition		= duration(0);
		duration	on_hold_border_color3_transition		= duration(0);
		duration	on_hold_shadow_thickness_transition		= duration(0);
		duration	on_hold_shadow_color_transition			= duration(0);

		void publish(GUI2Dynamic& gui_dynamic);

	protected:

		void apply_properties_to(GUI2Dynamic::WindowDesc& desc);
		void apply_properties_to(GUI2Dynamic::BoxDesc& desc);
		void apply_properties_to(GUI2Dynamic::GridDesc& desc);
		void apply_properties_to(GUI2Dynamic::StackDesc& desc);

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

	struct Label {

		std::string text;
		void publish(GUI2Dynamic& gui_dynamic);

	};

	struct TextArea {

		void publish(GUI2Dynamic& gui_dynamic);

	};

	struct Slider {

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