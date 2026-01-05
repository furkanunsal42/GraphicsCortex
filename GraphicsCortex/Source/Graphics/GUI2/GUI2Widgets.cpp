#include "GUI2Widgets.h"
#include "WindowBoundGlobalResources.h"
#include "Window.h"

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


void widget2::IOWidget::resolve_io(GUI2Dynamic& gui_dynamic)
{
	GUI2::MouseEvent event = gui_dynamic.get_resolved_properties(id).event;

	MouseState current_state = mouse_states[0];
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
			last_hold_end = std::chrono::system_clock::now();
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
		last_hover_begin = std::chrono::system_clock::now();
	
	if (current_state == Leave)
		last_hover_end = std::chrono::system_clock::now();

	if (current_state == HoldBegin) {
		widget_position_when_hold_begin = gui_dynamic.get_resolved_properties(id).position;
		cursor_position_when_hold_begin = gui_dynamic.get_io_state().mouse_position;
		last_hold_begin = std::chrono::system_clock::now();
	}
	
	if (current_state == HoldEnd)
		last_hold_end = std::chrono::system_clock::now();

	if (current_state == HoldBegin || current_state == Hold) {
		if (glm::dot(cursor_position_when_hold_begin - gui_dynamic.get_io_state().mouse_position, cursor_position_when_hold_begin - gui_dynamic.get_io_state().mouse_position) >= carry_begin_min_offset * carry_begin_min_offset) {
			current_state = CarryBegin;
		}
	}

	if (current_state == Click) {
		if (last_click != invalid_time && (std::chrono::system_clock::now() - last_click) <= double_click_max_delay && !was_last_click_double) {
			current_state = DoubleClick;
		}
		else {
			last_click = std::chrono::system_clock::now();
			was_last_click_double = false;
		}
	}

	if (current_state == DoubleClick) {
		last_click = std::chrono::system_clock::now();
		was_last_click_double = true;
	}

	mouse_states[0] = current_state;
}


float widget2::StyledWidget::get_t(
	std::chrono::system_clock::time_point last_begin,
	std::chrono::system_clock::time_point last_end,
	std::chrono::system_clock::duration transition_time)
{
	if (transition_time.count() == 0)
		return 1;

	auto now = std::chrono::system_clock::now();

	bool happening =
		(last_begin != IOWidget::invalid_time && last_end == IOWidget::invalid_time) ||
		(last_begin != IOWidget::invalid_time && last_end != IOWidget::invalid_time && last_begin > last_end);

	bool recovering =
		(last_begin != IOWidget::invalid_time && last_end != IOWidget::invalid_time && last_begin < last_end && now - last_end < std::min(transition_time, last_end - last_begin));

	float t = 0;
	if (happening) {
		std::chrono::system_clock::duration time_passed = std::min(std::chrono::system_clock::now() - last_begin, transition_time);
		t = 1 - (transition_time - time_passed).count() / (float)transition_time.count();
	}
	if (recovering) {
		std::chrono::system_clock::duration time_passed_hovering = std::min(last_end - last_begin, transition_time);
		std::chrono::system_clock::duration time_passed_recovering = std::min(std::chrono::system_clock::now() - last_end, transition_time);
		t = 1 - (transition_time - time_passed_hovering + time_passed_recovering).count() / (float)transition_time.count();
	}

	return t;
}

void widget2::Box::publish(GUI2Dynamic& gui_dynamic) {

	gui_dynamic.box_begin(id);

	gui_dynamic.box_prop().margin			=  widget2_get_property2(margin,			on_hover, on_hold);
	gui_dynamic.box_prop().target_size		=  widget2_get_property2(target_size,		on_hover, on_hold);
	gui_dynamic.box_prop().color			=  widget2_get_property2(color,				on_hover, on_hold);
	gui_dynamic.box_prop().border_thickness	=  widget2_get_property2(border_thickness,	on_hover, on_hold);
	gui_dynamic.box_prop().border_rounding	=  widget2_get_property2(border_rounding,	on_hover, on_hold);
	gui_dynamic.box_prop().border_color0	=  widget2_get_property2(border_color0,		on_hover, on_hold);
	gui_dynamic.box_prop().border_color1	=  widget2_get_property2(border_color1,		on_hover, on_hold);
	gui_dynamic.box_prop().border_color2	=  widget2_get_property2(border_color2,		on_hover, on_hold);
	gui_dynamic.box_prop().border_color3	=  widget2_get_property2(border_color3,		on_hover, on_hold);
	gui_dynamic.box_prop().shadow_thickness	=  widget2_get_property2(shadow_thickness,	on_hover, on_hold);
	gui_dynamic.box_prop().shadow_color		=  widget2_get_property2(shadow_color,		on_hover, on_hold);

	//gui_dynamic.box_end();

	resolve_io(gui_dynamic);

}

void widget2::Box::apply_properties_to(GUI2Dynamic::WindowDesc& desc)
{
	desc.padding			= glm::vec4(0);
	desc.target_size		=  widget2_get_property2(target_size,		on_hover, on_hold);
	desc.color				=  widget2_get_property2(color,				on_hover, on_hold);
	desc.border_thickness	=  widget2_get_property2(border_thickness,	on_hover, on_hold);
	desc.border_rounding	=  widget2_get_property2(border_rounding,	on_hover, on_hold);
	desc.border_color0		=  widget2_get_property2(border_color0,		on_hover, on_hold);
	desc.border_color1		=  widget2_get_property2(border_color1,		on_hover, on_hold);
	desc.border_color2		=  widget2_get_property2(border_color2,		on_hover, on_hold);
	desc.border_color3		=  widget2_get_property2(border_color3,		on_hover, on_hold);
	desc.shadow_thickness	=  widget2_get_property2(shadow_thickness,	on_hover, on_hold);
	desc.shadow_color		=  widget2_get_property2(shadow_color,		on_hover, on_hold);
}

void widget2::Box::apply_properties_to(GUI2Dynamic::BoxDesc& desc)
{
	desc.margin				=  widget2_get_property2(margin,			on_hover, on_hold);
	desc.target_size		=  widget2_get_property2(target_size,		on_hover, on_hold);
	desc.color				=  widget2_get_property2(color,				on_hover, on_hold);
	desc.border_thickness	=  widget2_get_property2(border_thickness,	on_hover, on_hold);
	desc.border_rounding	=  widget2_get_property2(border_rounding,	on_hover, on_hold);
	desc.border_color0		=  widget2_get_property2(border_color0,		on_hover, on_hold);
	desc.border_color1		=  widget2_get_property2(border_color1,		on_hover, on_hold);
	desc.border_color2		=  widget2_get_property2(border_color2,		on_hover, on_hold);
	desc.border_color3		=  widget2_get_property2(border_color3,		on_hover, on_hold);
	desc.shadow_thickness	=  widget2_get_property2(shadow_thickness,	on_hover, on_hold);
	desc.shadow_color		=  widget2_get_property2(shadow_color,		on_hover, on_hold);
}

void widget2::Box::apply_properties_to(GUI2Dynamic::GridDesc& desc)
{
	desc.margin				=  widget2_get_property2(margin,			on_hover, on_hold);
	desc.target_size		=  widget2_get_property2(target_size,		on_hover, on_hold);
	desc.padding			=  glm::vec4(0);
}

void widget2::Box::apply_properties_to(GUI2Dynamic::StackDesc& desc)
{
	desc.margin				=  widget2_get_property2(margin,			on_hover, on_hold);
	desc.target_size		=  widget2_get_property2(target_size,		on_hover, on_hold);
	desc.padding			=  glm::vec4(0);
	desc.spacing			=  0;
}

void widget2::Grid::publish(GUI2Dynamic& gui_dynamic) {

	gui_dynamic.grid_begin(id);

	gui_dynamic.grid_prop().margin		= widget2_get_property2(margin,			on_hover, on_hold);
	gui_dynamic.grid_prop().padding		= widget2_get_property2(padding,		on_hover, on_hold);
	gui_dynamic.grid_prop().target_size = widget2_get_property2(target_size,	on_hover, on_hold);
	
	resolve_io(gui_dynamic);

}

void widget2::Stack::publish(GUI2Dynamic& gui_dynamic) {

	gui_dynamic.stack_begin(id);

	gui_dynamic.stack_prop().margin			= widget2_get_property2(margin,			on_hover, on_hold);
	gui_dynamic.stack_prop().padding		= widget2_get_property2(padding,		on_hover, on_hold);
	gui_dynamic.stack_prop().target_size	= widget2_get_property2(target_size,	on_hover, on_hold);
	gui_dynamic.stack_prop().spacing		= widget2_get_property2(spacing,		on_hover, on_hold);
	
	resolve_io(gui_dynamic);

}

void widget2::Window::publish(GUI2Dynamic& gui_dynamic) {
	
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
		.set_color(glm::vec4(0.8, 0.8, 0.8, 1));

	resolve_io(gui_dynamic);
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

void widget2::Label::publish(GUI2Dynamic& gui_dynamic)
{
	if (active_global_resources == nullptr || !FontBank::get().does_font_exist(font))
		return;

	if (text.size() == 0)
		return;

	begin(gui_dynamic);
	end(gui_dynamic);
}

void widget2::Label::begin(GUI2Dynamic& gui_dynamic) {
	
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

void widget2::Label::publish_glyph(GUI2Dynamic& gui_dynamic, size_t end_index) {
	
	for (last_published_index; last_published_index < std::min(end_index, text.size()); last_published_index++) {

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
			.set_color(widget2_get_property2(text_color, on_hover, on_hold))
			.set_texture_handle(FontBank::get().get_font(font).atlas->texture_handle);

		text_size.y = glm::max(text_size.y, size.y);
		text_size.x = position.x + size.x;

		advance += table.advance * atlas_size.x / font_size * text_height;
	}
}

void widget2::Label::end(GUI2Dynamic& gui_dynamic) {

	publish_glyph(gui_dynamic, text.size());

	gui_dynamic.grid_add_column(text_size.x);
	gui_dynamic.grid_add_row(text_size.y);
	gui_dynamic.grid_prop().target_size = text_size;
	gui_dynamic.grid_end();

	resolve_io(gui_dynamic);
}

void widget2::TextArea::publish(GUI2Dynamic& gui_dynamic)
{
	if (can_aquire_keyboard_focus) {
		if ((get_mouse_state(0) & Click) == Click)
			keyboard_focus_begin = std::chrono::system_clock::now();

		if (get_mouse_state(0) == None && gui_dynamic.get_io_state().mouse_state & GUI2::MouseEvent::LeftRelease)
			keyboard_focus_begin = invalid_time;
	}
	
	//widget2_styled_property2(glm::vec4, placeholder_text_color, glm::vec4(0.4, 0.4, 0.4, 1),	on_hover, on_focus)
	//widget2_styled_property2(glm::vec4, selected_text_color,	glm::vec4(0.5, 0.5, 0.5, 1),	on_hover, on_focus)
	//widget2_styled_property2(glm::vec4,	text_color,				glm::vec4(0.2, 0.2, 0.2, 1),	on_hover, on_focus)
	//
	//widget2_styled_event(glm::vec4,		border_thickness,	on_focus)
	//widget2_styled_event(glm::vec4,		border_rounding,	on_focus)
	//widget2_styled_event(glm::vec4,		border_color0,		on_focus)
	//widget2_styled_event(glm::vec4,		border_color1,		on_focus)
	//widget2_styled_event(glm::vec4,		border_color2,		on_focus)
	//widget2_styled_event(glm::vec4,		border_color3,		on_focus)
	//widget2_styled_event(glm::vec4,		shadow_thickness,	on_focus)
	//widget2_styled_event(glm::vec4,		shadow_color,		on_focus)

	glm::vec4 prev_label_text_color = label.text_color;

	bool focused = keyboard_focus_begin != invalid_time;

	if (focused) {
		resolve_keyboard_io(gui_dynamic);
	}

	label.text			= text.size() == 0 && !focused ? placeholder_text : text;
	label.text_color	= label.text.size() == 0 ? placeholder_text_color :
							focused ? on_focus_text_color.value_or(label.text_color) : label.text_color;

	Grid::publish(gui_dynamic);
	gui_dynamic.grid_region(glm::ivec2(0));
	
	background.publish(gui_dynamic);
	label.publish(gui_dynamic);

	gui_dynamic.grid_add_column(GUI2Dynamic::avail);
	gui_dynamic.grid_add_row(GUI2Dynamic::avail);
	gui_dynamic.grid_end();

	resolve_io(gui_dynamic);

	label.text_color = prev_label_text_color;
}

void widget2::TextArea::resolve_keyboard_io(GUI2Dynamic& gui_dynamic) {
	
	for (GUI2::KeyboardEvent& e : gui_dynamic.get_io_state().keyboard_events) {
		if (e.data.index() == 0)
			text += std::get<uint32_t>(e.data);
		else {
			auto result = std::get<::Window::KeyPressResult>(e.data);
			if (result.key == ::Window::Key::BACKSPACE && result.action != ::Window::PressAction::RELEASE && text.size() != 0) {
				text.pop_back();
			}
		}
	}

}