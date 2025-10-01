#pragma once
#include "GUI/CortexGUI.h"

#include "CortexGUIWidgets/Container/GUIWidget_Grid.h"
#include "CortexGUIWidgets/Container/GUIWidget_Stack.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Rectangle.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Label.h"

#include "Timer.h"

#include <codecvt>

namespace widget {

	class TextInput : public Grid {
	public:

		WidgetHandle<widget::Label> label = widget::create<widget::Label>();
		WidgetHandle<widget::Rectangle> selector = widget::create<widget::Rectangle>();

		glm::vec4 on_focus_border_color0 = glm::vec4(0.50, 0.50, 0.56, 1);
		glm::vec4 on_focus_border_color1 = glm::vec4(0.50, 0.50, 0.56, 1);
		glm::vec4 on_focus_border_color2 = glm::vec4(0.50, 0.50, 0.56, 1);
		glm::vec4 on_focus_border_color3 = glm::vec4(0.50, 0.50, 0.56, 1);

		int32_t cursor_position = 0;
		bool focused = false;

		std::chrono::milliseconds cursor_timer_blink_period = std::chrono::milliseconds(500);
		Timer<true> cursor_timer;

		TextInput() {
			color = glm::vec4(1, 1, 1, 1);
			border_thickness = glm::vec4(2);
			border_color0 = glm::vec4(0.68, 0.71, 0.75, 1);
			border_color1 = glm::vec4(0.68, 0.71, 0.75, 1);
			border_color2 = glm::vec4(0.68, 0.71, 0.75, 1);
			border_color3 = glm::vec4(0.68, 0.71, 0.75, 1);
			target_size = glm::vec2(400, 60);

			add_column(target_size.x);
			add_row(target_size.y);

			label->text = U"Enter Text Here";
			label->font = 1;
			label->margin = glm::vec4(8, 0, 0, 0);
			label->text_color = glm::vec4(0.2, 0.2, 0.2, 1);
			label->z = z;
			add(label, 0, 0);

			selector->color = glm::vec4(0, 0, 0, 1);
			selector->margin = glm::vec4(0, -1, 0, -1);

			add(selector, 0, 0);

			events.subscribe([&](GUIEvent e) {
				if (e == GUIEvent::Clicked)
					this->focused = true;
					gain_keyboard_focus();
				});

			cursor_timer.get_newsletter().subscribe([&]() {
				selector->color.a = selector->color.a == 0 ? 1 : 0;
				});
			
		}

		Element& get_element(glm::vec2 allocated_size) {

			label->get_element(glm::vec2(0));
			cursor_position = glm::clamp(cursor_position, 0, label->get_glyph_count());
			
			if (cursor_position == 0 && label->get_glyph_count() == 0)
				selector->margin.x = label->margin.x - 2;
			//else if (cursor_position == label->get_glyph_count())
			//	selector->margin.x = label->get_glyph_position(cursor_position-1).x + label->get_glyph_size(cursor_position - 1).x + label->margin.x - 2;
			else
				selector->margin.x = label->get_glyph_position(cursor_position).x + label->margin.x - 2;

			selector->target_size = glm::vec2(2, std::min(label->text_height + 8, allocated_size.y));

			label->margin = glm::vec4(8, element.size().y / 2 - label->text_height / 2, 0, 0);

			set_column_size(target_size.x, 0);
			set_row_size(target_size.y, 0);

			return Grid::get_element(allocated_size);
		}

		size_t on_char_event	= Newsletter<>::invalid_id;
		size_t on_key_event		= Newsletter<>::invalid_id;

		void gain_keyboard_focus() {
			if (on_char_event == Newsletter<>::invalid_id) {
				on_char_event = GUI::get().get_window()->newsletters->on_char_events.subscribe([&](uint32_t character) {
					label->text.insert(label->text.begin() + cursor_position, character);
					cursor_position++;
					});
			}
			
			if (on_key_event == Newsletter<>::invalid_id) {
				on_key_event = GUI::get().get_window()->newsletters->on_key_events.subscribe([&](Window::KeyPressResult result) {
					
					if (result.action == Window::PressAction::RELEASE)
						return;
					
					selector->color.a = 1;
					cursor_timer.reset();

					if (result.key == Window::Key::BACKSPACE) {
						if (cursor_position <= 0)
							return;
						cursor_position--;
						label->text.erase(label->text.begin() + cursor_position);
					}
					else if (result.key == Window::Key::TAB) {
						this->focused = false;
					}
					else if (result.key == Window::Key::DELETE) {
						if (label->text.size() <= cursor_position)
							return;

						label->text.erase(label->text.begin() + cursor_position);
					}
					else if (result.key == Window::Key::ENTER) {
						this->focused = false;
					}
					else if (result.key == Window::Key::KP_ENTER) {
						this->focused = false;
					}
					else if (result.key == Window::Key::ESCAPE) {
						this->focused = false;
					}
					else if (result.key == Window::Key::RIGHT) {
						cursor_position++;
					}
					else if (result.key == Window::Key::LEFT) {
						cursor_position--;
					}
					else if (result.key == Window::Key::V && ((int32_t)result.mods & (int32_t)Window::KeyMods::CONTROL) != 0) {
						std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
						std::u32string s32 = conv.from_bytes(clipboard::get());
						
						label->text.insert(cursor_position, s32);
						cursor_position += s32.size();
					}

					});
			}

			selector->color.a = 1;
		}

		void lose_keyboard_focus() {
			if (on_char_event != Newsletter<>::invalid_id) {
				GUI::get().get_window()->newsletters->on_char_events.unsubscribe(on_char_event);
				on_char_event = Newsletter<>::invalid_id;
			}

			if (on_key_event != Newsletter<>::invalid_id) {
				GUI::get().get_window()->newsletters->on_key_events.unsubscribe(on_key_event);
				on_key_event = Newsletter<>::invalid_id;
			}

			selector->color.a = 0;
		}

		void poll_events(glm::vec2 absolute_position) override {
			
			Widget::poll_events(absolute_position);

			AABB2 self_aabb(absolute_position, absolute_position + element.size());
			glm::vec2 cursor_pos = GUI::get().get_window()->get_cursor_position();
			bool cursor_on_widget = self_aabb.does_contain(cursor_pos);

			if (GUI::get().get_window()->get_mouse_release_inpulse(Window::MouseButton::LEFT) && !cursor_on_widget) {
				this->focused = false;
				lose_keyboard_focus();
			}

			if (!focused)
				lose_keyboard_focus();

			if (focused && !cursor_timer.is_running())
				cursor_timer.start_periodic(cursor_timer_blink_period);
			
			if (!focused && cursor_timer.is_running())
				cursor_timer.stop();
		}

		void apply_properties_to_element(Element& element, glm::vec2 allocated_size) override
		{
			glm::vec4& border_color0_to_use = focused ? on_focus_border_color0 : border_color0;
			glm::vec4& border_color1_to_use = focused ? on_focus_border_color1 : border_color1;
			glm::vec4& border_color2_to_use = focused ? on_focus_border_color2 : border_color2;
			glm::vec4& border_color3_to_use = focused ? on_focus_border_color3 : border_color3;

			element.size()				= allocated_size;
			element.color()				= get_property(color,					on_hover_color,				last_hover_begin, last_hover_end, on_hover_color_transition,			on_hold_color,				last_hold_begin, last_hold_end, on_hold_color_transition			);
			element.border_rounding()	= get_property(border_rounding,			on_hover_border_rounding,	last_hover_begin, last_hover_end, on_hover_border_rounding_transition,	on_hold_border_rounding,	last_hold_begin, last_hold_end, on_hold_border_rounding_transition	); 
			element.border_thickness()	= get_property(border_thickness,		on_hover_border_thickness,	last_hover_begin, last_hover_end, on_hover_border_thickness_transition, on_hold_border_thickness,	last_hold_begin, last_hold_end, on_hold_border_thickness_transition	); 
			element.border_color0()		= get_property(border_color0_to_use,	on_hover_border_color0,		last_hover_begin, last_hover_end, on_hover_border_color0_transition,	on_hold_border_color0,		last_hold_begin, last_hold_end, on_hold_border_color0_transition	); 
			element.border_color1()		= get_property(border_color1_to_use,	on_hover_border_color1,		last_hover_begin, last_hover_end, on_hover_border_color1_transition,	on_hold_border_color1,		last_hold_begin, last_hold_end, on_hold_border_color1_transition	); 
			element.border_color2()		= get_property(border_color2_to_use,	on_hover_border_color2,		last_hover_begin, last_hover_end, on_hover_border_color2_transition,	on_hold_border_color2,		last_hold_begin, last_hold_end, on_hold_border_color2_transition	); 
			element.border_color3()		= get_property(border_color3_to_use,	on_hover_border_color3,		last_hover_begin, last_hover_end, on_hover_border_color3_transition,	on_hold_border_color3,		last_hold_begin, last_hold_end, on_hold_border_color3_transition	); 
			element.shadow_thickness()	= get_property(shadow_thickness,		on_hover_shadow_thickness,	last_hover_begin, last_hover_end, on_hover_shadow_thickness_transition, on_hold_shadow_thickness,	last_hold_begin, last_hold_end, on_hold_shadow_thickness_transition	); 
			element.shadow_color()		= get_property(shadow_color,			on_hover_shadow_color,		last_hover_begin, last_hover_end, on_hover_shadow_color_transition,		on_hold_shadow_color,		last_hold_begin, last_hold_end, on_hold_shadow_color_transition		); 
			element.z() = z;
		}

	private:
	};
}
