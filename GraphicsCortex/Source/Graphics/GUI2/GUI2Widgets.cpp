#include "GUI2Widgets.h"
#include "WindowBoundGlobalResources.h"
#include "Window.h"
#include "Clipboard.h"

#include <codecvt>

GUI2Dynamic::ResolvedProperties widget2::Widget::get_resolved_properties(GUI2Dynamic& gui_dynamic)
{
	return gui_dynamic.get_resolved_properties(id);
}

widget2::IOWidget::MouseState widget2::IOWidget::get_mouse_state(int32_t mouse_button)
{
	if (mouse_button >= 2 || mouse_button < 0)
	{
		std::cout << "[GUI Error] widget::IOWidget::get_mouse_state() is called for (mouse" << mouse_button << ") but only (mouse0) and (mouse1) are supported" << std::endl;
		
	}
	return mouse_states[mouse_button];
}

glm::vec2 widget2::IOWidget::get_cursor_position_when_hold_begin(int32_t mouse_button) {
	return cursor_position_when_hold_begin;
}

glm::vec2 widget2::IOWidget::get_widget_position_when_hold_begin(int32_t mouse_button) {
	return widget_position_when_hold_begin;
}

bool widget2::IOWidget::is_topmost_widget()
{
	return is_top_most;
}


void widget2::IOWidget::resolve_io(GUI2Dynamic& gui_dynamic)
{
	GUI2::MouseEvent event = gui_dynamic.get_resolved_properties(id).event;

	MouseState current_state = mouse_states[0];
	
	if (ignore_mouse_if_not_topmost_widget && !is_topmost_widget())
		event = GUI2::MouseEvent::None;

	switch (current_state) {

	case None : 
	{
		if((event & GUI2::MouseEvent::None) != 0)
			current_state = None;
		if((event & GUI2::MouseEvent::Hover) != 0)
			current_state = Enter;
		if ((event & GUI2::MouseEvent::LeftPress) != 0)
			current_state = Enter;
		if((event & GUI2::MouseEvent::LeftRelease) != 0)
			current_state = Enter; //?
		if ((event & GUI2::MouseEvent::LeftHold) != 0)
			current_state = Enter;
		break;
	}
	case Enter : 
	{
		if((event & GUI2::MouseEvent::None) != 0)
			current_state = Leave;
		if((event & GUI2::MouseEvent::Hover) != 0)
			current_state = Hover;
		if((event & GUI2::MouseEvent::LeftPress) != 0)
			current_state = HoldBegin;
		if((event & GUI2::MouseEvent::LeftRelease) != 0)
			current_state = PressEnd; //?
		if((event & GUI2::MouseEvent::LeftHold) != 0)
			current_state = PressBegin; //? 
		break;
	}
	case Hover : 
	{
		if((event & GUI2::MouseEvent::None) != 0)
			current_state = Leave;
		if((event & GUI2::MouseEvent::Hover) != 0)
			current_state = Hover;
		if((event & GUI2::MouseEvent::LeftPress) != 0)
			current_state = HoldBegin;
		if((event & GUI2::MouseEvent::LeftRelease) != 0)
			current_state = PressEnd;
		if((event & GUI2::MouseEvent::LeftHold) != 0)
			current_state = PressBegin;
		break;
	}
	case Leave : 
	{
		if((event & GUI2::MouseEvent::None) != 0)
			current_state = None;
		if((event & GUI2::MouseEvent::Hover) != 0)
			current_state = Enter;
		if((event & GUI2::MouseEvent::LeftPress) != 0)
			current_state = Enter; //?
		if((event & GUI2::MouseEvent::LeftRelease) != 0)
			current_state = PressEnd;  //?
		if((event & GUI2::MouseEvent::LeftHold) != 0)
			current_state = PressBegin;
		break;
	}
	case PressBegin : 
	{
		
		if((event & GUI2::MouseEvent::None) != 0)
			current_state = Leave;
		if((event & GUI2::MouseEvent::Hover) != 0)
			current_state = Hover; // ?
		if((event & GUI2::MouseEvent::LeftPress) != 0)
			current_state = HoldBegin; // ?
		if((event & GUI2::MouseEvent::LeftRelease) != 0)
			current_state = PressEnd;
		if((event & GUI2::MouseEvent::LeftHold) != 0)
			current_state = Press;
		break;
	}
	case Press : 
	{
		
		if((event & GUI2::MouseEvent::None) != 0)
			current_state = Leave;
		if((event & GUI2::MouseEvent::Hover) != 0)
			current_state = Hover; // ?
		if((event & GUI2::MouseEvent::LeftPress) != 0)
			current_state = HoldBegin; // ?
		if((event & GUI2::MouseEvent::LeftRelease) != 0)
			current_state = PressEnd; //?
		if((event & GUI2::MouseEvent::LeftHold) != 0)
			current_state = Press;
		break;
	}
	case PressEnd : 
	{
		
		if((event & GUI2::MouseEvent::None) != 0)
			current_state = Leave;
		if((event & GUI2::MouseEvent::Hover) != 0)
			current_state = Hover;
		if((event & GUI2::MouseEvent::LeftPress) != 0)
			current_state = HoldBegin;
		if((event & GUI2::MouseEvent::LeftRelease) != 0)
			current_state = PressEnd; //?
		if((event & GUI2::MouseEvent::LeftHold) != 0)
			current_state = HoldBegin; // ?
		break;
	}
	case HoldBegin : 
	{
		
		if((event & GUI2::MouseEvent::None) != 0)
			current_state = Leave;
		if((event & GUI2::MouseEvent::Hover) != 0)
			current_state = HoldEnd; // ?
		if((event & GUI2::MouseEvent::LeftPress) != 0)
			current_state = HoldBegin; // ?
		if((event & GUI2::MouseEvent::LeftRelease) != 0)
			current_state = HoldEnd;
		if((event & GUI2::MouseEvent::LeftHold) != 0)
			current_state = Hold;
		break;
	}
	case Hold : 
	{
		
		if ((event & GUI2::MouseEvent::None) != 0) {
			hold.finish(gui_dynamic);
			//hold.end_time = gui_dynamic.get_current_frame_timepoint();
			current_state = Leave; // HoldLeave?
		}
		if((event & GUI2::MouseEvent::Hover) != 0)
			current_state = HoldEnd;
		if((event & GUI2::MouseEvent::LeftPress) != 0)
			current_state = Hold; // ?
		if((event & GUI2::MouseEvent::LeftRelease) != 0)
			current_state = HoldEnd;
		if((event & GUI2::MouseEvent::LeftHold) != 0)
			current_state = Hold;
		break;
	}
	case HoldEnd : 
	{
		
		if((event & GUI2::MouseEvent::None) != 0)
			current_state = Click;
		if((event & GUI2::MouseEvent::Hover) != 0)
			current_state = Click;
		if((event & GUI2::MouseEvent::LeftPress) != 0)
			current_state = Click;
		if((event & GUI2::MouseEvent::LeftRelease) != 0)
			current_state = Click;
		if((event & GUI2::MouseEvent::LeftHold) != 0)
			current_state = Click;
		break;
	}
	case Click : 
	{
		
		if((event & GUI2::MouseEvent::None) != 0)
			current_state = Leave;
		if((event & GUI2::MouseEvent::Hover) != 0)
			current_state = Hover;
		if((event & GUI2::MouseEvent::LeftPress) != 0)
			current_state = HoldBegin;
		if((event & GUI2::MouseEvent::LeftRelease) != 0)
			current_state = PressEnd; // ?
		if((event & GUI2::MouseEvent::LeftHold) != 0)
			current_state = PressBegin; // ?
		break;
	}
	case DoubleClick : 
	{
		
		if((event & GUI2::MouseEvent::None) != 0)
			current_state = Leave;
		if((event & GUI2::MouseEvent::Hover) != 0)
			current_state = Hover;
		if((event & GUI2::MouseEvent::LeftPress) != 0)
			current_state = HoldBegin;
		if((event & GUI2::MouseEvent::LeftRelease) != 0)
			current_state = PressEnd; // ?
		if((event & GUI2::MouseEvent::LeftHold) != 0)
			current_state = PressBegin; // ?
		break;
	}
	case CarryBegin : 
	{
		
		if((event & GUI2::MouseEvent::None) != 0)
			current_state = CarryEnd;
		if((event & GUI2::MouseEvent::Hover) != 0)
			current_state = CarryEnd;
		if((event & GUI2::MouseEvent::LeftPress) != 0)
			current_state = CarryEnd;
		if((event & GUI2::MouseEvent::LeftRelease) != 0)
			current_state = CarryEnd;
		if((event & GUI2::MouseEvent::LeftHold) != 0)
			current_state = Carry;
		break;
	}
	case Carry : 
	{
		
		if((event & GUI2::MouseEvent::None) != 0)
			current_state = CarryEnd; // ?
		if((event & GUI2::MouseEvent::Hover) != 0)
			current_state = CarryEnd;
		if((event & GUI2::MouseEvent::LeftPress) != 0)
			current_state = CarryEnd;
		if((event & GUI2::MouseEvent::LeftRelease) != 0)
			current_state = CarryEnd;
		if((event & GUI2::MouseEvent::LeftHold) != 0)
			current_state = Carry;
		break;
	}
	case CarryEnd : 
	{
		
		if((event & GUI2::MouseEvent::None) != 0)
			current_state = HoldEnd;
		if((event & GUI2::MouseEvent::Hover) != 0)
			current_state = HoldEnd;
		if((event & GUI2::MouseEvent::LeftPress) != 0)
			current_state = HoldEnd;
		if((event & GUI2::MouseEvent::LeftRelease) != 0)
			current_state = HoldEnd; // ?
		if((event & GUI2::MouseEvent::LeftHold) != 0)
			current_state = HoldEnd; // ?
		break;
	}
	};

	if (current_state == Enter)
		hover.start(gui_dynamic);
	
	if (current_state == Leave)
		hover.finish(gui_dynamic);

	if (current_state == HoldBegin) {
		widget_position_when_hold_begin = gui_dynamic.get_resolved_properties(id).position;
		cursor_position_when_hold_begin = gui_dynamic.get_io_state().mouse_position;
		hold.start(gui_dynamic);
	}
	
	if (current_state == HoldEnd)
		hold.finish(gui_dynamic);

	if (current_state == HoldBegin || current_state == Hold) {
		if (glm::dot(cursor_position_when_hold_begin - gui_dynamic.get_io_state().mouse_position, cursor_position_when_hold_begin - gui_dynamic.get_io_state().mouse_position) >= carry_begin_min_offset * carry_begin_min_offset) {
			current_state = CarryBegin;
		}
	}

	if (current_state == Click) {
		if (click.begin_time != GUI2Dynamic::invalid_time && (std::chrono::system_clock::now() - click.begin_time) <= double_click_max_delay && !was_last_click_double) {
			current_state = DoubleClick;
		}
		else {
			click.begin_time = std::chrono::system_clock::now();
			was_last_click_double = false;
		}
	}

	if (current_state == DoubleClick) {
		click.impulse(gui_dynamic);
		was_last_click_double = true;
	}

	mouse_states[0] = current_state;

	is_top_most = gui_dynamic.get_resolved_properties(id).level == gui_dynamic.get_levels_under_cursor() - 1;
}

void widget2::IOEvent::impulse(GUI2Dynamic& gui_dynamic) {
	start(gui_dynamic);
	finish(gui_dynamic);
}

void widget2::IOEvent::start(GUI2Dynamic& gui_dynamic)
{
	begin_time = gui_dynamic.get_current_frame_timepoint();
}

void widget2::IOEvent::finish(GUI2Dynamic& gui_dynamic)
{
	end_time = gui_dynamic.get_current_frame_timepoint();
}

bool widget2::IOEvent::is_active()
{
	if (begin_time == GUI2Dynamic::invalid_time && end_time == GUI2Dynamic::invalid_time)
		return false;
	if (begin_time == GUI2Dynamic::invalid_time && end_time != GUI2Dynamic::invalid_time)
		return false;
	if (begin_time != GUI2Dynamic::invalid_time && end_time == GUI2Dynamic::invalid_time)
		return true;
	return (begin_time > end_time);
}

bool widget2::IOEvent::is_activated_now(GUI2Dynamic& gui_dynamic)
{
	return begin_time == gui_dynamic.get_current_frame_timepoint();
}

bool widget2::IOEvent::is_deactivated_now(GUI2Dynamic& gui_dynamic)
{
	return end_time == gui_dynamic.get_current_frame_timepoint();
}

void widget2::Box::publish(GUI2Dynamic& gui_dynamic) {

	resolve_io(gui_dynamic);

	gui_dynamic.box_begin(id);

	gui_dynamic.box_prop().margin			=  margin;
	gui_dynamic.box_prop().target_size		=  target_size;
	gui_dynamic.box_prop().color			=  color;
	gui_dynamic.box_prop().border_thickness	=  border_thickness;
	gui_dynamic.box_prop().border_rounding	=  border_rounding;
	gui_dynamic.box_prop().border_color0	=  border_color0;
	gui_dynamic.box_prop().border_color1	=  border_color1;
	gui_dynamic.box_prop().border_color2	=  border_color2;
	gui_dynamic.box_prop().border_color3	=  border_color3;
	gui_dynamic.box_prop().shadow_thickness	=  shadow_thickness;
	gui_dynamic.box_prop().shadow_color		=  shadow_color;

	//gui_dynamic.box_end();
}

void widget2::Box::apply_properties_to(GUI2Dynamic::WindowDesc& desc)
{
	desc.padding			= glm::vec4(0);
	desc.target_size		=  target_size;
	desc.color				=  color;
	desc.border_thickness	=  border_thickness;
	desc.border_rounding	=  border_rounding;
	desc.border_color0		=  border_color0;
	desc.border_color1		=  border_color1;
	desc.border_color2		=  border_color2;
	desc.border_color3		=  border_color3;
	desc.shadow_thickness	=  shadow_thickness;
	desc.shadow_color		=  shadow_color;
}

void widget2::Box::apply_properties_to(GUI2Dynamic::BoxDesc& desc)
{
	desc.margin				=  margin;
	desc.target_size		=  target_size;
	desc.color				=  color;
	desc.border_thickness	=  border_thickness;
	desc.border_rounding	=  border_rounding;
	desc.border_color0		=  border_color0;
	desc.border_color1		=  border_color1;
	desc.border_color2		=  border_color2;
	desc.border_color3		=  border_color3;
	desc.shadow_thickness	=  shadow_thickness;
	desc.shadow_color		=  shadow_color;
}

void widget2::Box::apply_properties_to(GUI2Dynamic::GridDesc& desc)
{
	desc.margin				=  margin;
	desc.target_size		=  target_size;
	desc.padding			=  glm::vec4(0);
}

void widget2::Box::apply_properties_to(GUI2Dynamic::StackDesc& desc)
{
	desc.margin				=  margin;
	desc.target_size		=  target_size;
	desc.padding			=  glm::vec4(0);
	desc.spacing			=  0;
}

void widget2::Grid::publish(GUI2Dynamic& gui_dynamic) {

	resolve_io(gui_dynamic);

	gui_dynamic.grid_begin(id);

	gui_dynamic.grid_prop().margin		= margin;
	gui_dynamic.grid_prop().padding		= padding;
	gui_dynamic.grid_prop().target_size = target_size;
	
	gui_dynamic.grid_region(glm::ivec2(0));
}

void widget2::Stack::publish(GUI2Dynamic& gui_dynamic) {

	resolve_io(gui_dynamic);

	gui_dynamic.stack_begin(id);

	gui_dynamic.stack_prop().margin			= margin;
	gui_dynamic.stack_prop().padding		= padding;
	gui_dynamic.stack_prop().target_size	= target_size;
	gui_dynamic.stack_prop().spacing		= spacing;
}

void widget2::Window::publish(GUI2Dynamic& gui_dynamic) {
	
	resolve_io(gui_dynamic);

	if (draggable && get_mouse_state() == widget2::IOWidget::Carry) {
		position =
			get_widget_position_when_hold_begin() +
			gui_dynamic.get_io_state().mouse_position -
			get_cursor_position_when_hold_begin();
	}

	gui_dynamic.window_begin(id)
		.set_is_resizable(false)
		.set_is_decorated(false)
		.set_position(position)
		.set_color(glm::vec4(0.98, 0.98, 0.98, 1));

}

void widget2::Image::publish(GUI2Dynamic& gui_dynamic)
{
	if (texture == nullptr) {
		Box::publish(gui_dynamic);
		gui_dynamic.box_prop().texture_handle = 0;
		gui_dynamic.box_prop().uv00 = uv00;
		gui_dynamic.box_prop().uv11 = uv11;
		return;
	}

	if (type == Stretch) {
		Box::publish(gui_dynamic);
		gui_dynamic.box_prop().texture_handle = texture->texture_handle;
		gui_dynamic.box_prop().uv00 = uv00;
		gui_dynamic.box_prop().uv11 = uv11;
	}
	else if (type == Fit) {
		
		auto& desc = gui_dynamic.grid_begin(grid_id);
		apply_properties_to(desc);
		
		GUI2Dynamic::ResolvedProperties properties = gui_dynamic.get_resolved_properties(grid_id);
		glm::vec2 size = glm::max(properties.size, get_resolved_properties(gui_dynamic).size);

		gui_dynamic.grid_add_column(size.x);
		gui_dynamic.grid_add_row(size.y);
		gui_dynamic.grid_region(glm::ivec2(0));

		Box::publish(gui_dynamic);

		glm::vec2 self_size = size;
		glm::vec2 texture_size = texture->get_size();

		float self_aspect_ratio = self_size.x / self_size.y;
		float texture_aspect_ratio = texture_size.x / texture_size.y;

		glm::vec2 clipped_size;

		if (self_aspect_ratio >= texture_aspect_ratio) {
			clipped_size.y = self_size.y;
			clipped_size.x = clipped_size.y * texture_aspect_ratio;
			
			gui_dynamic.box_prop().margin.x = (self_size.x - clipped_size.x) / 2;
		}
		else {
			clipped_size.x = self_size.x;
			clipped_size.y = clipped_size.x / texture_aspect_ratio;
			gui_dynamic.box_prop().margin.y = (self_size.y - clipped_size.y) / 2;
		}

		gui_dynamic.box_prop().target_size = clipped_size;

		gui_dynamic.box_prop().texture_handle = texture->texture_handle;
		gui_dynamic.box_prop().uv00 = uv00;
		gui_dynamic.box_prop().uv11 = uv11;
		
		gui_dynamic.grid_end();
	}
}

void widget2::Label::publish(GUI2Dynamic& gui_dynamic, const std::u32string& text)
{
	if (active_global_resources == nullptr || !FontBank::get().does_font_exist(font))
		return;

	if (text.size() == 0)
		return;

	begin(gui_dynamic, text);
	end(gui_dynamic, text);
}

void widget2::Label::begin(GUI2Dynamic& gui_dynamic, const std::u32string& text) {
	
	if (active_global_resources == nullptr || !FontBank::get().does_font_exist(font))
		return;

	if (text.size() == 0)
		return;

	advance = 0;
	text_size = glm::vec2(0);
	last_published_index = 0;

	Grid::publish(gui_dynamic);
	gui_dynamic.grid_region(glm::ivec2(0));

}

bool widget2::Label::publish_glyph(GUI2Dynamic& gui_dynamic, size_t end_index, const std::u32string& text) {
	
	bool published_at_least_once = false;

	for (last_published_index; last_published_index < std::min(end_index, text.size()); last_published_index++) {

		published_at_least_once = true;

		FontBank::get().get_font(font);

		float font_size = FontBank::get().get_font(font).font_size;
		glm::vec2 atlas_size = FontBank::get().get_font(font).atlas->get_size();
		auto& table = FontBank::get().get_font(font).glyph_table.at(text[last_published_index]);

		glm::vec2 inverted_offset = glm::vec2(table.offset.x, -table.offset.y);

		glm::vec2 position = glm::vec2(advance, 0) + inverted_offset * atlas_size / font_size * text_height + glm::vec2(0, text_height);
		glm::vec2 size = glm::max(glm::vec2(0), (table.coords_hi - table.coords_low) * atlas_size / font_size * text_height);

		gui_dynamic.box_begin()
			.set_target_size(size)
			.set_margin(glm::vec4(position.x, position.y, 0, 0))
			.set_uv00(glm::vec2(table.coords_hi.x, 1 - table.coords_hi.y))
			.set_uv11(glm::vec2(table.coords_low.x, 1 - table.coords_low.y))
			.set_color(text_color)
			.set_texture_handle(FontBank::get().get_font(font).atlas->texture_handle);

		text_size.y = glm::max(text_size.y, size.y);
		text_size.x = position.x + size.x;

		advance += table.advance * atlas_size.x / font_size * text_height;
	}

	return published_at_least_once;
}

float widget2::Label::get_current_advance()
{
	return advance;
}

float widget2::Label::compute_advance(size_t end_index, const std::u32string& text)
{
	if (active_global_resources == nullptr || !FontBank::get().does_font_exist(font))
		return 0;

	if (text.size() == 0)
		return 0;

	float computed_advance = 0;

	for (int32_t i = 0; i < std::min(end_index, text.size()); i++) {

		FontBank::get().get_font(font);

		float font_size = FontBank::get().get_font(font).font_size;
		glm::vec2 atlas_size = FontBank::get().get_font(font).atlas->get_size();
		auto& table = FontBank::get().get_font(font).glyph_table.at(text[i]);
		
		computed_advance += table.advance * atlas_size.x / font_size * text_height;
	}

	return computed_advance;
}

void widget2::Label::end(GUI2Dynamic& gui_dynamic, const std::u32string& text) {

	if (active_global_resources == nullptr || !FontBank::get().does_font_exist(font))
		return;

	if (text.size() == 0)
		return;

	publish_glyph(gui_dynamic, text.size(), text);

	gui_dynamic.grid_add_column(text_size.x);
	gui_dynamic.grid_add_row(text_size.y);
	gui_dynamic.grid_prop().target_size = text_size;
	gui_dynamic.grid_end();

}

void widget2::TextInput::publish(GUI2Dynamic& gui_dynamic, std::u32string& text)
{
	ignore_mouse_if_not_topmost_widget = false;

	if (can_aquire_keyboard_focus) {
		if ((get_mouse_state(0) & Click) == Click)
			focus.start(gui_dynamic);

		if (get_mouse_state(0) == None && gui_dynamic.get_io_state().mouse_state & GUI2::MouseEvent::LeftRelease) {
			focus.finish(gui_dynamic);
			selection_index_begin	= invalid_selection_index;
			selection_index_end		= invalid_selection_index;
		}
	}

	if (focus.is_active())
		resolve_keyboard_io(gui_dynamic, text);

	std::u32string& text_to_use	= text.size() == 0 && !focus.is_active() ? placeholder_text : text;
	label.text_color	= text.size() == 0 && !focus.is_active() ? placeholder_text_color : text_color;
	
	Grid::publish(gui_dynamic);
	gui_dynamic.grid_region(glm::ivec2(0));
	
	background.publish(gui_dynamic);
	
	if (selection_index_begin != invalid_selection_index && selection_index_end != invalid_selection_index) {
		int32_t min = std::min(selection_index_begin, selection_index_end);
		int32_t max = std::max(selection_index_begin, selection_index_end);
		float advance_begin = label.compute_advance(min, text_to_use);
		float advance_end = label.compute_advance(max, text_to_use);

		gui_dynamic.box_begin()
			.set_color(selected_background_color)
			.set_target_size(glm::vec2(advance_end - advance_begin, label.text_height + 8))
			.set_margin(glm::vec4(advance_begin + label.margin.value.x, GUI2Dynamic::avail, 0, GUI2Dynamic::avail));
	}

	label.begin(gui_dynamic, text_to_use);
	if (selection_index_begin != invalid_selection_index && selection_index_end != invalid_selection_index) {
		int32_t min = std::min(selection_index_begin, selection_index_end);
		int32_t max = std::max(selection_index_begin, selection_index_end);

		label.publish_glyph(gui_dynamic, min, text_to_use);
		for (int32_t i = min + 1; i <= max; i++) {
			if (label.publish_glyph(gui_dynamic, i, text_to_use))
				gui_dynamic.box_prop().set_color(selected_text_color);
		}
	}
	label.end(gui_dynamic, text_to_use);

	gui_dynamic.grid_add_column(GUI2Dynamic::avail);
	gui_dynamic.grid_add_row(GUI2Dynamic::avail);
	gui_dynamic.grid_end();
}

void widget2::TextInput::resolve_keyboard_io(GUI2Dynamic& gui_dynamic, std::u32string& text) {
	
	for (GUI2::KeyboardEvent& e : gui_dynamic.get_io_state().keyboard_events) {
		if (e.data.index() == 0) {

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
				int32_t max_selection = std::min(std::max(selection_index_begin, selection_index_end), (int32_t)text.size());

				text.erase(min_selection, max_selection - min_selection);

				if (selection_index_begin < selection_index_end)
					text_cursor_position -= max_selection - min_selection;
				text_cursor_position = std::clamp(text_cursor_position, 0, (int32_t)text.size());

				selection_index_begin = invalid_selection_index;
				selection_index_end = invalid_selection_index;
			}

			text.insert(text.begin() + text_cursor_position, std::get<uint32_t>(e.data));
			text_cursor_position++;
		}
		else {
			auto result = std::get<::Window::KeyPressResult>(e.data);
			
			text_cursor_position = std::clamp(text_cursor_position, 0, (int32_t)text.size());
			if (selection_index_begin > (int32_t)text.size() ||
				selection_index_end > (int32_t)text.size() ||
				selection_index_begin == selection_index_end
				) {
				selection_index_begin = invalid_selection_index;
				selection_index_end = invalid_selection_index;
			}

			if (result.action == ::Window::PressAction::RELEASE)
				return;
			if (result.key == ::Window::Key::LEFT_CONTROL ||
				result.key == ::Window::Key::RIGHT_CONTROL ||
				result.key == ::Window::Key::LEFT_SHIFT ||
				result.key == ::Window::Key::RIGHT_SHIFT)
				return;

			//text_cursor_color.a = 1;
			//text_cursor_timer.reset();

			if (result.key == ::Window::Key::BACKSPACE) {

				if (selection_index_begin == invalid_selection_index &&
					selection_index_end == invalid_selection_index
					) {
					if (text_cursor_position <= 0)
						return;

					if (((int32_t)result.mods & (int32_t)::Window::KeyMods::CONTROL) != 0) {

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
					int32_t max_selection = std::min(std::max(selection_index_begin, selection_index_end), (int32_t)text.size());

					text.erase(min_selection, max_selection - min_selection);

					if (selection_index_begin < selection_index_end)
						text_cursor_position -= max_selection - min_selection;
					text_cursor_position = std::clamp(text_cursor_position, 0, (int32_t)text.size());

					selection_index_begin = invalid_selection_index;
					selection_index_end = invalid_selection_index;
				}
			}
			else if (result.key == ::Window::Key::DELETE) {
				if (selection_index_begin == invalid_selection_index &&
					selection_index_end == invalid_selection_index
					) {
					if (text.size() <= text_cursor_position)
						return;

					if (((int32_t)result.mods & (int32_t)::Window::KeyMods::CONTROL) != 0) {

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
					int32_t max_selection = std::min(std::max(selection_index_begin, selection_index_end), (int32_t)text.size());

					text.erase(min_selection, max_selection - min_selection);

					if (selection_index_begin < selection_index_end)
						text_cursor_position -= max_selection - min_selection;
					text_cursor_position = std::clamp(text_cursor_position, 0, (int32_t)text.size());

					selection_index_begin = invalid_selection_index;
					selection_index_end = invalid_selection_index;
				}
			}
			else if (result.key == ::Window::Key::TAB) {
				focus.end_time = std::chrono::system_clock::now();
			}
			else if (result.key == ::Window::Key::ENTER) {
				focus.finish(gui_dynamic);
				selection_index_begin = invalid_selection_index;
				selection_index_end = invalid_selection_index;
			}
			else if (result.key == ::Window::Key::KP_ENTER) {
				focus.finish(gui_dynamic);
				selection_index_begin = invalid_selection_index;
				selection_index_end = invalid_selection_index;
			}
			else if (result.key == ::Window::Key::ESCAPE) {
				focus.finish(gui_dynamic);
				selection_index_begin = invalid_selection_index;
				selection_index_end = invalid_selection_index;
			}
			else if (result.key == ::Window::Key::RIGHT) {

				if (selection_index_begin != invalid_selection_index &&
					selection_index_end != invalid_selection_index &&
					(((int32_t)result.mods & (int32_t)::Window::KeyMods::SHIFT) == 0)
					)
				{
					int32_t min_selection = std::max(std::min(selection_index_begin, selection_index_end), 0);
					int32_t max_selection = std::min(std::max(selection_index_begin, selection_index_end), (int32_t)text.size());

					text_cursor_position = max_selection - 1;

					selection_index_begin = invalid_selection_index;
					selection_index_end = invalid_selection_index;
				}

				int32_t initial_cursor_position = text_cursor_position;

				if (((int32_t)result.mods & (int32_t)::Window::KeyMods::CONTROL) != 0) {
					while (text_cursor_position < text.size() && text[text_cursor_position] != ' ')
						text_cursor_position++;
					while (text_cursor_position < text.size() && text[text_cursor_position] == ' ')
						text_cursor_position++;
				}
				else
					text_cursor_position++;

				text_cursor_position = std::clamp(text_cursor_position, 0, (int32_t)text.size());

				if (((int32_t)result.mods & (int32_t)::Window::KeyMods::SHIFT) != 0) {
					if (selection_index_begin == invalid_selection_index)
						selection_index_begin = initial_cursor_position;
					selection_index_end = text_cursor_position;
				}
			}
			else if (result.key == ::Window::Key::LEFT) {

				if (selection_index_begin != invalid_selection_index &&
					selection_index_end != invalid_selection_index &&
					(((int32_t)result.mods & (int32_t)::Window::KeyMods::SHIFT) == 0)
					)
				{
					int32_t min_selection = std::max(std::min(selection_index_begin, selection_index_end), 0);
					int32_t max_selection = std::min(std::max(selection_index_begin, selection_index_end), (int32_t)text.size());

					text_cursor_position = min_selection + 1;

					selection_index_begin = invalid_selection_index;
					selection_index_end = invalid_selection_index;
				}

				int32_t initial_cursor_position = text_cursor_position;

				if (((int32_t)result.mods & (int32_t)::Window::KeyMods::CONTROL) != 0) {
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

				if (((int32_t)result.mods & (int32_t)::Window::KeyMods::SHIFT) != 0) {
					if (selection_index_begin == invalid_selection_index)
						selection_index_begin = initial_cursor_position;
					selection_index_end = text_cursor_position;
				}
			}
			else if (result.key == ::Window::Key::V && ((int32_t)result.mods & (int32_t)::Window::KeyMods::CONTROL) != 0) {
				std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
				std::u32string s32 = conv.from_bytes(clipboard::get());

				text.insert(text_cursor_position, s32);
				text_cursor_position += s32.size();
			}
			else if (result.key == ::Window::Key::C && ((int32_t)result.mods & (int32_t)::Window::KeyMods::CONTROL) != 0) {

				if (selection_index_begin == invalid_selection_index ||
					selection_index_end == invalid_selection_index)
					return;

				int32_t min_selection = std::max(std::min(selection_index_begin, selection_index_end), 0);
				int32_t max_selection = std::min(std::max(selection_index_begin, selection_index_end), (int32_t)text.size());

				std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
				clipboard::set(conv.to_bytes(text.substr(min_selection, max_selection)));
			}
			else if (result.key == ::Window::Key::A && ((int32_t)result.mods & (int32_t)::Window::KeyMods::CONTROL) != 0) {

				selection_index_begin = 0;
				selection_index_end = text.size();
				text_cursor_position = text.size();
			}



		}
	}



}

void widget2::Slider::publish(GUI2Dynamic& gui_dynamic, float& value) {

	ignore_mouse_if_not_topmost_widget = false;
	
	Grid::publish(gui_dynamic);
	gui_dynamic.grid_add_column(GUI2Dynamic::avail);
	gui_dynamic.grid_add_row(GUI2Dynamic::avail);

	if (hold.is_activated_now(gui_dynamic))
		grab.start(gui_dynamic);
	if (gui_dynamic.get_io_state().mouse_state & GUI2::LeftRelease)
		grab.finish(gui_dynamic);

	if (grab.is_active()) {
		float position	= get_resolved_properties(gui_dynamic).position.x;
		float size		= get_resolved_properties(gui_dynamic).size.x;
		float cursor_position = gui_dynamic.get_io_state().mouse_position.x - gui_dynamic.window_prop().position.x;

		value = glm::clamp(glm::mix(min_value, max_value, (cursor_position - position) / size), min_value, max_value);
	}
		
	float filled_width = get_resolved_properties(gui_dynamic).size.x * (value - min_value) / (max_value - min_value);
	filled_bar.target_size.value.x	= filled_width;
	head.margin.value.x				= filled_width - head.get_resolved_properties(gui_dynamic).size.x / 2;

	background.publish(gui_dynamic);
	filled_bar.publish(gui_dynamic);
	head.publish(gui_dynamic);

	gui_dynamic.grid_end();
}

void widget2::DragFloat::publish(GUI2Dynamic& gui_dynamic, float& value) {
	
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;

	if (id == GUI2Dynamic::invalid_id) {
		string = convert.from_bytes(std::to_string(value));
		old_value = value;
	}

	if (old_value != value)
		string = convert.from_bytes(std::to_string(value));

	bool previous_focus = focus.is_active();

	TextInput::publish(gui_dynamic, string);

	if (!previous_focus && focus.is_active()) {
		selection_index_begin = 0;
		selection_index_end = string.size();
	}

	if (previous_focus && !focus.is_active()) {
		try {
			size_t idx;
			value = std::stof(convert.to_bytes(string), &idx);
			if (idx != string.size())
				throw std::exception();
		}
		catch (...) {
			if (string.size() == 0)
				value = 0;
			string = convert.from_bytes(std::to_string(value));
		}
	}

	if ((get_mouse_state() & IOWidget::Hold) && (gui_dynamic.get_io_state().mouse_state & GUI2::LeftHold))
	{
		grab.start(gui_dynamic);
	}

	if (gui_dynamic.get_io_state().mouse_state & GUI2::LeftRelease) {
		grab.finish(gui_dynamic);
		cursor_position_when_grabbed_publish = invalid_cursor_position;
	}

	if (grab.is_active()) {
		
		if (cursor_position_when_grabbed_publish == invalid_cursor_position)
			cursor_position_when_grabbed_publish = gui_dynamic.get_io_state().mouse_position;

		value += (gui_dynamic.get_io_state().mouse_position.x - cursor_position_when_grabbed_publish.x) * sensitivity;
		string = convert.from_bytes(std::to_string(value));
	
		selection_index_begin = 0;
		selection_index_end = string.size();

		cursor_position_when_grabbed_publish = gui_dynamic.get_io_state().mouse_position;
	}

	old_value = value;
}

void widget2::Button::publish(GUI2Dynamic& gui_dynamic) {

	ignore_mouse_if_not_topmost_widget = false;

	Grid::publish(gui_dynamic);
	gui_dynamic.grid_add_column(GUI2Dynamic::avail);
	gui_dynamic.grid_add_row(GUI2Dynamic::avail);

	background.publish(gui_dynamic);
	label.publish(gui_dynamic, text);

	gui_dynamic.grid_end();
}
