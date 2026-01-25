#include "GUIWidgets.h"
#include "WindowBoundGlobalResources.h"
#include "Window.h"
#include "Clipboard.h"
#include "Math/GLMCout.h"

#include <codecvt>

GUIDynamic::ResolvedProperties widget::Widget::get_resolved_properties(GUIDynamic& gui_dynamic)
{
	return gui_dynamic.get_resolved_properties(id);
}

glm::vec2 widget::IOWidget::get_cursor_position_when_hold_begin(int32_t mouse_button) {
	return cursor_position_when_hold_begin;
}

glm::vec2 widget::IOWidget::get_widget_position_when_hold_begin(int32_t mouse_button) {
	return widget_position_when_hold_begin;
}

void widget::IOWidget::resolve_io(GUIDynamic& gui_dynamic)
{
	GUI::MouseEvent event = gui_dynamic.get_resolved_properties(id).event;

	bool click_happened = false;

	if (!hover.is_active() && (event & GUI::Hover))
		hover.start(gui_dynamic);

	if (hover.is_active() && (event & GUI::None))
		hover.finish(gui_dynamic);

	bool focused = false;
	if (gui_dynamic.window_prop_handle() != nullptr)
		focused = gui_dynamic.window_prop_handle()->is_window_focused();

	if (!hold.is_active() && (event & GUI::LeftPress) && focused) {
		hold.start(gui_dynamic);

		cursor_position_when_hold_begin = gui_dynamic.get_io_state().mouse_position;
		widget_position_when_hold_begin = get_resolved_properties(gui_dynamic).position;
	}

	if (hold.is_active() && (gui_dynamic.get_io_state().mouse_state & GUI::LeftRelease)) {
		hold.finish(gui_dynamic);
		click_happened = hover.is_active() && !carry.is_active();
	}

	if (click_happened) {

		if (click.end_time != GUIDynamic::invalid_time &&
			((gui_dynamic.get_current_frame_timepoint() - click.end_time) < double_click_max_delay) &&
			!was_last_click_double)
		{
			doubleclick.impulse(gui_dynamic);
			was_last_click_double = true;
		}
		else
			was_last_click_double = false;

		click.impulse(gui_dynamic);
	}

	if (!press.is_active() && event & GUI::LeftHold)
		press.start(gui_dynamic);

	if (press.is_active() && !(event & GUI::LeftHold))
		press.finish(gui_dynamic);

	if (hold.is_active() &&
		glm::distance(gui_dynamic.get_io_state().mouse_position, cursor_position_when_hold_begin) > carry_begin_min_offset)
	{
		carry.start(gui_dynamic);
	}

	if (carry.is_active() && !hold.is_active())
		carry.finish(gui_dynamic);
}

void widget::IOEvent::impulse(GUIDynamic& gui_dynamic) {
	start(gui_dynamic);
	finish(gui_dynamic);
}

void widget::IOEvent::start(GUIDynamic& gui_dynamic)
{
	begin_time = gui_dynamic.get_current_frame_timepoint();
}

void widget::IOEvent::finish(GUIDynamic& gui_dynamic)
{
	end_time = gui_dynamic.get_current_frame_timepoint();
}

bool widget::IOEvent::is_active()
{
	if (begin_time == GUIDynamic::invalid_time && end_time == GUIDynamic::invalid_time)
		return false;
	if (begin_time == GUIDynamic::invalid_time && end_time != GUIDynamic::invalid_time)
		return false;
	if (begin_time != GUIDynamic::invalid_time && end_time == GUIDynamic::invalid_time)
		return true;
	return (begin_time > end_time);
}

bool widget::IOEvent::is_activated_now(GUIDynamic& gui_dynamic)
{
	return begin_time == gui_dynamic.get_current_frame_timepoint();
}

bool widget::IOEvent::is_deactivated_now(GUIDynamic& gui_dynamic)
{
	return end_time == gui_dynamic.get_current_frame_timepoint();
}


void widget::Box::publish(GUIDynamic& gui_dynamic) {

	resolve_io(gui_dynamic);

	gui_dynamic.box_begin(id);

	gui_dynamic.box_prop().margin			=  margin;
	gui_dynamic.box_prop().target_size		=  target_size;
	gui_dynamic.box_prop().min_size			=  min_size;
	gui_dynamic.box_prop().max_size			=  max_size;
	gui_dynamic.box_prop().color			=  color;
	gui_dynamic.box_prop().border_thickness	=  border_thickness;
	gui_dynamic.box_prop().border_rounding	=  border_rounding;
	gui_dynamic.box_prop().border_color0	=  border_color0;
	gui_dynamic.box_prop().border_color1	=  border_color1;
	gui_dynamic.box_prop().border_color2	=  border_color2;
	gui_dynamic.box_prop().border_color3	=  border_color3;
	gui_dynamic.box_prop().shadow_thickness	=  shadow_thickness;
	gui_dynamic.box_prop().shadow_color		=  shadow_color;
	gui_dynamic.box_prop().pass_through_events = pass_through_events;

	//gui_dynamic.box_end();
}

void widget::Box::apply_properties_to(GUIDynamic::WindowDesc& desc)
{
	desc.padding			=  glm::vec4(0);
	desc.target_size		=  target_size;
	desc.min_size			=  min_size;
	desc.max_size			=  max_size;
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

void widget::Box::apply_properties_to(GUIDynamic::BoxDesc& desc)
{
	desc.margin				=  margin;
	desc.target_size		=  target_size;
	desc.min_size			=  min_size;
	desc.max_size			=  max_size;
	desc.color				=  color;
	desc.border_thickness	=  border_thickness;
	desc.border_rounding	=  border_rounding;
	desc.border_color0		=  border_color0;
	desc.border_color1		=  border_color1;
	desc.border_color2		=  border_color2;
	desc.border_color3		=  border_color3;
	desc.shadow_thickness	=  shadow_thickness;
	desc.shadow_color		=  shadow_color;
	desc.pass_through_events = pass_through_events;
}

void widget::Box::apply_properties_to(GUIDynamic::GridDesc& desc)
{
	desc.margin				=  margin;
	desc.target_size		=  target_size;
	desc.min_size			=  min_size;
	desc.max_size			=  max_size;
	desc.padding			=  glm::vec4(0);
	desc.pass_through_events = pass_through_events;
}

void widget::Box::apply_properties_to(GUIDynamic::StackDesc& desc)
{
	desc.margin				=  margin;
	desc.target_size		=  target_size;
	desc.min_size			=  min_size;
	desc.max_size			=  max_size;
	desc.padding			=  glm::vec4(0);
	desc.spacing			=  0;
	desc.pass_through_events = pass_through_events;
}

void widget::Grid::publish_begin(GUIDynamic& gui_dynamic) {

	resolve_io(gui_dynamic);

	gui_dynamic.grid_begin(id);

	gui_dynamic.grid_prop().margin		= margin;
	gui_dynamic.grid_prop().padding		= padding;
	gui_dynamic.grid_prop().target_size = target_size;
	gui_dynamic.grid_prop().min_size	= min_size;
	gui_dynamic.grid_prop().max_size	= max_size;
	gui_dynamic.grid_prop().pass_through_events = pass_through_events;

	gui_dynamic.grid_region(glm::ivec2(0));
}

void widget::Grid::publish_end(GUIDynamic& gui_dynamic) {
	
	gui_dynamic.grid_end();

}

void widget::Stack::publish_begin(GUIDynamic& gui_dynamic) {

	resolve_io(gui_dynamic);

	gui_dynamic.stack_begin(id);

	gui_dynamic.stack_prop().margin					= margin;
	gui_dynamic.stack_prop().padding				= padding;
	gui_dynamic.stack_prop().target_size			= target_size;
	gui_dynamic.stack_prop().min_size				= min_size;
	gui_dynamic.stack_prop().max_size				= max_size;
	gui_dynamic.stack_prop().spacing				= spacing;
	gui_dynamic.stack_prop().pass_through_events 	= pass_through_events;
	gui_dynamic.stack_prop().is_vertical			= is_vertical;
}

void widget::Stack::publish_end(GUIDynamic& gui_dynamic) {

	gui_dynamic.stack_end();

}

void widget::Window::publish_begin(GUIDynamic& gui_dynamic) {

	gui_dynamic.window_begin(id)
		.set_name(name)
		.set_is_resizable(is_resizable)
		.set_is_decorated(has_native_decoration)
		.set_position(position)
		.set_padding(padding)
		.set_target_size(target_size)
		.set_min_size(min_size)
		.set_max_size(max_size)
		.set_color(color)
		.set_border_thickness(border_thickness)
		.set_border_rounding(border_rounding)
		.set_border_color0(border_color0)
		.set_border_color1(border_color1)
		.set_border_color2(border_color2)
		.set_border_color3(border_color3)
		.set_shadow_thickness(shadow_thickness)
		.set_shadow_color(shadow_color);

	if (gui_dynamic.get_window_handle(id) != nullptr && resolution_change_newsletter == Newsletter<>::invalid_id) {

		resolution_change_newsletter = gui_dynamic.get_window_handle(id)->newsletters->on_window_resolution_events.subscribe([&](const glm::ivec2& resolution) {
			if (resolution == glm::ivec2(0))
				return;

			target_size.value = glm::vec2(resolution) / gui_dynamic.get_gui_scale();
			std::cout << resolution << std::endl;

			if (restore.is_activated_now(gui_dynamic)) {
				position	= restored_previous_position.value_or(position);
				target_size = restored_previous_size.value_or(target_size);

				if (restore_invalidation_counter == 1) {
					restored_previous_position	= std::nullopt;
					restored_previous_size		= std::nullopt;
				}
				else 
					restore_invalidation_counter++;
			}
				
			});

		position_change_newsletter = gui_dynamic.get_window_handle(id)->newsletters->on_window_position_events.subscribe([&](const glm::ivec2& position) {
			if (glm::any(glm::lessThanEqual(position, glm::ivec2(-32000))))
				return;
			this->position.value = position;

			if (restore.is_activated_now(gui_dynamic)) {
				this->position.value = restored_previous_position.value_or(this->position.value);
				target_size = restored_previous_size.value_or(target_size);

				if (restore_invalidation_counter == 1) {
					restored_previous_position	= std::nullopt;
					restored_previous_size		= std::nullopt;
					restore_invalidation_counter = 0;
				}
				else 
					restore_invalidation_counter++;
			}
			});
	}

	resolve_io(gui_dynamic);

	menubar_published = false;
}

void widget::Window::publish_end(GUIDynamic& gui_dynamic) {

	if (menubar_published)
		gui_dynamic.grid_end();

	gui_dynamic.window_prop()
		.set_name(name)
		.set_is_resizable(is_resizable)
		.set_is_decorated(has_native_decoration)
		.set_position(position)
		.set_padding(padding)
		.set_target_size(target_size)
		.set_min_size(min_size)
		.set_max_size(max_size)
		.set_color(color)
		.set_border_thickness(border_thickness)
		.set_border_rounding(border_rounding)
		.set_border_color0(border_color0)
		.set_border_color1(border_color1)
		.set_border_color2(border_color2)
		.set_border_color3(border_color3)
		.set_shadow_thickness(shadow_thickness)
		.set_shadow_color(shadow_color);

	if (gui_dynamic.get_window_handle(id) != nullptr && does_desire_maximal && !gui_dynamic.get_window_handle(id)->is_window_maximized()) {
		gui_dynamic.get_window_handle(id)->window_maximize();
		does_desire_maximal = false;
		restored_previous_position	= position;
		restored_previous_size		= target_size;
	}

	if (gui_dynamic.get_window_handle(id) != nullptr && does_desire_iconify && !gui_dynamic.get_window_handle(id)->is_window_minimized()) {
		gui_dynamic.get_window_handle(id)->window_minimize();
		does_desire_iconify = false;
	}

	if (gui_dynamic.get_window_handle(id) != nullptr && does_desire_restore && !gui_dynamic.get_window_handle(id)->is_window_restored()) {
		gui_dynamic.get_window_handle(id)->window_restore();
		does_desire_restore = false;
	}

	if (gui_dynamic.get_window_handle(id) != nullptr && does_desire_maximal_restore_swap) {

		if (gui_dynamic.get_window_handle(id)->is_window_maximized()) {
			gui_dynamic.get_window_handle(id)->window_restore();
		}
		else if (gui_dynamic.get_window_handle(id)->is_window_restored()) {
			restored_previous_position = position;
			restored_previous_size = target_size;
			gui_dynamic.get_window_handle(id)->window_maximize();
		}

		does_desire_maximal_restore_swap = false;
	}

	if (gui_dynamic.get_window_handle(id) != nullptr && !maximize.is_active() && gui_dynamic.get_window_handle(id)->is_window_maximized())
		maximize.start(gui_dynamic);

	if (gui_dynamic.get_window_handle(id) != nullptr && maximize.is_active() && !gui_dynamic.get_window_handle(id)->is_window_maximized()) {
		maximize.finish(gui_dynamic);
	}

	if (gui_dynamic.get_window_handle(id) != nullptr && !iconify.is_active() && gui_dynamic.get_window_handle(id)->is_window_minimized())
		iconify.start(gui_dynamic);

	if (gui_dynamic.get_window_handle(id) != nullptr && iconify.is_active() && !gui_dynamic.get_window_handle(id)->is_window_minimized()) {
		iconify.finish(gui_dynamic);
	}

	if (gui_dynamic.get_window_handle(id) != nullptr && !restore.is_active() && gui_dynamic.get_window_handle(id)->is_window_restored()) {
		restore.start(gui_dynamic);
	}

	if (gui_dynamic.get_window_handle(id) != nullptr && restore.is_active() && !gui_dynamic.get_window_handle(id)->is_window_restored()) {
		restore.finish(gui_dynamic);
	}

	gui_dynamic.window_end();
}

void widget::Window::publish_menubar_begin(GUIDynamic& gui_dynamic) {

	gui_dynamic.grid_begin()
		.set_target_size(glm::vec2(GUIDynamic::avail))
		.set_min_size(glm::vec2(GUIDynamic::fit))
		.set_padding(glm::vec4(0));

	gui_dynamic.grid_add_column(GUIDynamic::avail);
	gui_dynamic.grid_add_row(GUIDynamic::fit);
	gui_dynamic.grid_add_row(GUIDynamic::avail);

	gui_dynamic.grid_region(glm::ivec2(0, 0));

	menubar_published = true;
}

void widget::Window::publish_menubar_end(GUIDynamic& gui_dynamic) {

	gui_dynamic.grid_region(glm::ivec2(0, 1));

}

void widget::Window::drag(GUIDynamic& gui_dynamic, IOWidget& widget) {

	if (widget.doubleclick.is_activated_now(gui_dynamic)) {
		desire_maximal_restore_swap();
		return;
	}

	if (widget.carry.is_active()) {
		if (maximize.is_active() || iconify.is_active()) {
			restored_previous_position	= gui_dynamic.get_io_state().mouse_position - glm::vec2(restored_previous_size.value_or(glm::vec2(0)).x * gui_dynamic.get_gui_scale() / 2, widget.get_cursor_position_when_hold_begin().y + 10);
			//restored_previous_size		= target_size;
			desire_restore();
			return;
		}

		if (window_position_when_drag_begin == glm::vec2(-1000))
			window_position_when_drag_begin = position;

		position =
			window_position_when_drag_begin +
			gui_dynamic.get_io_state().mouse_position -
			widget.get_cursor_position_when_hold_begin();
	}
	else
		window_position_when_drag_begin = glm::vec2(-1000);

}

void widget::Window::desire_iconify()
{
	does_desire_maximal = false;
	does_desire_restore = false;
	does_desire_maximal_restore_swap = false;
	does_desire_iconify = true;
}

void widget::Window::desire_maximal()
{
	does_desire_iconify = false;
	does_desire_restore = false;
	does_desire_maximal_restore_swap = false;
	does_desire_maximal = true;
}

void widget::Window::desire_restore()
{
	does_desire_iconify = false;
	does_desire_maximal = false;
	does_desire_maximal_restore_swap = false;
	does_desire_restore = true;
}

void widget::Window::desire_maximal_restore_swap()
{
	does_desire_iconify = false;
	does_desire_maximal = false;
	does_desire_restore = false;
	does_desire_maximal_restore_swap = true;
}

void widget::Container::publish_begin(GUIDynamic& gui_dynamic) {

	Grid::publish_begin(gui_dynamic);
	gui_dynamic.grid_add_column(GUIDynamic::avail);
	gui_dynamic.grid_add_row(GUIDynamic::avail);
}

void widget::Container::publish_end(GUIDynamic& gui_dynamic) {

	Grid::publish_end(gui_dynamic);
}

void widget::Image::publish(GUIDynamic& gui_dynamic)
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
		
		GUIDynamic::ResolvedProperties properties = gui_dynamic.get_resolved_properties(grid_id);
		glm::vec2 size = glm::max(properties.size, get_resolved_properties(gui_dynamic).size);

		gui_dynamic.grid_add_column(GUIDynamic::avail);
		gui_dynamic.grid_add_row(GUIDynamic::avail);
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
		}
		else {
			clipped_size.x = self_size.x;
			clipped_size.y = clipped_size.x / texture_aspect_ratio;
		}

		gui_dynamic.box_prop().target_size		= clipped_size;
		gui_dynamic.box_prop().margin			= glm::vec4(GUIDynamic::avail);
		gui_dynamic.box_prop().texture_handle	= texture->texture_handle;
		gui_dynamic.box_prop().uv00				= uv00;
		gui_dynamic.box_prop().uv11				= uv11;
		
		gui_dynamic.grid_end();
	}
}

void widget::Label::publish(GUIDynamic& gui_dynamic, const std::u32string& text)
{
	if (active_global_resources == nullptr || !FontBank::get().does_font_exist(font))
		return;

	if (text.size() == 0)
		return;

	publish_begin(gui_dynamic, text);
	publish_end(gui_dynamic, text);
}

void widget::Label::publish_begin(GUIDynamic& gui_dynamic, const std::u32string& text) {
	
	if (active_global_resources == nullptr || !FontBank::get().does_font_exist(font))
		return;

	if (text.size() == 0)
		return;

	advance = 0;
	text_size = glm::vec2(0);
	last_published_index = 0;

	Container::publish_begin(gui_dynamic);

}

bool widget::Label::publish_glyph(GUIDynamic& gui_dynamic, size_t end_index, const std::u32string& text) {
	
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
			.set_uv00(glm::vec2(table.coords_low.x, 1 - table.coords_hi.y))
			.set_uv11(glm::vec2(table.coords_hi.x, 1 - table.coords_low.y))
			.set_color(text_color)
			.set_texture_handle(FontBank::get().get_font(font).atlas->texture_handle);

		text_size.y = glm::max(text_size.y, size.y);
		text_size.x = position.x + size.x;

		advance += table.advance * atlas_size.x / font_size * text_height;
	}

	return published_at_least_once;
}

float widget::Label::get_published_advance()
{
	return advance;
}

float widget::Label::compute_total_advance(size_t end_index, const std::u32string& text)
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

void widget::Label::publish_end(GUIDynamic& gui_dynamic, const std::u32string& text) {

	if (active_global_resources == nullptr || !FontBank::get().does_font_exist(font))
		return;

	if (text.size() == 0)
		return;

	publish_glyph(gui_dynamic, text.size(), text);

	//gui_dynamic.grid_add_column(text_size.x);
	//gui_dynamic.grid_add_row(text_height/*text_size.y*/);
	//gui_dynamic.grid_prop().target_size = text_size;
	
	gui_dynamic.grid_prop().target_size = glm::vec2(text_size.x, text_height);
	Container::publish_end(gui_dynamic);

}

void widget::TextInput::publish(GUIDynamic& gui_dynamic, std::u32string& text)
{
	Container::publish_begin(gui_dynamic);

	if (can_aquire_keyboard_focus) {
		if (click.is_activated_now(gui_dynamic))
			focus.start(gui_dynamic);

		if (!hover.is_active() && gui_dynamic.get_io_state().mouse_state & GUI::MouseEvent::LeftRelease) {
			focus.finish(gui_dynamic);
			selection_index_begin	= invalid_selection_index;
			selection_index_end		= invalid_selection_index;
		}
	}

	if (focus.is_active())
		resolve_keyboard_io(gui_dynamic, text);

	std::u32string& text_to_use	= text.size() == 0 && !focus.is_active() ? placeholder_text : text;
	label.text_color	= text.size() == 0 && !focus.is_active() ? placeholder_text_color : text_color;
	
	background.publish(gui_dynamic);
	
	if (selection_index_begin != invalid_selection_index && selection_index_end != invalid_selection_index) {
		int32_t min = std::min(selection_index_begin, selection_index_end);
		int32_t max = std::max(selection_index_begin, selection_index_end);
		float advance_begin = label.compute_total_advance(min, text_to_use);
		float advance_end = label.compute_total_advance(max, text_to_use);

		gui_dynamic.box_begin()
			.set_color(selected_background_color)
			.set_target_size(glm::vec2(advance_end - advance_begin, label.text_height + 8))
			.set_margin(glm::vec4(advance_begin + label.margin.value.x, GUIDynamic::avail, 0, GUIDynamic::avail));
	}

	//label.margin.value.y = (get_resolved_properties(gui_dynamic).size.y - label.text_height) / 2;
	
	label.publish_begin(gui_dynamic, text_to_use);
	if (selection_index_begin != invalid_selection_index && selection_index_end != invalid_selection_index) {
		int32_t min = std::min(selection_index_begin, selection_index_end);
		int32_t max = std::max(selection_index_begin, selection_index_end);

		label.publish_glyph(gui_dynamic, min, text_to_use);
		for (int32_t i = min + 1; i <= max; i++) {
			if (label.publish_glyph(gui_dynamic, i, text_to_use))
				gui_dynamic.box_prop().set_color(selected_text_color);
		}
	}
	label.publish_end(gui_dynamic, text_to_use);

	Grid::publish_end(gui_dynamic);
}

void widget::TextInput::resolve_keyboard_io(GUIDynamic& gui_dynamic, std::u32string& text) {
	
	for (GUI::KeyboardEvent& e : gui_dynamic.get_io_state().keyboard_events) {
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

void widget::Slider::publish(GUIDynamic& gui_dynamic, float& value) {

	Container::publish_begin(gui_dynamic);
	
	if (hold.is_active()) {
		float position	= get_resolved_properties(gui_dynamic).position.x;
		float size		= get_resolved_properties(gui_dynamic).size.x;
		float cursor_position = gui_dynamic.get_mouse_position_scale_independent().x;

		value = glm::clamp(glm::mix(min_value, max_value, (cursor_position - position) / size), min_value, max_value);
	}
		
	float filled_width = get_resolved_properties(gui_dynamic).size.x * glm::clamp((value - min_value) / (max_value - min_value), 0.0f, 1.0f);
	filled_bar.target_size.value.x	= filled_width;
	head.margin.value.x				= filled_width - head.get_resolved_properties(gui_dynamic).size.x / 2;

	background.publish(gui_dynamic);
	filled_bar.publish(gui_dynamic);
	head.publish(gui_dynamic);

	Container::publish_end(gui_dynamic);

}

namespace {

	std::string std_to_string_with_precision(float value, int32_t precision) {
		std::ostringstream ss;
		ss.precision(precision);
		ss << std::fixed << value;
		return std::move(ss).str();
	}
}

void widget::DragFloat::publish(GUIDynamic& gui_dynamic, float& value) {
	
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;

	if (id == GUIDynamic::invalid_id) {
		string = convert.from_bytes(std_to_string_with_precision(value, precision));
		old_value = value;
	}

	if (old_value != value)
		string = convert.from_bytes(std_to_string_with_precision(value, precision));

	bool previous_focus = focus.is_active();

	bool prev_can_aquire_keyboard_focus = can_aquire_keyboard_focus;
	can_aquire_keyboard_focus = false;

	TextInput::publish(gui_dynamic, string);

	can_aquire_keyboard_focus = prev_can_aquire_keyboard_focus;

	if (doubleclick.is_activated_now(gui_dynamic))
		focus.start(gui_dynamic);

	if (!hover.is_active() && gui_dynamic.get_io_state().mouse_state & GUI::MouseEvent::LeftRelease) {
		focus.finish(gui_dynamic);
		selection_index_begin = invalid_selection_index;
		selection_index_end = invalid_selection_index;
	}

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
			string = convert.from_bytes(std_to_string_with_precision(value, precision));
		}
	}

	if (hold.is_active()) {

		if (cursor_position_when_grabbed_publish == invalid_cursor_position)
			cursor_position_when_grabbed_publish = gui_dynamic.get_mouse_position_scale_independent();

		value += (gui_dynamic.get_mouse_position_scale_independent().x - cursor_position_when_grabbed_publish.x) * sensitivity;
		string = convert.from_bytes(std_to_string_with_precision(value, precision));
	
		if (selection_index_begin != invalid_selection_index && selection_index_end != invalid_selection_index) {
			selection_index_begin = 0;
			selection_index_end = string.size();
		}
	
		cursor_position_when_grabbed_publish = gui_dynamic.get_mouse_position_scale_independent();
	}
	
	if (hold.is_deactivated_now(gui_dynamic))
		cursor_position_when_grabbed_publish = invalid_cursor_position;

	old_value = value;
}

void widget::Button::publish(GUIDynamic& gui_dynamic) {

	Container::publish_begin(gui_dynamic);

	background.publish(gui_dynamic);
	label.publish(gui_dynamic, text);

	Container::publish_end(gui_dynamic);
}

void widget::ImageButton::publish(GUIDynamic& gui_dynamic) {

	Container::publish_begin(gui_dynamic);
	
	background.publish(gui_dynamic);
	image.publish(gui_dynamic);

	Container::publish_end(gui_dynamic);
}

void widget::CheckBox::publish(GUIDynamic& gui_dynamic, bool& checked) {
	
	Container::publish_begin(gui_dynamic);
	
	background.publish(gui_dynamic);
	
	if (checked)
		image.publish(gui_dynamic);

	Container::publish_end(gui_dynamic);

	if (click.is_activated_now(gui_dynamic)) {
		checked = !checked;
	}

	if (!checked && check.is_active()) check.finish(gui_dynamic);
	if (checked && !check.is_active()) check.start(gui_dynamic);

	image.texture = gui_dynamic.gui_texture_bank.get_texture("check.svg");

}


void widget::ComboBox::publish_begin(GUIDynamic& gui_dynamic){
		
	Container::publish_begin(gui_dynamic);

	background.publish(gui_dynamic);
	label.publish(gui_dynamic,  text);

	Container::publish_end(gui_dynamic);

	if (click.is_activated_now(gui_dynamic)) {
		if (!drop.is_active())
			drop.start(gui_dynamic);
		else
			drop.finish(gui_dynamic);
	}

	if (drop.is_active() && !(hover.is_active() || dropdown.hover.is_active()) && gui_dynamic.get_io_state().mouse_state & GUI::MouseEvent::LeftPress)
		drop.finish(gui_dynamic);


	if (drop.is_active()) {

		dropdown.position = 
			gui_dynamic.window_prop().position + 
			(get_resolved_properties(gui_dynamic).position + glm::vec2(0, get_resolved_properties(gui_dynamic).size.y)) * gui_dynamic.get_gui_scale();
		
		dropdown.publish_begin(gui_dynamic);
		dropdown_stack.publish_begin(gui_dynamic);
	}
}

widget::ComboBox::ComboBox(std::u32string text)
{
	this->text = text;
}

void widget::ComboBox::publish_end(GUIDynamic& gui_dynamic){
	
	if (drop.is_active()) {
		dropdown_stack.publish_end(gui_dynamic);
		dropdown.publish_end(gui_dynamic);
	}

	if (item_selected.is_activated_now(gui_dynamic))
		drop.finish(gui_dynamic);
}

void widget::ComboBoxItem::publish(GUIDynamic& gui_dynamic, ComboBox& owner_combobox) {

	Container::publish_begin(gui_dynamic);

	background.publish(gui_dynamic);
	label.publish(gui_dynamic, text);

	Container::publish_end(gui_dynamic);

	if (click.is_activated_now(gui_dynamic))
		select(gui_dynamic, owner_combobox);
}

void widget::ComboBoxItem::select(GUIDynamic& gui_dynamic, ComboBox& owner_combobox)
{
	owner_combobox.text = text;
	owner_combobox.item_selected.impulse(gui_dynamic);
}

widget::ComboBoxItem::ComboBoxItem(std::u32string text)
{
	this->text = text;
}

widget::Menu::Menu(std::u32string text) {
	this->text = text;
}

void widget::Menu::publish_begin(GUIDynamic& gui_dynamic) {

	Container::publish_begin(gui_dynamic);

	background.publish(gui_dynamic);
	label.publish(gui_dynamic, text);

	Container::publish_end(gui_dynamic);

	if (click.is_activated_now(gui_dynamic)) {
		if (!drop.is_active())
			drop.start(gui_dynamic);
		else
			drop.finish(gui_dynamic);
	}

	if (drop.is_active() && !(hover.is_active() || dropdown.hover.is_active()) && gui_dynamic.get_io_state().mouse_state & GUI::MouseEvent::LeftPress)
		drop.finish(gui_dynamic);


	if (drop.is_active()) {

		dropdown.position =
			gui_dynamic.window_prop().position +
			(get_resolved_properties(gui_dynamic).position + glm::vec2(0, get_resolved_properties(gui_dynamic).size.y)) * gui_dynamic.get_gui_scale();

		dropdown.publish_begin(gui_dynamic);
		dropdown_stack.publish_begin(gui_dynamic);
	}
}

void widget::Menu::publish_end(GUIDynamic& gui_dynamic) {
	
	if (drop.is_active()) {
		dropdown_stack.publish_end(gui_dynamic);
		dropdown.publish_end(gui_dynamic);
	}

	if (item_selected.is_activated_now(gui_dynamic))
		drop.finish(gui_dynamic);
}

widget::MenuItem::MenuItem(std::u32string text) {
	this->text = text;
}

void widget::MenuItem::publish(GUIDynamic& gui_dynamic, Menu& owner_menu){

	Container::publish_begin(gui_dynamic);

	background.publish(gui_dynamic);
	label.publish(gui_dynamic, text);

	Container::publish_end(gui_dynamic);

	if (click.is_activated_now(gui_dynamic))
		select(gui_dynamic, owner_menu);

};

void widget::MenuItem::select(GUIDynamic& gui_dynamic, Menu& owner_menu) {

	owner_menu.item_selected.impulse(gui_dynamic);

};

void widget::WindowControls::publish(GUIDynamic& gui_dynamic) {
	
	minimize_button.image.texture	= gui_dynamic.gui_texture_bank.get_texture("window-minimize-symbolic-svgrepo-com.svg");
	close_button.image.texture		= gui_dynamic.gui_texture_bank.get_texture("window-close-symbolic-svgrepo-com.svg");

	if (gui_dynamic.window_prop_handle() != nullptr && gui_dynamic.window_prop_handle()->is_window_maximized())
		restore_button.image.texture	= gui_dynamic.gui_texture_bank.get_texture("window-restore-symbolic-svgrepo-com.svg");
	else
		restore_button.image.texture	= gui_dynamic.gui_texture_bank.get_texture("window-maximize-symbolic-svgrepo-com.svg");


	Stack::publish_begin(gui_dynamic);
	
	minimize_button.publish(gui_dynamic);
	restore_button.publish(gui_dynamic);
	close_button.publish(gui_dynamic);

	Stack::publish_end(gui_dynamic);
}

void widget::WindowControls::manage(GUIDynamic& gui_dynamic, widget::Window& window) {
	
	if (minimize_button.click.is_activated_now(gui_dynamic))
		window.desire_iconify();

	if (restore_button.click.is_activated_now(gui_dynamic))
		window.desire_maximal_restore_swap();

	if (close_button.click.is_activated_now(gui_dynamic) && gui_dynamic.get_window_handle(window.id) != nullptr)
		gui_dynamic.get_window_handle(window.id)->set_should_close(true);

}

void widget::MenuBar::publish_begin(GUIDynamic& gui_dynamic) {

	Grid::publish_begin(gui_dynamic);

	gui_dynamic.grid_add_column(GUIDynamic::avail);
	gui_dynamic.grid_add_column(GUIDynamic::fit);
	gui_dynamic.grid_add_row(GUIDynamic::avail);

	gui_dynamic.grid_region(glm::ivec2(0, 0), glm::ivec2(2, 1));
	background.publish(gui_dynamic);

	gui_dynamic.grid_region(glm::ivec2(1, 0));
	window_controls.publish(gui_dynamic);

	gui_dynamic.grid_region(glm::ivec2(0, 0));
	manu_stack.publish_begin(gui_dynamic);
	
}

void widget::MenuBar::publish_end(GUIDynamic& gui_dynamic) {

	manu_stack.publish_end(gui_dynamic);
	Grid::publish_end(gui_dynamic);

}