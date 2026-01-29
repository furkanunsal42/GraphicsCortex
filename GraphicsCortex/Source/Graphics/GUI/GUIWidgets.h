#pragma once
#include "GUIDynamic.h"
#include <chrono>
#include "Font.h"

namespace widget {

	class Widget {
	public:
		GUIDynamic::ResolvedProperties get_resolved_properties(GUIDynamic& gui_dynamic);
		size_t id = GUIDynamic::invalid_id;
	protected:
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

		duration double_click_max_delay = std::chrono::milliseconds(500);
		float carry_begin_min_offset = 10;

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
		T& apply(T& widget) = delete;

	};

	extern DefaultStyle global_style;

	struct Window : public IOWidget {

		bool dockable				= false;
		bool has_native_decoration	= false;
		bool is_resizable			= false;

		std::string name = "CortexGUI Window";

		StyleProperty<glm::vec4> padding;
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

		StyleProperty<glm::vec2> target_size = glm::vec2(GUIDynamic::fit);
		StyleProperty<glm::vec2> position = glm::vec2(100);

		IOEvent maximize;
		IOEvent restore;
		IOEvent iconify;
		IOEvent should_close;

		void publish_begin(GUIDynamic& gui_dynamic);
		void publish_end(GUIDynamic& gui_dynamic);

		void publish_menubar_begin(GUIDynamic& gui_dynamic);
		void publish_menubar_end(GUIDynamic& gui_dynamic);

		void drag(GUIDynamic& gui_dynamic, IOWidget& widget);

		void desire_iconify();
		void desire_maximal();
		void desire_restore();
		void desire_maximal_restore_swap();

	private:

		std::optional<glm::vec2> restored_previous_position = std::nullopt;
		std::optional<glm::vec2> restored_previous_size = std::nullopt;
		int32_t restore_invalidation_counter = 0;

		bool does_desire_iconify = false;
		bool does_desire_maximal = false;
		bool does_desire_maximal_restore_swap = false;
		bool does_desire_restore = true;

		size_t resolution_change_newsletter = Newsletter<>::invalid_id;
		size_t position_change_newsletter = Newsletter<>::invalid_id;

		bool menubar_published = false;
		glm::vec2 window_position_when_drag_begin = glm::vec2(-1000);

	};

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
		bool pass_through_events;

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
		bool pass_through_events;

		void publish_begin(GUIDynamic& gui_dynamic);
		void publish_end(GUIDynamic& gui_dynamic);

		void make_column_resize_grip(int32_t column_index);
		void make_row_resize_grip(int32_t row_index);

	};

	struct Stack : public IOWidget {

		StyleProperty<glm::vec4>	margin;
		StyleProperty<glm::vec4>	padding;
		StyleProperty<glm::vec2>	target_size;
		StyleProperty<glm::vec2>	min_size;
		StyleProperty<glm::vec2>	max_size;
		StyleProperty<float>		spacing;
		bool is_vertical;
		bool pass_through_events;

		void publish_begin(GUIDynamic& gui_dynamic);
		void publish_end(GUIDynamic& gui_dynamic);

	};

	struct Container : public Grid {

		void publish_begin(GUIDynamic& gui_dynamic);
		void publish_end(GUIDynamic& gui_dynamic);
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
		Type type;
		
		void publish(GUIDynamic& gui_dynamic);

	private:
		size_t grid_id = GUIDynamic::invalid_id;
	};

	struct Label : public Container {

		font_id font;
		float text_height;
		StyleProperty<glm::vec4> text_color;

		void publish(GUIDynamic& gui_dynamic, const std::u32string& text);

		void publish_begin(GUIDynamic& gui_dynamic, const std::u32string& text);
		bool publish_glyph(GUIDynamic& gui_dynamic, size_t end_index, const std::u32string& text);
		void publish_end(GUIDynamic& gui_dynamic, const std::u32string& text);
		
		float get_published_advance();
		float compute_total_advance(size_t end_index, const std::u32string& text);

	private:
		float		advance					= 0;
		glm::vec2	text_size				= glm::vec2(0);
		int32_t		last_published_index	= 0; 
	};

	struct TextInput : public Container {

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

	struct Slider : public Container {

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

		int32_t precision	= 3;

		void publish(GUIDynamic& gui_dynamic, float& value);

	protected:
		
		std::u32string string = U"";
		float old_value = 0;

		void publish(GUIDynamic& gui_dynamic, std::u32string& text) = delete;

		constexpr static glm::vec2 invalid_cursor_position = glm::vec2(-1);
		glm::vec2 cursor_position_when_grabbed_publish = invalid_cursor_position;
	};

	struct Button : public Container {

		Box background;
		Label label;

		std::u32string text = U"button";

		void publish(GUIDynamic& gui_dynamic);

	};

	struct ImageButton : public Container {

		Image	image;
		Box		background;

		void publish(GUIDynamic& gui_dynamic);

	};

	struct CheckBox : public Container {

		Image	image;
		Box		background;

		void publish(GUIDynamic& gui_dynamic, bool& checked);

		IOEvent check;
	};

	struct ComboBox : public Container {

		Box		background;
		Label	label;

		Window	dropdown;
		Stack	dropdown_stack;

		std::u32string text = U"ComboBox";
		
		ComboBox() = default;
		ComboBox(std::u32string text);

		void publish_begin(GUIDynamic& gui_dynamic);
		void publish_end(GUIDynamic& gui_dynamic);

		IOEvent item_selected;
		IOEvent drop;
	};

	struct ComboBoxItem : Container {

		Box		background;
		Label	label;

		std::u32string text = U"Combo Item";

		ComboBoxItem() = default;
		ComboBoxItem(std::u32string text);

		void select(GUIDynamic& gui_dynamic, ComboBox& owner_combobox);
		void publish(GUIDynamic& gui_dynamic, ComboBox& owner_combobox);

	};

	struct Menu : public Container {

		Box background;
		Label label;

		Window	dropdown;
		Stack	dropdown_stack;
		
		IOEvent item_selected;
		IOEvent drop;
		std::u32string text = U"Menu";

		Menu() = default;
		Menu(std::u32string text);

		void publish_begin(GUIDynamic& gui_dynamic);
		void publish_end(GUIDynamic& gui_dynamic);

	};

	struct MenuItem : public Container {

		Box		background;
		Label	label;

		std::u32string text = U"Menu Item";

		MenuItem() = default;
		MenuItem(std::u32string text);

		void select(GUIDynamic& gui_dynamic, Menu& owner_menu);
		void publish(GUIDynamic& gui_dynamic, Menu& owner_menu);

	};

	struct WindowControls : public Stack {
		
		ImageButton minimize_button;
		ImageButton restore_button;
		ImageButton close_button;

		void publish(GUIDynamic& gui_dynamic);
		void manage(GUIDynamic& gui_dynamic, widget::Window& window);

	};

	struct MenuBar : public Grid {

		Box background;
		Stack manu_stack;
		
		WindowControls window_controls;
		
		void publish_begin(GUIDynamic& gui_dynamic);
		void publish_end(GUIDynamic& gui_dynamic);
	};

	struct ScrollContainer : public Container {

		Box background_vertical;
		Box head_vertical;
		ImageButton button_left;
		ImageButton button_right;

		Box background_horizontal;
		Box head_horizontal;
		ImageButton button_down;
		ImageButton button_up;

		enum ScrollMode {
			Always,
			Overflow,
			Never,
		};

		ScrollMode vertical_scroll_mode = Overflow;
		ScrollMode horizontal_scroll_mode = Never;

		void publish_begin(GUIDynamic& gui_dynamic);
		void publish_end(GUIDynamic& gui_dynamic);

	};

	struct Tab {

		void publish(GUIDynamic& gui_dynamic, size_t& selected_id);

	};

	struct Collapsible {

		void publish(GUIDynamic& gui_dynamic, bool& is_active);

	};

}

#include "GUIWidget_Templated.h"
