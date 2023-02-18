#include "Gui.h"
#include "Graphic.h"
#include "Default_Programs.h"

Layout::Layout(uint32_t type, const AABB2& aabb) :
	type((Layout::LayoutType)type), window_size(aabb.size), position(aabb.position) {}

Layout::Layout(uint32_t type, const Vec2<float>& position, const Vec2<float>& window_size) :
	type((Layout::LayoutType)type), window_size(window_size), position(position) {}

void Layout::add_widget(const Vec2<float>& size){
	if (type == Layout::Horizional) {
		window_size.x += size.x;
		window_size.y = std::max(window_size.y, size.y);
	}
	else if (type == Layout::Vertical) {
		window_size.x = std::max(window_size.x, size.x);
		window_size.y += size.y;
	}
}

Vec2<float> Layout::get_widget_position() {
	if (type == Layout::Horizional) {
		return Vec2<float>(position.x + window_size.x, position.y);
	}
	else if (type == Layout::Vertical) {
		return Vec2<float>(position.x, position.y + window_size.y);
	}
}

Vec2<float> Layout::get_position() {
	return position;
}

Vec2<float> Layout::get_raw_position() {
	return _raw_position;
}

void Layout::clear() {
	position = _raw_position;
	window_size = Vec2<float>(0, 0);
}

void Layout::update_position_by_style(const Vec4<float>& margin, const Vec4<float>& padding) {
	position = _raw_position + Vec2<float>(margin.y, margin.x);
	window_size = Vec2<float>(window_size.x + padding.y + padding.w, window_size.y + padding.x + padding.z);
}

//StaticStyle& Style::get_hover(){
//	return _on_hover;
//}
//
//StaticStyle& Style::get_active() {
//	return _on_active;
//}

namespace {
	template<typename T>
	T optional_get(std::vector<std::optional<T>> fallback_list, T null_value = T()) {
		for (std::optional<T>& _property: fallback_list) {
			if (_property)
				return _property.value();
		}
		return null_value;
	}

	template<typename T>
	T optional_get(std::optional<T> optional, T null_value = T()) {
		if (optional)
			return optional.value();
		return null_value;
	}

	template<typename T>
	T linear_interpolation(T default_value, T target_value, Time max_time, Time current_time) {
		return default_value + (target_value - default_value) / (max_time) * (std::min(current_time, max_time));
	}

	StaticStyle interpolate_styles(Style style, WidgetInfo& widget_info) {
		StaticStyle style_to_use;
		Time animation_time;
		if (widget_info.was_active){
			style_to_use = style.on_active;
			animation_time = widget_info.active_duration;
		}
		else if (widget_info.was_hovered){
			style_to_use = style.on_hover;
			animation_time = widget_info.hover_duration;
		}
		else if (widget_info.active_duration > 0) {
			style_to_use = style.on_active;
			animation_time = widget_info.active_duration;
		}
		else if (widget_info.hover_duration > 0) {
			style_to_use = style.on_hover;
			animation_time = widget_info.hover_duration;
		}

		// insert object to layout
		Vec3<float> color =				optional_get<Vec3<float>>({			style_to_use.color,				style.color });
		Vec2<float> displacement =		optional_get<Vec2<float>>({			style_to_use.displacement,		style.displacement });
		Vec2<float> rotation_euler =	optional_get<Vec2<float>>({			style_to_use.rotation_euler,	style.rotation_euler });
		Vec4<float> corner_rounding =	optional_get<Vec4<float>>({			style_to_use.corner_rounding,	style.corner_rounding });
		Vec4<float> padding =			optional_get<Vec4<float>>({			style_to_use.padding,			style.padding });
		Vec4<float> margin =			optional_get<Vec4<float>>({			style_to_use.margin,			style.margin });
		Vec4<float> border_thickness =	optional_get<Vec4<float>>({			style_to_use.border_thickness,	style.border_thickness });
		Vec3<float> border_color =		optional_get<Vec3<float>>({			style_to_use.border_color,		style.border_color });
		Frame::CursorType cursor_type =	optional_get<Frame::CursorType>({	style_to_use.cursor_type,		style.cursor_type });
		
		Vec3<float> color_default =				optional_get<Vec3<float>>({			style.color });
		Vec2<float> displacement_default =		optional_get<Vec2<float>>({			style.displacement });
		Vec2<float> rotation_euler_default =	optional_get<Vec2<float>>({			style.rotation_euler });
		Vec4<float> corner_rounding_default =	optional_get<Vec4<float>>({			style.corner_rounding });
		Vec4<float> padding_default =			optional_get<Vec4<float>>({			style.padding });
		Vec4<float> margin_default =			optional_get<Vec4<float>>({			style.margin });
		Vec4<float> border_thickness_default =	optional_get<Vec4<float>>({			style.border_thickness });
		Vec3<float> border_color_default =		optional_get<Vec3<float>>({			style.border_color });
		Frame::CursorType cursor_type_default = optional_get<Frame::CursorType>({	style.cursor_type });

		Time color_change =				style.color_change.value_or(0.0);
		Time displacement_change =		style.displacement_change.value_or(0.0);
		Time rotation_change =			style.rotation_change.value_or(0.0);
		Time corner_rounding_change =	style.corner_rounding_change.value_or(0.0);
		Time padding_change =			style.padding_change.value_or(0.0);
		Time margin_change =			style.margin_change.value_or(0.0);
		Time border_thickness_change =	style.border_thickness_change.value_or(0.0);
		Time border_color_change =		style.border_color_change.value_or(0.0);

		Time max_animation_duration = std::max({ color_change, displacement_change, rotation_change, corner_rounding_change, padding_change, margin_change, border_thickness_change, border_color_change });
		widget_info.hover_duration = std::min(widget_info.hover_duration, max_animation_duration);
		widget_info.active_duration = std::min(widget_info.active_duration, max_animation_duration);

		StaticStyle result;
		
		if (color_change > 0)				result.color =				linear_interpolation(color_default,				color,				color_change,				animation_time);
		else								result.color =				color; 
		if (displacement_change > 0)		result.displacement =		linear_interpolation(displacement_default,		displacement,		displacement_change,		animation_time);
		else								result.displacement =		displacement; 
		if (rotation_change > 0)			result.rotation_euler =		linear_interpolation(rotation_euler_default,	rotation_euler,		rotation_change,			animation_time);
		else								result.rotation_euler =		rotation_euler; 
		if (corner_rounding_change > 0)		result.corner_rounding =	linear_interpolation(corner_rounding_default,	corner_rounding,	corner_rounding_change,		animation_time);
		else								result.corner_rounding =	corner_rounding; 
		if (padding_change > 0)				result.padding =			linear_interpolation(padding_default,			padding,			padding_change,				animation_time);
		else								result.padding =			padding; 
		if (margin_change > 0)				result.margin =				linear_interpolation(margin_default,			margin,				margin_change,				animation_time);
		else								result.margin =				margin; 	
		if (border_thickness_change > 0)	result.border_thickness =	linear_interpolation(border_thickness_default,	border_thickness,	border_thickness_change,	animation_time);
		else								result.border_thickness =	border_thickness; 
		if (border_color_change > 0)		result.border_color =		linear_interpolation(border_color_default,		border_color,		border_color_change,		animation_time);
		else								result.border_color =		border_color;
		result.cursor_type = cursor_type;

		return result;
	}

}

Ui::Ui(Frame& frame) :
	frame(frame)
{
	window_size = Vec2<int>(frame.window_width, frame.window_height);
	_update_matrix(frame.window_width, frame.window_height);
	program = default_program::gui_program();
}

void Ui::new_frame(){
	
	// update time
	Time now = frame.get_time_sec() * 1000;
	frame_time = now - last_update_ms;
	last_update_ms = now;
	
	// update projection matrix on window resize
	if (window_size.x != frame.window_width || window_size.y != frame.window_height) {
		window_size = Vec2<int>(frame.window_width, frame.window_height);
		_update_matrix(frame.window_width, frame.window_height);
	}
	
	layouts.clear();
	_position_calculation_calls.clear();
	_render_calls.clear();

	Frame::CursorState cursor_state = frame.get_mouse_state();
	if (!_hovered)
		frame.set_cursor_type(Frame::Arrow);
	_hovered = false;

	if (_cursor_state == cursor_state)
		_cursor_state_just_changed = false;

	else if (_cursor_state != cursor_state)
		_cursor_state_just_changed = true;
	
	_cursor_state = cursor_state;
	
}

void Ui::begin(Layout::LayoutType type){
	Vec2<float> position;
	if (layouts.size() > 0)
		position = layouts.back().get_widget_position();
	layouts.push_back(Layout(type, position));
}

bool Ui::box(const std::string& id, const Vec2<float>& size, const Style& style){
	
	_position_calculation_calls.push_back([=](bool& hover_out, AABB2& aabb_out, StaticStyle& style_to_use_out) {

		// determine aabbs
		Vec2<float> screen_position = layouts.back().get_widget_position();
		Vec2<float> screen_position_hover = screen_position;

		WidgetInfo& old_info = widget_info_table[id];
		StaticStyle interpolated_style = interpolate_styles(style, old_info);

		Vec4<float> _style_margin = optional_get<Vec4<float>>({ interpolated_style.margin });
		Vec4<float> _style_padding = optional_get<Vec4<float>>({ interpolated_style.padding });

		Vec4<float> _style_margin_hover = optional_get<Vec4<float>>({ interpolated_style.margin });
		Vec4<float> _style_padding_hover = optional_get<Vec4<float>>({ interpolated_style.padding });

		screen_position = screen_position + Vec2<float>(_style_margin.y + _style_padding.y, _style_margin.x + _style_padding.x);
		screen_position_hover = screen_position_hover + Vec2<float>(_style_margin_hover.y + _style_padding_hover.y, _style_margin_hover.x + _style_padding_hover.x);

		Vec2<float> _size_default = Vec2<float>(size.x - _style_padding.y - _style_padding.w, size.y - _style_padding.x - _style_padding.z);
		Vec2<float> _size_hover = Vec2<float>(size.x - _style_padding_hover.y - _style_padding_hover.w, size.y - _style_padding_hover.x - _style_padding_hover.z);

		AABB2 default_box(screen_position, _size_default);
		AABB2 hover_box(screen_position_hover, _size_hover);
		AABB2 aabb_to_use;
		Vec2<int> cursor = frame.get_cursor_position();

		// determine hover
		bool hover;
		if (default_box.does_contain(cursor) && hover_box.does_contain(cursor)) {
			hover = true;
			_hovered_id = id;
			_hovered = true;
			aabb_to_use = hover_box;
		}
		else if (default_box.does_contain(cursor)) {
			hover = true;
			_hovered_id = id;
			_hovered = true;
			aabb_to_use = hover_box;
		}
		else if (hover_box.does_contain(cursor)) {
			if (_hovered_id == id) {
				hover = true;
				aabb_to_use = hover_box;
			}
			else {
				hover = false;
				aabb_to_use = default_box;
			}
		}
		else {
			hover = false;
			aabb_to_use = default_box;
		}

		// hover/active/passive update
		if (hover) {
			if ((_cursor_state == Frame::LeftPressed || _cursor_state == Frame::RightPressed) && _cursor_state_just_changed) {
				_focused_id = id;
			}
		}

		// determine style_to_use
		bool active = _focused_id == id;

		

		// insert object to layout

		Vec3<float> color =				optional_get<Vec3<float>>(interpolated_style.color);
		Vec2<float> displacement =		optional_get<Vec2<float>>(interpolated_style.displacement);
		Vec2<float> rotation_euler =	optional_get<Vec2<float>>(interpolated_style.rotation_euler);
		Vec4<float> corner_rounding =	optional_get<Vec4<float>>(interpolated_style.corner_rounding);
		Vec4<float> padding =			optional_get<Vec4<float>>(interpolated_style.padding);
		Vec4<float> margin =			optional_get<Vec4<float>>(interpolated_style.margin);
		Vec4<float> border_thickness =	optional_get<Vec4<float>>(interpolated_style.border_thickness);
		Vec3<float> border_color =		optional_get<Vec3<float>>(interpolated_style.border_color);
		Frame::CursorType cursor_type =	optional_get<Frame::CursorType>(interpolated_style.cursor_type);

		layouts.back().add_widget(Vec2<float>(size.x + margin.y + margin.w, size.y + margin.x + margin.z));

		// update widget_info 
		Time now = frame.get_time_sec();
		if (old_info.last_update == -1) old_info.last_update = now;
		
		Time deltatime = now - old_info.last_update;

		if (hover)		old_info.hover_duration += deltatime;
		if (!hover)		old_info.hover_duration -= deltatime;
		if (active)		old_info.active_duration += deltatime;
		if (!active)	old_info.active_duration -= deltatime;

		old_info.hover_duration = std::max(old_info.hover_duration, 0.0);
		old_info.active_duration = std::max(old_info.active_duration, 0.0);
		
		old_info.last_update = now;

		hover_out = hover;
		aabb_out = aabb_to_use;
		style_to_use_out = interpolated_style;
		});
	
	// render
	_render_calls.push_back([&](bool hover, AABB2 aabb, StaticStyle style_to_use) {
		
		//Vec3<float> color =				optional_get<Vec3<float>>({			style_to_use.color,				style.color });
		WidgetInfo& widget_info = widget_info_table[id];
		Vec3<float> color =				optional_get<Vec3<float>>(style_to_use.color);
		Vec2<float> displacement =		optional_get<Vec2<float>>(style_to_use.displacement);
		Vec2<float> rotation_euler =	optional_get<Vec2<float>>(style_to_use.rotation_euler);
		Vec4<float> corner_rounding =	optional_get<Vec4<float>>(style_to_use.corner_rounding);
		Vec4<float> padding =			optional_get<Vec4<float>>(style_to_use.padding);
		Vec4<float> margin =			optional_get<Vec4<float>>(style_to_use.margin);
		Vec4<float> border_thickness =	optional_get<Vec4<float>>(style_to_use.border_thickness);
		Vec3<float> border_color =		optional_get<Vec3<float>>(style_to_use.border_color);
		Frame::CursorType cursor_type =	optional_get<Frame::CursorType>(style_to_use.cursor_type);
		Vec2<float> padded_size = size - Vec2<float>(padding.y + padding.w, padding.x + padding.z);
		
		if (hover) frame.set_cursor_type(cursor_type);

		Mesh box_mesh(aabb.generate_model());
		Mesh_s box_mesh_s = Mesh_s(box_mesh);
		Program_s program_s = Program_s(program);
		Graphic box_graphic;

		box_graphic.load_program(program_s);
		box_graphic.load_model(box_mesh_s);
		box_graphic.set_uniform("screen_position", aabb.position.x, frame.window_height - aabb.position.y);
		box_graphic.set_uniform("projection", projection_matrix);
		box_graphic.set_uniform("rect_color", color.x, color.y, color.z, 1.0f);
		box_graphic.set_uniform("rect_size", padded_size.x, padded_size.y);
		box_graphic.set_uniform("corner_rounding", corner_rounding.x, corner_rounding.y, corner_rounding.z, corner_rounding.w);
		box_graphic.set_uniform("border_color", border_color.x, border_color.y, border_color.z, 1.0f);
		box_graphic.set_uniform("border_thickness", border_thickness.x, border_thickness.y, border_thickness.z, border_thickness.w);

		box_graphic.update_matrix();
		box_graphic.update_uniforms();
		box_graphic.draw(false);
		});

	return false; // temp
}

void Ui::end(const Style& style) {
	
	// widget positioning	
	std::vector<bool> widget_hovers;
	std::vector<AABB2> widget_aabbs;
	std::vector<StaticStyle> widget_styles;
	for (auto& positioning_call : _position_calculation_calls){
		bool widget_hover;
		AABB2 widget_aabb;
		StaticStyle widget_style;
		positioning_call(widget_hover, widget_aabb, widget_style);
	}

	// layout render
	Vec3<float> color =				optional_get<Vec3<float>>({			style.color				});
	Vec2<float> displacement =		optional_get<Vec2<float>>({			style.displacement		});
	Vec2<float> rotation_euler =	optional_get<Vec2<float>>({			style.rotation_euler	});
	Vec4<float> corner_rounding =	optional_get<Vec4<float>>({			style.corner_rounding	});
	Vec4<float> padding =			optional_get<Vec4<float>>({			style.padding			});
	Vec4<float> margin =			optional_get<Vec4<float>>({			style.margin			});
	Vec4<float> border_thickness =	optional_get<Vec4<float>>({			style.border_thickness	});
	Vec3<float> border_color =		optional_get<Vec3<float>>({			style.border_color		});
	Frame::CursorType cursor_type = optional_get<Frame::CursorType>({	style.cursor_type		});

	layouts.back().update_position_by_style(margin, padding);
	Vec2<float> size = layouts.back().window_size;
	Vec2<float> position = layouts.back().get_position();

	AABB2 aabb(position, size);
	bool layout_hover = aabb.does_contain(frame.get_cursor_position());
	
	if (layout_hover) {
		color =				optional_get<Vec3<float>>({			style.on_hover.color,				style.color				});
		displacement =		optional_get<Vec2<float>>({			style.on_hover.displacement,		style.displacement		});
		rotation_euler =	optional_get<Vec2<float>>({			style.on_hover.rotation_euler,		style.rotation_euler	});
		corner_rounding =	optional_get<Vec4<float>>({			style.on_hover.corner_rounding,		style.corner_rounding	});
		padding =			optional_get<Vec4<float>>({			style.on_hover.padding,				style.padding			});
		margin =			optional_get<Vec4<float>>({			style.on_hover.margin,				style.margin			});
		border_thickness =	optional_get<Vec4<float>>({			style.on_hover.border_thickness,	style.border_thickness	});
		border_color =		optional_get<Vec3<float>>({			style.on_hover.border_color,		style.border_color		});
		cursor_type =		optional_get<Frame::CursorType>({	style.on_hover.cursor_type,			style.cursor_type		});
	}

	layouts.back().clear();
	layouts.back().update_position_by_style(margin, padding);

	for (auto& positioning_call : _position_calculation_calls) {
		bool widget_hover;
		AABB2 widget_aabb;
		StaticStyle widget_style;
		positioning_call(widget_hover, widget_aabb, widget_style);
		widget_hovers.push_back(widget_hover);
		widget_aabbs.push_back(widget_aabb);
		widget_styles.push_back(widget_style);
	}

	position = layouts.back().get_position();
	size = layouts.back().window_size;

	Mesh box_mesh(aabb.generate_model());
	Mesh_s box_mesh_s = Mesh_s(box_mesh);
	Program_s program_s = Program_s(program);
	Graphic box_graphic;

	box_graphic.load_program(program_s);
	box_graphic.load_model(box_mesh_s);
	box_graphic.set_uniform("screen_position", position.x, frame.window_height - position.y);
	box_graphic.set_uniform("projection", projection_matrix);
	box_graphic.set_uniform("rect_color", color.x, color.y, color.z, 1.0f);
	box_graphic.set_uniform("rect_size", size.x, size.y);
	box_graphic.set_uniform("corner_rounding", corner_rounding.x, corner_rounding.y, corner_rounding.z, corner_rounding.w);
	box_graphic.set_uniform("border_color", border_color.x, border_color.y, border_color.z, 1.0f);
	box_graphic.set_uniform("border_thickness", border_thickness.x, border_thickness.y, border_thickness.z, border_thickness.w);

	box_graphic.update_matrix();
	box_graphic.update_uniforms();
	box_graphic.draw(false);
	
	// widget render
	for (uint32_t i = 0; i < _render_calls.size(); i++) {
		auto& render_call = _render_calls[i];
		render_call(widget_hovers[i], widget_aabbs[i], widget_styles[i]);
	}

	// update layouts
	layouts.pop_back();
	layouts.back().add_widget(size);
}

void Ui::_update_matrix(int screen_width, int screen_height) {
	
	projection_matrix = glm::ortho(0.0f, (float)screen_width, 0.0f, (float)screen_height);
}