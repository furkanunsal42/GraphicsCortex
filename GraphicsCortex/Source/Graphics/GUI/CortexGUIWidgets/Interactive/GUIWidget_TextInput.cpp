#include "GUIWidget_TextInput.h"

#include "Clipboard.h"

widget::TextInput::TextInput() {
	
	add_column(target_size.x);
	add_row(target_size.y);

	color = glm::vec4(1, 1, 1, 1);
	border_thickness = glm::vec4(2);
	border_color0 = glm::vec4(0.68, 0.71, 0.75, 1);
	border_color1 = glm::vec4(0.68, 0.71, 0.75, 1);
	border_color2 = glm::vec4(0.68, 0.71, 0.75, 1);
	border_color3 = glm::vec4(0.68, 0.71, 0.75, 1);
	target_size = glm::vec2(400, 60);

	label->text = U"Enter Text Here";
	label->font = 1;
	label->margin = glm::vec4(8, 0, 0, 0);
	label->text_color = glm::vec4(0.2, 0.2, 0.2, 1);
	label->z = z;
	add(label, 0, 0);

	text_selection->color = glm::vec4(0.23f, 0.48f, 0.72f, 1);
	text_selection->margin = glm::vec4(0, -1, 0, -1);

	text_cursor->color = glm::vec4(0, 0, 0, 1);
	text_cursor->margin = glm::vec4(0, -1, 0, -1);
	add(text_cursor, 0, 0);

	events.subscribe([&](GUIEvent e, glm::vec2 absolute_position, glm::vec2 cursor_positions) {
		if (e == GUIEvent::HoldBegin)
			this->focused = true;
		gain_keyboard_focus();
		});

	text_cursor_timer.get_newsletter().subscribe([&]() {
		text_cursor->color.a = text_cursor->color.a == 0 ? 1 : 0;
		});

}

Element& widget::TextInput::get_element(glm::vec2 allocated_size) {

	text_cursor_position = std::clamp(text_cursor_position, 0, (int32_t)text.size());
	if (selection_index_begin	> (int32_t)text.size() ||
		selection_index_end		> (int32_t)text.size() ||
		selection_index_begin == selection_index_end
	) {
		selection_index_begin = invalid_selection_index;
		selection_index_end	  = invalid_selection_index;
	}

	if (text.size() == 0) {
		label->text = placeholder_text;
		label->text_color = placeholder_text_color;
	}
	else {
		label->text = text;
		label->text_color = text_color;
	}

	if (selection_index_begin == selection_index_end ||
		selection_index_begin == invalid_selection_index ||
		selection_index_end == invalid_selection_index) 
	{
		remove(text_selection);
		text_selection_active = false;
	}
	else if (!text_selection_active) {
		add(text_selection, 0, 0);
		text_selection_active = true;
	}
		

	label->get_element(glm::vec2(0));

	text_cursor_position = glm::clamp(text_cursor_position, 0, label->get_glyph_count());

	if (text_cursor_position == 0 && label->get_glyph_count() == 0)
		text_cursor->margin.x = label->margin.x - 2;
	else
		text_cursor->margin.x = label->get_glyph_position(text_cursor_position).x + label->margin.x - 2;

	if (selection_index_begin != invalid_selection_index && text_selection_active) {
		int32_t min_selection = std::max(std::min(selection_index_begin, selection_index_end), 0);
		int32_t max_selection = std::min(std::max(selection_index_begin, selection_index_end), label->get_glyph_count());
		
		text_selection->margin.x		= label->get_glyph_position(min_selection).x + label->margin.x;
		text_selection->target_size.x	= label->get_glyph_position(max_selection-1).x - label->get_glyph_position(min_selection).x + label->get_glyph_size(max_selection-1).x;
		text_selection->target_size.y	= element.size().y / 2 - label->text_height / 2 + 2;
	}
	else 
		text_selection->target_size = glm::vec2(-1, 0);


	text_cursor->target_size = glm::vec2(2, std::min(label->text_height + 8, allocated_size.y));

	label->margin = glm::vec4(8, element.size().y / 2 - label->text_height / 2, 0, 0);

	set_column_size(target_size.x, 0);
	set_row_size(target_size.y, 0);

	auto& result = Grid::get_element(allocated_size);

	if (text_selection_active) {
		int32_t min_selection = std::max(std::min(selection_index_begin, selection_index_end), 0);
		int32_t max_selection = std::min(std::max(selection_index_begin, selection_index_end), label->get_glyph_count());

		for (int32_t i = min_selection; i < max_selection; i++) {
			label->set_glyph_color(i, text_selected_color);
		}
	}
	
	return result;
}

void widget::TextInput::gain_keyboard_focus() {
	if (on_char_event == Newsletter<>::invalid_id) {
		on_char_event = GUI::get().get_window()->newsletters->on_char_events.subscribe([&](uint32_t character) {

			text_cursor_position = std::clamp(text_cursor_position, 0, (int32_t)text.size());
			if (selection_index_begin > (int32_t)text.size() ||
				selection_index_end > (int32_t)text.size() ||
				selection_index_begin == selection_index_end
				) {
				selection_index_begin = invalid_selection_index;
				selection_index_end = invalid_selection_index;
			}

			if (selection_index_begin != invalid_selection_index &&
				selection_index_end != invalid_selection_index) 
			{
				int32_t min_selection = std::max(std::min(selection_index_begin, selection_index_end), 0);
				int32_t max_selection = std::min(std::max(selection_index_begin, selection_index_end), label->get_glyph_count());

				text.erase(min_selection, max_selection - min_selection);

				if (selection_index_begin < selection_index_end)
					text_cursor_position -= max_selection - min_selection;
				text_cursor_position = std::clamp(text_cursor_position, 0, (int32_t)text.size());

				selection_index_begin	= invalid_selection_index;
				selection_index_end		= invalid_selection_index;
			}

			text.insert(text.begin() + text_cursor_position, character);
			text_cursor_position++;
			});
	}

	if (on_key_event == Newsletter<>::invalid_id) {
		on_key_event = GUI::get().get_window()->newsletters->on_key_events.subscribe([&](Window::KeyPressResult result) {

			text_cursor_position = std::clamp(text_cursor_position, 0, (int32_t)text.size());
			if (selection_index_begin > (int32_t)text.size() ||
				selection_index_end > (int32_t)text.size() ||
				selection_index_begin == selection_index_end
				) {
				selection_index_begin = invalid_selection_index;
				selection_index_end = invalid_selection_index;
			}

			if (result.action == Window::PressAction::RELEASE)
				return;
			if (result.key == Window::Key::LEFT_CONTROL ||
				result.key == Window::Key::RIGHT_CONTROL ||
				result.key == Window::Key::LEFT_SHIFT ||
				result.key == Window::Key::RIGHT_SHIFT)
				return;

			text_cursor->color.a = 1;
			text_cursor_timer.reset();

			if (result.key == Window::Key::BACKSPACE) {
				
				if (selection_index_begin	== invalid_selection_index &&
					selection_index_end		== invalid_selection_index
				) {
					if (text_cursor_position <= 0)
						return;
					
					if (((int32_t)result.mods & (int32_t)Window::KeyMods::CONTROL) != 0) {

						int32_t delete_end = text_cursor_position;
						delete_end--;
						while (delete_end > 0 && text[delete_end] == ' ')
							delete_end--;
						while (delete_end > 0 && text[delete_end] != ' ')
							delete_end--;
						if (text[delete_end] == ' ')
							delete_end++;

						delete_end = std::clamp(delete_end, 0, (int32_t)text.size());

						int32_t delete_begin = std::min(text_cursor_position, delete_end);
						delete_end = std::max(text_cursor_position, delete_end);

						text.erase(delete_begin, delete_end - delete_begin);

						if (selection_index_begin < selection_index_end)
							text_cursor_position -= delete_end - delete_begin;
						text_cursor_position = std::clamp(text_cursor_position, 0, (int32_t)text.size());

					}
					else {
						text_cursor_position--;
						text.erase(text.begin() + text_cursor_position);
					}
				}
				else {
					
					int32_t min_selection = std::max(std::min(selection_index_begin, selection_index_end), 0);
					int32_t max_selection = std::min(std::max(selection_index_begin, selection_index_end), label->get_glyph_count());

					text.erase(min_selection, max_selection - min_selection);

					if (selection_index_begin < selection_index_end)
						text_cursor_position -= max_selection - min_selection;
					text_cursor_position = std::clamp(text_cursor_position, 0, (int32_t)text.size());

					selection_index_begin	= invalid_selection_index;
					selection_index_end		= invalid_selection_index;
				}
			}
			else if (result.key == Window::Key::DELETE) {
				if (selection_index_begin	== invalid_selection_index &&
					selection_index_end		== invalid_selection_index
				) {
					if (text.size() <= text_cursor_position)
						return;

					if (((int32_t)result.mods & (int32_t)Window::KeyMods::CONTROL) != 0) {

						int32_t delete_end = text_cursor_position;
						
						while (delete_end < text.size() && text[delete_end] != ' ')
							delete_end++;
						while (delete_end < text.size() && text[delete_end] == ' ')
							delete_end++;

						delete_end = std::clamp(delete_end, 0, (int32_t)text.size());

						int32_t delete_begin = std::min(text_cursor_position, delete_end);
						delete_end = std::max(text_cursor_position, delete_end);

						text.erase(delete_begin, delete_end - delete_begin);

						if (selection_index_begin < selection_index_end)
							text_cursor_position -= delete_end - delete_begin;
						text_cursor_position = std::clamp(text_cursor_position, 0, (int32_t)text.size());

					}
					else {
						text.erase(text.begin() + text_cursor_position);
					}
				} 
				else {

					int32_t min_selection = std::max(std::min(selection_index_begin, selection_index_end), 0);
					int32_t max_selection = std::min(std::max(selection_index_begin, selection_index_end), label->get_glyph_count());

					text.erase(min_selection, max_selection - min_selection);

					if (selection_index_begin < selection_index_end)
						text_cursor_position -= max_selection - min_selection;
					text_cursor_position = std::clamp(text_cursor_position, 0, (int32_t)text.size());

					selection_index_begin = invalid_selection_index;
					selection_index_end = invalid_selection_index;
				}
			}
			else if (result.key == Window::Key::TAB) {
				this->focused = false;
			}
			else if (result.key == Window::Key::ENTER) {
				this->focused = false;
				selection_index_begin	= invalid_selection_index;
				selection_index_end		= invalid_selection_index;
			}
			else if (result.key == Window::Key::KP_ENTER) {
				this->focused = false;
				selection_index_begin	= invalid_selection_index;
				selection_index_end		= invalid_selection_index;
			}
			else if (result.key == Window::Key::ESCAPE) {
				this->focused = false;
				selection_index_begin	= invalid_selection_index;
				selection_index_end		= invalid_selection_index;
			}
			else if (result.key == Window::Key::RIGHT) {

				if (selection_index_begin != invalid_selection_index &&
					selection_index_end != invalid_selection_index &&
					(((int32_t)result.mods & (int32_t)Window::KeyMods::SHIFT) == 0)
					)
				{
					int32_t min_selection = std::max(std::min(selection_index_begin, selection_index_end), 0);
					int32_t max_selection = std::min(std::max(selection_index_begin, selection_index_end), label->get_glyph_count());

					text_cursor_position = max_selection - 1;

					selection_index_begin	= invalid_selection_index;
					selection_index_end		= invalid_selection_index;
				}

				int32_t initial_cursor_position = text_cursor_position;

				if (((int32_t)result.mods & (int32_t)Window::KeyMods::CONTROL) != 0) {
					while (text_cursor_position < text.size() && text[text_cursor_position] != ' ')
						text_cursor_position++;
					while (text_cursor_position < text.size() && text[text_cursor_position] == ' ')
						text_cursor_position++;
				}
				else
					text_cursor_position++;

				text_cursor_position = std::clamp(text_cursor_position, 0, (int32_t)text.size());

				if (((int32_t)result.mods & (int32_t)Window::KeyMods::SHIFT) != 0) {
					if (selection_index_begin == invalid_selection_index)
						selection_index_begin = initial_cursor_position;
					selection_index_end = text_cursor_position;
				}
			}
			else if (result.key == Window::Key::LEFT) {
				
				if (selection_index_begin != invalid_selection_index &&
					selection_index_end != invalid_selection_index &&
					(((int32_t)result.mods & (int32_t)Window::KeyMods::SHIFT) == 0)
					)
				{
					int32_t min_selection = std::max(std::min(selection_index_begin, selection_index_end), 0);
					int32_t max_selection = std::min(std::max(selection_index_begin, selection_index_end), label->get_glyph_count());

					text_cursor_position = min_selection + 1;

					selection_index_begin	= invalid_selection_index;
					selection_index_end		= invalid_selection_index;
				}

				int32_t initial_cursor_position = text_cursor_position;
				
				if (((int32_t)result.mods & (int32_t)Window::KeyMods::CONTROL) != 0) {
					text_cursor_position--;
					while (text_cursor_position > 0 && text[text_cursor_position] == ' ')
						text_cursor_position--;
					while (text_cursor_position > 0 && text[text_cursor_position] != ' ')
						text_cursor_position--;
					if (text[text_cursor_position] == ' ')
						text_cursor_position++;
				}
				else
					text_cursor_position--;

				text_cursor_position = std::clamp(text_cursor_position, 0, (int32_t)text.size());

				if (((int32_t)result.mods & (int32_t)Window::KeyMods::SHIFT) != 0) {
					if (selection_index_begin == invalid_selection_index)
						selection_index_begin = initial_cursor_position;
					selection_index_end = text_cursor_position;
				}
			}
			else if (result.key == Window::Key::V && ((int32_t)result.mods & (int32_t)Window::KeyMods::CONTROL) != 0) {
				std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
				std::u32string s32 = conv.from_bytes(clipboard::get());

				text.insert(text_cursor_position, s32);
				text_cursor_position += s32.size();
			}
			else if (result.key == Window::Key::C && ((int32_t)result.mods & (int32_t)Window::KeyMods::CONTROL) != 0) {

				if (selection_index_begin == invalid_selection_index ||
					selection_index_end	  == invalid_selection_index)
					return;

				int32_t min_selection = std::max(std::min(selection_index_begin, selection_index_end), 0);
				int32_t max_selection = std::min(std::max(selection_index_begin, selection_index_end), label->get_glyph_count());

				std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
				clipboard::set(conv.to_bytes(text.substr(min_selection, max_selection)));
			}
			else if (result.key == Window::Key::A && ((int32_t)result.mods & (int32_t)Window::KeyMods::CONTROL) != 0) {

				selection_index_begin = 0;
				selection_index_end = text.size();
				text_cursor_position = text.size();
			}


			});
	}

	text_cursor->color.a = 1;
}

void widget::TextInput::lose_keyboard_focus() {
	if (on_char_event != Newsletter<>::invalid_id) {
		GUI::get().get_window()->newsletters->on_char_events.unsubscribe(on_char_event);
		on_char_event = Newsletter<>::invalid_id;
	}

	if (on_key_event != Newsletter<>::invalid_id) {
		GUI::get().get_window()->newsletters->on_key_events.unsubscribe(on_key_event);
		on_key_event = Newsletter<>::invalid_id;
	}

	text_cursor->color.a = 0;
}

void widget::TextInput::poll_events(glm::vec2 absolute_position) {
	text_cursor_position = std::clamp(text_cursor_position, 0, (int32_t)text.size());
	if (selection_index_begin	> (int32_t)text.size() ||
		selection_index_end		> (int32_t)text.size() ||
		selection_index_begin == selection_index_end
	) {
		selection_index_begin = invalid_selection_index;
		selection_index_end	  = invalid_selection_index;
	}

	text_cursor_timer.handle_events();

	Widget::poll_events(absolute_position);

	AABB2 self_aabb(absolute_position, absolute_position + element.size());
	glm::vec2 cursor_pos = GUI::get().get_window()->get_cursor_position();
	bool cursor_on_widget = self_aabb.does_contain(cursor_pos);

	if (GUI::get().get_window()->get_mouse_press_inpulse(Window::MouseButton::LEFT) && !cursor_on_widget) {
		this->focused = false;
		lose_keyboard_focus();
	}

	if (!focused)
		lose_keyboard_focus();

	if (focused && !text_cursor_timer.is_running())
		text_cursor_timer.start_periodic(text_cursor_timer_blink_period);

	if (!focused && text_cursor_timer.is_running())
		text_cursor_timer.stop();
}

void widget::TextInput::apply_properties_to_element(Element& element, glm::vec2 allocated_size) 
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