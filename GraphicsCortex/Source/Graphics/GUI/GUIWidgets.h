#pragma once
#include "GUIDynamic.h"
#include <chrono>
#include "Font.h"

namespace widget2 {

	class Widget {
	public:
		GUIDynamic::ResolvedProperties get_resolved_properties(GUIDynamic& gui_dynamic);
	protected:
		size_t id = GUIDynamic::invalid_id;
	};

	struct IOEvent {
		
		GUIDynamic::time_point begin_time	= GUIDynamic::invalid_time;
		GUIDynamic::time_point end_time	= GUIDynamic::invalid_time;

		void impulse(GUIDynamic& gui_dynamic);
		void start(GUIDynamic& gui_dynamic);
		void finish(GUIDynamic& gui_dynamic);

		bool is_active();
		bool is_activated_now(GUIDynamic& gui_dynamic);
		bool is_deactivated_now(GUIDynamic& gui_dynamic);
	};

	class IOWidget : public Widget {
		using duration		= std::chrono::system_clock::duration;
	public:
		constexpr static int32_t mouse_left = 0;
		constexpr static int32_t mouse_right = 1;
		
		glm::vec2	get_cursor_position_when_hold_begin(int32_t mouse_button = mouse_left);
		glm::vec2	get_widget_position_when_hold_begin(int32_t mouse_button = mouse_left);
		bool		is_topmost_widget();

		duration double_click_max_delay = std::chrono::milliseconds(500);
		float carry_begin_min_offset = 10;
		bool ignore_mouse_if_not_topmost_widget = true;

		IOEvent hover;
		IOEvent hold;
		IOEvent press;
		IOEvent carry;
		IOEvent click;
		IOEvent doubleclick;

	protected:
		
		void resolve_io(GUIDynamic& gui_dynamic);
		
	private:

		constexpr static glm::vec2	invalid_position = glm::vec2(-1);
		
		glm::vec2 cursor_position_when_hold_begin	= invalid_position;
		glm::vec2 widget_position_when_hold_begin	= invalid_position;
		
		bool was_last_click_double = false;
		bool is_top_most = false;
	};

	template<typename T>
	struct StyleProperty {
		using duration = std::chrono::system_clock::duration;

		StyleProperty() = default;
		StyleProperty(T value);

		operator T();

		T value = T(0);

		StyleProperty& transition(T default_value);

		StyleProperty& transition(const IOEvent& event, T transition_value, duration transition_duration = std::chrono::milliseconds(1));

	private:

		float get_t(const IOEvent& event, duration transition_duration);
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

		void publish(GUIDynamic& gui_dynamic);

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
	//	void publish(GUIDynamic& gui_dynamic);
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
		
		void publish(GUIDynamic& gui_dynamic);
	
	protected:

		void apply_properties_to(GUIDynamic::WindowDesc& desc);
		void apply_properties_to(GUIDynamic::BoxDesc& desc);
		void apply_properties_to(GUIDynamic::GridDesc& desc);
		void apply_properties_to(GUIDynamic::StackDesc& desc);
	};

	struct Grid : public IOWidget {

		StyleProperty<glm::vec4>	margin;
		StyleProperty<glm::vec4>	padding;
		StyleProperty<glm::vec2>	target_size;
		StyleProperty<glm::vec2>	min_size;
		StyleProperty<glm::vec2>	max_size;

		void publish(GUIDynamic& gui_dynamic);

	};

	struct Stack : public IOWidget {

		StyleProperty<glm::vec4>	margin;
		StyleProperty<glm::vec4>	padding;
		StyleProperty<glm::vec2>	target_size;
		StyleProperty<glm::vec2>	min_size;
		StyleProperty<glm::vec2>	max_size;
		StyleProperty<float>		spacing;

		void publish(GUIDynamic& gui_dynamic);
	};

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
		
		void publish(GUIDynamic& gui_dynamic);

	private:
		size_t grid_id = GUIDynamic::invalid_id;
	};

	struct Label : public Grid {

		font_id font;
		float text_height;
		StyleProperty<glm::vec4> text_color;

		void publish(GUIDynamic& gui_dynamic, const std::u32string& text);

		void begin(GUIDynamic& gui_dynamic, const std::u32string& text);
		bool publish_glyph(GUIDynamic& gui_dynamic, size_t end_index, const std::u32string& text);
		float get_current_advance();
		float compute_advance(size_t end_index, const std::u32string& text);
		void end(GUIDynamic& gui_dynamic, const std::u32string& text);

	private:
		float		advance					= 0;
		glm::vec2	text_size				= glm::vec2(0);
		int32_t		last_published_index	= 0; 
	};

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

		void publish(GUIDynamic& gui_dynamic, std::u32string& text);
	
	private:
		void resolve_keyboard_io(GUIDynamic& gui_dynamic, std::u32string& text);
	};

	struct Slider : public Grid {

		Box head;
		Box background;
		Box filled_bar;

		float min_value = 0;
		float max_value = 1;

		void publish(GUIDynamic& gui_dynamic, float& value);

	};

	struct DragFloat : public TextInput {

		float sensitivity	= 0.01;
		float min_value		= std::numeric_limits<float>::min();
		float max_value		= std::numeric_limits<float>::max();

		void publish(GUIDynamic& gui_dynamic, float& value);

	protected:
		
		std::u32string string = U"";
		float old_value = 0;

		void publish(GUIDynamic& gui_dynamic, std::u32string& text) = delete;

		constexpr static glm::vec2 invalid_cursor_position = glm::vec2(-1);
		glm::vec2 cursor_position_when_grabbed_publish = invalid_cursor_position;
	};

	struct Button : public Grid {

		Box background;
		Label label;

		std::u32string text = U"button";

		void publish(GUIDynamic& gui_dynamic);

	};

	struct ImageButton : public Grid {

		Image	image;
		Box		background;

		void publish(GUIDynamic& gui_dynamic);

	};

	struct CheckBox : public Grid {

		Image	image;
		Box		background;

		void publish(GUIDynamic& gui_dynamic, bool& checked);

		IOEvent check;
	};

	struct ComboBox {

		void publish(GUIDynamic& gui_dynamic);

	};

	struct Menu {

		void publish(GUIDynamic& gui_dynamic);
		void end(GUIDynamic& gui_dynamic);

	};

	struct Tab {

		void publish(GUIDynamic& gui_dynamic);
		void end(GUIDynamic& gui_dynamic);

	};

	struct Collapsible {

		void publish(GUIDynamic& gui_dynamic);
		void end(GUIDynamic& gui_dynamic);


	};

}

#include "GUIWidget_Templated.h"
