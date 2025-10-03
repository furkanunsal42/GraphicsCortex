#pragma once
#include "GUI/CortexGUI.h"

#include "CortexGUIWidgets/Container/GUIWidget_Grid.h"
#include "CortexGUIWidgets/Container/GUIWidget_Stack.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Rectangle.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Label.h"

#include "Timer.h"

#include <codecvt>


namespace widget {


	class TextInput : public Grid  {
	public:
		
		WidgetHandle<widget::Label>		label			= widget::create<widget::Label>();
		WidgetHandle<widget::Rectangle> text_cursor		= widget::create<widget::Rectangle>();
		WidgetHandle<widget::Rectangle> text_selection	= widget::create<widget::Rectangle>();

		glm::vec4 on_focus_border_color0 = glm::vec4(0.50, 0.50, 0.56, 1);
		glm::vec4 on_focus_border_color1 = glm::vec4(0.50, 0.50, 0.56, 1);
		glm::vec4 on_focus_border_color2 = glm::vec4(0.50, 0.50, 0.56, 1);
		glm::vec4 on_focus_border_color3 = glm::vec4(0.50, 0.50, 0.56, 1);

		std::chrono::milliseconds text_cursor_timer_blink_period = std::chrono::milliseconds(500);
		
		glm::vec4 text_selected_color		= glm::vec4(1, 1, 1, 1);
		glm::vec4 text_color				= glm::vec4(0.2, 0.2, 0.2, 1);
		glm::vec4 placeholder_text_color	= glm::vec4(0.5, 0.5, 0.5, 1);
		
		std::u32string placeholder_text = U"Enter Text Here";
		std::u32string text				= U"";

		Timer<false> text_cursor_timer;

		size_t on_char_event	= Newsletter<>::invalid_id;
		size_t on_key_event		= Newsletter<>::invalid_id;

		static constexpr int32_t invalid_selection_index = -1;
		int32_t selection_index_begin	= invalid_selection_index;
		int32_t selection_index_end		= invalid_selection_index;
		bool text_selection_active		= false;

		int32_t text_cursor_position = 0;
		bool focused = false;

		TextInput();

		Element& get_element(glm::vec2 allocated_size);

		void gain_keyboard_focus();
		void lose_keyboard_focus();
		void poll_events(glm::vec2 absolute_position) override;

		void apply_properties_to_element(Element& element, glm::vec2 allocated_size) override;

	private:
	};
}
