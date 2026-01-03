#include "GUI2Widgets.h"
#include "WindowBoundGlobalResources.h"

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

	gui_dynamic.box_prop().margin			=  get_property(margin,				on_hover_margin,			on_hover_margin_transition,				on_hold_margin,				on_hold_margin_transition);
	gui_dynamic.box_prop().target_size		=  get_property(target_size,		on_hover_target_size,		on_hover_target_size_transition,		on_hold_target_size,		on_hold_target_size_transition);
	gui_dynamic.box_prop().color			=  get_property(color,				on_hover_color,				on_hover_color_transition,				on_hold_color,				on_hold_color_transition);
	gui_dynamic.box_prop().border_thickness	=  get_property(border_thickness,	on_hover_border_thickness,	on_hover_border_thickness_transition,	on_hold_border_thickness,	on_hold_border_thickness_transition);
	gui_dynamic.box_prop().border_rounding	=  get_property(border_rounding,	on_hover_border_rounding,	on_hover_border_rounding_transition,	on_hold_border_rounding,	on_hold_border_rounding_transition);
	gui_dynamic.box_prop().border_color0	=  get_property(border_color0,		on_hover_border_color0,		on_hover_border_color0_transition,		on_hold_border_color0,		on_hold_border_color0_transition);
	gui_dynamic.box_prop().border_color1	=  get_property(border_color1,		on_hover_border_color1,		on_hover_border_color1_transition,		on_hold_border_color1,		on_hold_border_color1_transition);
	gui_dynamic.box_prop().border_color2	=  get_property(border_color2,		on_hover_border_color2,		on_hover_border_color2_transition,		on_hold_border_color2,		on_hold_border_color2_transition);
	gui_dynamic.box_prop().border_color3	=  get_property(border_color3,		on_hover_border_color3,		on_hover_border_color3_transition,		on_hold_border_color3,		on_hold_border_color3_transition);
	gui_dynamic.box_prop().shadow_thickness	=  get_property(shadow_thickness,	on_hover_shadow_thickness,	on_hover_shadow_thickness_transition,	on_hold_shadow_thickness,	on_hold_shadow_thickness_transition);
	gui_dynamic.box_prop().shadow_color		=  get_property(shadow_color,		on_hover_shadow_color,		on_hover_shadow_color_transition,		on_hold_shadow_color,		on_hold_shadow_color_transition);

	//gui_dynamic.box_end();

	resolve_io(gui_dynamic);

}

void widget2::Box::apply_properties_to(GUI2Dynamic::WindowDesc& desc)
{
	desc.padding			= glm::vec4(0);
	desc.target_size		=  get_property(target_size,		on_hover_target_size,		on_hover_target_size_transition,		on_hold_target_size,		on_hold_target_size_transition);
	desc.color				=  get_property(color,				on_hover_color,				on_hover_color_transition,				on_hold_color,				on_hold_color_transition);
	desc.border_thickness	=  get_property(border_thickness,	on_hover_border_thickness,	on_hover_border_thickness_transition,	on_hold_border_thickness,	on_hold_border_thickness_transition);
	desc.border_rounding	=  get_property(border_rounding,	on_hover_border_rounding,	on_hover_border_rounding_transition,	on_hold_border_rounding,	on_hold_border_rounding_transition);
	desc.border_color0		=  get_property(border_color0,		on_hover_border_color0,		on_hover_border_color0_transition,		on_hold_border_color0,		on_hold_border_color0_transition);
	desc.border_color1		=  get_property(border_color1,		on_hover_border_color1,		on_hover_border_color1_transition,		on_hold_border_color1,		on_hold_border_color1_transition);
	desc.border_color2		=  get_property(border_color2,		on_hover_border_color2,		on_hover_border_color2_transition,		on_hold_border_color2,		on_hold_border_color2_transition);
	desc.border_color3		=  get_property(border_color3,		on_hover_border_color3,		on_hover_border_color3_transition,		on_hold_border_color3,		on_hold_border_color3_transition);
	desc.shadow_thickness	=  get_property(shadow_thickness,	on_hover_shadow_thickness,	on_hover_shadow_thickness_transition,	on_hold_shadow_thickness,	on_hold_shadow_thickness_transition);
	desc.shadow_color		=  get_property(shadow_color,		on_hover_shadow_color,		on_hover_shadow_color_transition,		on_hold_shadow_color,		on_hold_shadow_color_transition);
}

void widget2::Box::apply_properties_to(GUI2Dynamic::BoxDesc& desc)
{
	desc.margin				=  get_property(margin,				on_hover_margin,			on_hover_margin_transition,				on_hold_margin,				on_hold_margin_transition);
	desc.target_size		=  get_property(target_size,		on_hover_target_size,		on_hover_target_size_transition,		on_hold_target_size,		on_hold_target_size_transition);
	desc.color				=  get_property(color,				on_hover_color,				on_hover_color_transition,				on_hold_color,				on_hold_color_transition);
	desc.border_thickness	=  get_property(border_thickness,	on_hover_border_thickness,	on_hover_border_thickness_transition,	on_hold_border_thickness,	on_hold_border_thickness_transition);
	desc.border_rounding	=  get_property(border_rounding,	on_hover_border_rounding,	on_hover_border_rounding_transition,	on_hold_border_rounding,	on_hold_border_rounding_transition);
	desc.border_color0		=  get_property(border_color0,		on_hover_border_color0,		on_hover_border_color0_transition,		on_hold_border_color0,		on_hold_border_color0_transition);
	desc.border_color1		=  get_property(border_color1,		on_hover_border_color1,		on_hover_border_color1_transition,		on_hold_border_color1,		on_hold_border_color1_transition);
	desc.border_color2		=  get_property(border_color2,		on_hover_border_color2,		on_hover_border_color2_transition,		on_hold_border_color2,		on_hold_border_color2_transition);
	desc.border_color3		=  get_property(border_color3,		on_hover_border_color3,		on_hover_border_color3_transition,		on_hold_border_color3,		on_hold_border_color3_transition);
	desc.shadow_thickness	=  get_property(shadow_thickness,	on_hover_shadow_thickness,	on_hover_shadow_thickness_transition,	on_hold_shadow_thickness,	on_hold_shadow_thickness_transition);
	desc.shadow_color		=  get_property(shadow_color,		on_hover_shadow_color,		on_hover_shadow_color_transition,		on_hold_shadow_color,		on_hold_shadow_color_transition);
}

void widget2::Box::apply_properties_to(GUI2Dynamic::GridDesc& desc)
{
	desc.margin				=  get_property(margin,				on_hover_margin,			on_hover_margin_transition,				on_hold_margin,				on_hold_margin_transition);
	desc.target_size		=  get_property(target_size,		on_hover_target_size,		on_hover_target_size_transition,		on_hold_target_size,		on_hold_target_size_transition);
	desc.padding			=  glm::vec4(0);
}

void widget2::Box::apply_properties_to(GUI2Dynamic::StackDesc& desc)
{
	desc.margin				=  get_property(margin,				on_hover_margin,			on_hover_margin_transition,				on_hold_margin,				on_hold_margin_transition);
	desc.target_size		=  get_property(target_size,		on_hover_target_size,		on_hover_target_size_transition,		on_hold_target_size,		on_hold_target_size_transition);
	desc.padding			=  glm::vec4(0);
	desc.spacing			=  0;
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
		gui_dynamic.box_begin().uv00 = uv00;
		gui_dynamic.box_begin().uv11 = uv11;
		return;
	}
	

	if (type == Stretch) {
		Box::publish(gui_dynamic);
		gui_dynamic.box_prop().texture_handle = texture->texture_handle;
		gui_dynamic.box_begin().uv00 = uv00;
		gui_dynamic.box_begin().uv11 = uv11;
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

	float advance = 0;
	glm::vec2 text_size(0);

	auto& desc = gui_dynamic.grid_begin(id);
	apply_properties_to(desc);
	desc.target_size = glm::vec2(GUI2Dynamic::fit);

	gui_dynamic.grid_region(glm::ivec2(0));

	for (int32_t i = 0; i < text.size(); i++) {
	
		FontBank::get().get_font(font);

		float font_size			= FontBank::get().get_font(font).font_size;
		glm::vec2 atlas_size	= FontBank::get().get_font(font).atlas->get_size();
		auto& table				= FontBank::get().get_font(font).glyph_table.at(text[i]);

		glm::vec2 inverted_offset = glm::vec2(table.offset.x, -table.offset.y);

		glm::vec2 position	= glm::vec2(advance, 0) + inverted_offset * atlas_size / font_size * text_height + glm::vec2(0, text_height);
		glm::vec2 size		= glm::max(glm::vec2(0), (table.coords_hi - table.coords_low) * atlas_size / font_size * text_height);
		
		gui_dynamic.box_begin()
			.set_target_size(size)
			.set_margin(glm::vec4(position.x, position.y, 0, 0))
			.set_uv00(glm::vec2(table.coords_hi.x,	1-table.coords_hi.y))
			.set_uv11(glm::vec2(table.coords_low.x, 1 - table.coords_low.y))
			.set_color(get_property(color, on_hover_color, on_hover_color_transition, on_hold_color, on_hold_color_transition))
			.set_texture_handle(FontBank::get().get_font(font).atlas->texture_handle);

		text_size.y = glm::max(text_size.y, size.y);
		text_size.x = position.x + size.x;

		advance += table.advance * atlas_size.x / font_size * text_height;
	}

	gui_dynamic.grid_add_column(text_size.x);
	gui_dynamic.grid_add_row(text_size.y);
	gui_dynamic.grid_end();

	resolve_io(gui_dynamic);

	//if (element.size().x == 0)	element.size().x = advance;
	//if (element.size().y == 0)	element.size().y = text_size.y;
}

void widget2::TextArea::publish(GUI2Dynamic& gui_dynamic)
{
	label.text = text.size() == 0 ? placeholder_text : text;
	
	gui_dynamic.grid_begin();
	gui_dynamic.grid_region(glm::ivec2(0));
	
	label.publish(gui_dynamic);
	Box::publish(gui_dynamic);

	glm::vec2 size = get_resolved_properties(gui_dynamic).size;
	size = glm::max(size, label.get_resolved_properties(gui_dynamic).size);
	size = glm::max(size, glm::vec2(1));

	gui_dynamic.grid_add_column(size.x);
	gui_dynamic.grid_add_row(size.y);
	gui_dynamic.grid_end();

	resolve_io(gui_dynamic);
}
