#include "Gui.h"
#include "Graphic.h"
#include "Default_Programs.h"

#include <algorithm>

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

_interpolation Interpolation::linear() {
	_interpolation result;
	result.interpolation_type = _interpolation::Linear;
	return result;
}

_interpolation Interpolation::polynomial(float power) {
	_interpolation result;
	result.interpolation_type = _interpolation::Polynomial;
	result.power = power;
	return result;
}

vec3f gui::colorcode(uint32_t hexcode) {
	color result;
	uint8_t* bytes = (uint8_t*)&hexcode;
	result.x = *(bytes + 2) / 255.0f;
	result.y = *(bytes + 1) / 255.0f;
	result.z = *(bytes + 0) / 255.0f;
	return result;
}

namespace {
	template<typename T>
	T optional_get(const std::vector<std::optional<T>>& fallback_list, T null_value = T()) {
		for (const std::optional<T>& _property: fallback_list) {
			if (_property)
				return _property.value();
		}
		return null_value;
	}

	template<typename T>
	T optional_get(const std::optional<T>& optional, T null_value = T()) {
		if (optional)
			return optional.value();
		return null_value;
	}

	template<typename T>
	T style_attribute_get(const std::vector<StyleAttribute<T>>& fallback_list, const _widget_info& info, T null_value = T()) {	// TODO: implement for Persentage
		for (const StyleAttribute<T>& attribute : fallback_list) {
			if (attribute.exist())
				return attribute.get_value(info);
		}
		return null_value;
	}

	template<typename T>
	T style_attribute_get(const StyleAttribute<T>& attribute, const _widget_info& info, T null_value = T()) {	// TODO: implement for Persentage
		if (attribute.exist())
			return attribute.get_value(info);
		return null_value;
	}

	StaticStyle interpolate_styles(Style style, StaticStyle target_style, _widget_info& widget_info, bool hover) {
		// insert object to layout
		Vec3<float> color =				style_attribute_get<Vec3<float>>({	target_style.color,				style.color }, widget_info);
		Vec2<float> displacement =		style_attribute_get<Vec2<float>>({	target_style.displacement,		style.displacement }, widget_info);
		Vec2<float> rotation_euler =	style_attribute_get<Vec2<float>>({	target_style.rotation_euler,	style.rotation_euler }, widget_info);
		Vec4<float> corner_rounding =	style_attribute_get<Vec4<float>>({	target_style.corner_rounding,	style.corner_rounding}, widget_info);
		Vec4<float> padding =			style_attribute_get<Vec4<float>>({	target_style.padding,			style.padding }, widget_info);
		Vec4<float> margin =			style_attribute_get<Vec4<float>>({	target_style.margin,			style.margin }, widget_info);
		Vec4<float> border_thickness =	style_attribute_get<Vec4<float>>({	target_style.border_thickness,	style.border_thickness }, widget_info);
		Vec3<float> border_color =		style_attribute_get<Vec3<float>>({	target_style.border_color,		style.border_color }, widget_info);
		Frame::CursorType cursor_type = optional_get<Frame::CursorType>({	target_style.cursor_type,		style.cursor_type });
		
		Vec3<float> color_default =				style_attribute_get<Vec3<float>>({	style.color }, widget_info);
		Vec2<float> displacement_default =		style_attribute_get<Vec2<float>>({	style.displacement }, widget_info);
		Vec2<float> rotation_euler_default =	style_attribute_get<Vec2<float>>({	style.rotation_euler }, widget_info);
		Vec4<float> corner_rounding_default =	style_attribute_get<Vec4<float>>({	style.corner_rounding }, widget_info);
		Vec4<float> padding_default =			style_attribute_get<Vec4<float>>({	style.padding }, widget_info);
		Vec4<float> margin_default =			style_attribute_get<Vec4<float>>({	style.margin }, widget_info);
		Vec4<float> border_thickness_default =	style_attribute_get<Vec4<float>>({	style.border_thickness }, widget_info);
		Vec3<float> border_color_default =		style_attribute_get<Vec3<float>>({	style.border_color }, widget_info);
		Frame::CursorType cursor_type_default = optional_get<Frame::CursorType>({	style.cursor_type });

		Time color_change =				style.color_change.value_or(0.0);
		Time displacement_change =		style.displacement_change.value_or(0.0);
		Time rotation_change =			style.rotation_change.value_or(0.0);
		Time corner_rounding_change =	style.corner_rounding_change.value_or(0.0);
		Time padding_change =			style.padding_change.value_or(0.0);
		Time margin_change =			style.margin_change.value_or(0.0);
		Time border_thickness_change =	style.border_thickness_change.value_or(0.0);
		Time border_color_change =		style.border_color_change.value_or(0.0);

		widget_info._current_color_time =				std::min(color_change, widget_info._current_color_time);
		widget_info._current_displacement_time =		std::min(displacement_change, widget_info._current_displacement_time);
		widget_info._current_rotation_time =			std::min(rotation_change, widget_info._current_rotation_time);
		widget_info._current_corner_rounding_time =		std::min(corner_rounding_change, widget_info._current_corner_rounding_time);
		widget_info._current_padding_time =				std::min(padding_change, widget_info._current_padding_time);
		widget_info._current_margin_time =				std::min(margin_change, widget_info._current_margin_time);
		widget_info._current_border_thickness_time =	std::min(border_thickness_change, widget_info._current_border_thickness_time);
		widget_info._current_border_color_time =		std::min(border_color_change, widget_info._current_border_color_time);

		widget_info._current_color_time =				std::max((Time)0.0f, widget_info._current_color_time);
		widget_info._current_displacement_time =		std::max((Time)0.0f, widget_info._current_displacement_time);
		widget_info._current_rotation_time =			std::max((Time)0.0f, widget_info._current_rotation_time);
		widget_info._current_corner_rounding_time =		std::max((Time)0.0f, widget_info._current_corner_rounding_time);
		widget_info._current_padding_time =				std::max((Time)0.0f, widget_info._current_padding_time);
		widget_info._current_margin_time =				std::max((Time)0.0f, widget_info._current_margin_time);
		widget_info._current_border_thickness_time =	std::max((Time)0.0f, widget_info._current_border_thickness_time);
		widget_info._current_border_color_time =		std::max((Time)0.0f, widget_info._current_border_color_time);

		StaticStyle result;
		
		if (color_change > 0)				result.color =				style.color_interpolation.value_or(Interpolation::linear()).interpolate(			color_default,				color,				color_change,				widget_info._current_color_time);
		else if (hover)						result.color =				color; 
		else								result.color =				color_default; 
		if (displacement_change > 0)		result.displacement =		style.displacement_interpolation.value_or(Interpolation::linear()).interpolate(		displacement_default,		displacement,		displacement_change,		widget_info._current_displacement_time);
		else if (hover)						result.displacement =		displacement; 
		else								result.displacement =		displacement_default; 
		if (rotation_change > 0)			result.rotation_euler =		style.rotation_interpolation.value_or(Interpolation::linear()).interpolate(			rotation_euler_default,		rotation_euler,		rotation_change,			widget_info._current_rotation_time);
		else if (hover)						result.rotation_euler =		rotation_euler;
		else								result.rotation_euler =		rotation_euler_default;
		if (corner_rounding_change > 0)		result.corner_rounding =	style.corner_rounding_interpolation.value_or(Interpolation::linear()).interpolate(	corner_rounding_default,	corner_rounding,	corner_rounding_change,		widget_info._current_corner_rounding_time);
		else if (hover)						result.corner_rounding =	corner_rounding;
		else								result.corner_rounding =	corner_rounding_default;
		if (padding_change > 0)				result.padding =			style.padding_interpolation.value_or(Interpolation::linear()).interpolate(			padding_default,			padding,			padding_change,				widget_info._current_padding_time);
		else if (hover)						result.padding =			padding;
		else								result.padding =			padding_default;
		if (margin_change > 0)				result.margin =				style.margin_interpolation.value_or(Interpolation::linear()).interpolate(			margin_default,				margin,				margin_change,				widget_info._current_margin_time);
		else if (hover)						result.margin =				margin;
		else 								result.margin =				margin_default;
		if (border_thickness_change > 0)	result.border_thickness =	style.border_thickness_interpolation.value_or(Interpolation::linear()).interpolate(	border_thickness_default,	border_thickness,	border_thickness_change,	widget_info._current_border_thickness_time);
		else if (hover)						result.border_thickness =	border_thickness;
		else 								result.border_thickness =	border_thickness_default;
		if (border_color_change > 0)		result.border_color =		style.border_color_interpolation.value_or(Interpolation::linear()).interpolate(		border_color_default,		border_color,		border_color_change,		widget_info._current_border_color_time);
		else if (hover)						result.border_color =		border_color;
		else 								result.border_color =		border_color_default;
		
		if (hover)	result.cursor_type = cursor_type;
		else		result.cursor_type = cursor_type_default;

		return result;
	}

	StaticStyle interpolate_styles(Style style, _widget_info& widget_info, bool hover) {
		StaticStyle style_to_use = style;
		if (widget_info.is_hovering) {
			style_to_use = style.on_hover;
		}
		return interpolate_styles(style, style_to_use, widget_info, hover);
	}

	StaticStyle merge_styles_by_priority(const std::vector<StaticStyle>& styles, const _widget_info& info) {
		std::vector<StyleAttribute<vec3f>> colors;
		std::vector<StyleAttribute<vec2f>> displacements;
		std::vector<StyleAttribute<vec2f>> rotation_eulers;
		std::vector<StyleAttribute<vec4f>> corner_roundings;
		std::vector<StyleAttribute<vec4f>> paddings;
		std::vector<StyleAttribute<vec4f>> margins;
		std::vector<StyleAttribute<vec4f>> border_thicknesss;
		std::vector<StyleAttribute<vec3f>> border_colors;
		std::vector<std::optional<Frame::CursorType>> cursor_types;
		
		StaticStyle merged_style;

		for (const StaticStyle& style : styles) {
			if (style.color.exist())
				merged_style.color = style.color;
		}
		for (const StaticStyle& style : styles) {
			if (style.displacement.exist())
				merged_style.displacement = style.displacement;
		}
		for (const StaticStyle& style : styles) {
			if (style.rotation_euler.exist())
				merged_style.rotation_euler = style.rotation_euler;
		}
		for (const StaticStyle& style : styles) {
			if (style.corner_rounding.exist())
				merged_style.corner_rounding = style.corner_rounding;
		}
		for (const StaticStyle& style : styles) {
			if (style.padding.exist())
				merged_style.padding = style.padding;
		}
		for (const StaticStyle& style : styles) {
			if (style.margin.exist())
				merged_style.margin = style.margin;
		}
		for (const StaticStyle& style : styles) {
			if (style.border_thickness.exist())
				merged_style.border_thickness = style.border_thickness;
		}
		for (const StaticStyle& style : styles) {
			if (style.border_color.exist())
				merged_style.border_color = style.border_color;
		}
		for (const StaticStyle& style : styles) {
			if (style.cursor_type)
				merged_style.cursor_type = style.cursor_type.value();
		}

		return merged_style;
	}

	Style merge_static_style_with_style(const StaticStyle& static_style, Style& style, const _widget_info& info) {
		StaticStyle merged_style = merge_styles_by_priority({ static_style, style }, info);
		Style style_copy = style;
		
		style_copy.color = merged_style.color;
		style_copy.displacement = merged_style.displacement;
		style_copy.rotation_euler = merged_style.rotation_euler;
		style_copy.corner_rounding = merged_style.corner_rounding;
		style_copy.padding = merged_style.padding;
		style_copy.margin = merged_style.margin;
		style_copy.border_thickness = merged_style.border_thickness;
		style_copy.border_color = merged_style.border_color;
		style_copy.cursor_type = merged_style.cursor_type;

		return style_copy;
	}
	/*
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
	*/
}

std::string Persentage::RespectedAttribute_to_string(const RespectedAttribute& attribute) {
	switch (attribute) {
	case SIZE_X:
		return "SIZE_X";
		break;
	case SIZE_Y:
		return "SIZE_Y";
		break;
	case PADDED_SIZE_X:
		return "PADDED_SIZE_X";
		break;
	case PADDED_SIZE_Y:
		return "PADDED_SIZE_Y";
		break;
	case POSITION_X:
		return "POSITION_X";
		break;
	case POSITION_Y:
		return "POSITION_Y";
		break;
	case PADDED_POSITION_X:
		return "PADDED_POSITION_X";
		break;
	case PADDED_POSITION_Y:
		return "PADDED_POSITION_Y";
		break;
	case PARENT_WIDTH:
		return "PARENT_WIDTH";
		break;
	case PARENT_HEIGHT:
		return "PARENT_HEIGHT";
		break;
	case AVAILABLE_WIDTH:
		return "AVAILABLE_WIDTH";
		break;
	case AVAILABLE_HEIGHT:
		return "AVAILABLE_HEIGHT";
		break;
	}
}

Persentage::Persentage(float value, RespectedAttribute attribute_type) : 
	value(value), attribute_type(attribute_type) {}

float Persentage::get_value(const _widget_info& info) const {
	switch (attribute_type) {
	case SIZE_X :
		return value * info.size.x;	
		break;
	case SIZE_Y :
		return value * info.size.y;
		break;
	case PADDED_SIZE_X :
		return 0;	
		break;
	case PADDED_SIZE_Y :
		return 0;	
		break;
	case MAX_SIZE_DIM: {
		float max_size = info.size.x;
		if (info.size.y > max_size)
			max_size = info.size.y;
		return value * max_size;
		break;
	}
	case MIN_SIZE_DIM: {
		float min_size = info.size.x;
		if (info.size.y < min_size)
			min_size = info.size.y;
		return value * min_size;
		break;
	}
	case POSITION_X :
		return value * info.position.x;
		break;
	case POSITION_Y :
		return value * info.position.y;
		break;
	case PADDED_POSITION_X :
		return 0;	
		break;
	case PADDED_POSITION_Y :
		return 0;	
		break;
	case PARENT_WIDTH :
		return 0;	
		break;
	case PARENT_HEIGHT :
		return 0;	
		break;
	case AVAILABLE_WIDTH :
		return 0;	
		break;
	case AVAILABLE_HEIGHT :
		return 0;
		break;
	}
	return 0;
}

// object based system

void _widget_info::increment_time(Time deltatime) {
	//last_update = now;
	if (is_hovering) {
		_current_color_time += deltatime;
		_current_displacement_time += deltatime;
		_current_rotation_time += deltatime;
		_current_corner_rounding_time += deltatime;
		_current_padding_time += deltatime;
		_current_margin_time += deltatime;
		_current_border_thickness_time += deltatime;
		_current_border_color_time += deltatime;
	}
	else {
		_current_color_time -= deltatime;
		_current_displacement_time -= deltatime;
		_current_rotation_time -= deltatime;
		_current_corner_rounding_time -= deltatime;
		_current_padding_time -= deltatime;
		_current_margin_time -= deltatime;
		_current_border_thickness_time -= deltatime;
		_current_border_color_time -= deltatime;
	}
}

Box::Box(Frame& frame, Style style, AABB2 aabb, uint32_t id) :
	_frame_ref(frame), _style(style), _aabb(aabb), _original_size(aabb.size), _original_position(aabb.position), _id(id)
{
	Gui::_initialize();
	
	_graphic_representation->load_program(Gui::default_gui_renderer);
	_graphic_representation->load_model(Mesh_s(_aabb.generate_model()));
	//_update_matrix(_frame_ref.window_width, _frame_ref.window_height);

	_info.size = _original_size;
	_info.position = _original_position;
}

Box::Box(Frame& frame, Style style, AABB2 aabb, Program_s custom_renderer, uint32_t id):
	_frame_ref(frame), _style(style), _aabb(aabb), _original_size(aabb.size), _original_position(aabb.position), _id(id)
{
	Gui::_initialize();

	_graphic_representation->load_program(custom_renderer);
	_graphic_representation->load_model(Mesh_s(_aabb.generate_model()));
	//_update_matrix(_frame_ref.window_width, _frame_ref.window_height);

	_info.size = _original_size;
	_info.position = _original_position;
}

void Box::set_position(Vec2<float> position){
	_original_position = position;
	_info.position = position;
}

void Box::set_size(Vec2<float> size){
	_info.size = size;
	_original_size = size;
}

bool Box::is_mouse_hover(){
	return _aabb.does_contain(_frame_ref.get_cursor_position());
}


void Box::render(){

	StaticStyle default_style = merge_styles_by_priority({ overwrite_style, _style }, _info);
	StaticStyle hover_style = merge_styles_by_priority({ overwrite_style.on_hover, _style.on_hover, default_style }, _info);
	
	StaticStyle style_to_use = default_style;
	_info.is_hovering = false;
	bool hover = is_mouse_hover();
	if (hover) {
	//	style_to_use = hover_style;
		_info.is_hovering = true;
		Gui::_hover_happened = true;
	}
	Style base_style = merge_static_style_with_style(default_style, _style, _info);
	Style target_style = merge_static_style_with_style(hover_style, _style, _info);	// append timing information of style to style_to_use, TODO: merge timing information of _style and overwrite_style here
	StaticStyle interpolated_style = interpolate_styles(base_style, target_style, _info, hover);
	Vec3<float> color =				style_attribute_get<Vec3<float>>( interpolated_style.color, _info);
	Vec2<float> displacement =		style_attribute_get<Vec2<float>>( interpolated_style.displacement, _info);
	Vec2<float> rotation_euler =	style_attribute_get<Vec2<float>>( interpolated_style.rotation_euler, _info);
	Vec4<float> corner_rounding =	style_attribute_get<Vec4<float>>( interpolated_style.corner_rounding, _info);
	Vec4<float> padding =			style_attribute_get<Vec4<float>>( interpolated_style.padding, _info);
	Vec4<float> margin =			style_attribute_get<Vec4<float>>( interpolated_style.margin, _info);
	Vec4<float> border_thickness =	style_attribute_get<Vec4<float>>( interpolated_style.border_thickness, _info);
	Vec3<float> border_color =		style_attribute_get<Vec3<float>>( interpolated_style.border_color, _info);
	Frame::CursorType cursor_type =	optional_get<Frame::CursorType>	( interpolated_style.cursor_type );

	Vec2<float> padded_size = _original_size - Vec2<float>(padding.y + padding.w, padding.x + padding.z);
	Vec2<float> displaced_position = _original_position + displacement;
	if (_aabb.position != displaced_position) {
		_aabb.position = displaced_position;
	}
	if (_aabb.size != padded_size) {
		_aabb.size = padded_size;
		_graphic_representation->mesh->load_model(_aabb.generate_model());
	}

	Gui::default_gui_renderer->update_uniform(Gui::_default_uniform_screen_position,	_aabb.position.x, _frame_ref.window_height - _aabb.position.y);
	//Gui::default_gui_renderer->update_uniform(Gui::_default_uniform_projection, Gui::_projection_matrix);
	Gui::default_gui_renderer->update_uniform(Gui::_default_uniform_rect_color,			color.x, color.y, color.z, 1.0f);
	Gui::default_gui_renderer->update_uniform(Gui::_default_uniform_rect_size,			padded_size.x, padded_size.y);
	Gui::default_gui_renderer->update_uniform(Gui::_default_uniform_corner_rounding,	corner_rounding.x, corner_rounding.y, corner_rounding.z, corner_rounding.w);
	Gui::default_gui_renderer->update_uniform(Gui::_default_uniform_border_color,		border_color.x, border_color.y, border_color.z, 1.0f);
	Gui::default_gui_renderer->update_uniform(Gui::_default_uniform_border_thickness,	border_thickness.x, border_thickness.y, border_thickness.z, border_thickness.w);

	_graphic_representation->update_matrix();
	//_graphic_representation->update_uniforms();
	_graphic_representation->draw(false);
	
	if (hover)
		_frame_ref.set_cursor_type(cursor_type);

	//_info.last_update = _frame_ref.get_time_sec();
	_info.increment_time(Gui::_frame_time_ms / 1000.0f);
}


vec2f Box::get_size(){
	return _original_size;
}
vec2f Box::get_position(){
	return _original_position;
}

glm::mat4 Gui::_projection_matrix;
Vec2<int> Gui::window_size;
bool Gui::_hover_happened;
Program_s Gui::default_gui_renderer;	// can only be initialized after initializing opengl, thus initialized by Gui::new_frame() 
bool Gui::_initialized = false;
std::vector<Box> Gui::widget_table;
std::vector<std::function<void(Time)>> Gui::render_queue;
Time Gui::_frame_time_ms;
uint32_t Gui::_widget_next_id;
unsigned int Gui::_default_uniform_screen_position;
unsigned int Gui::_default_uniform_projection;
unsigned int Gui::_default_uniform_rect_color;
unsigned int Gui::_default_uniform_rect_size;
unsigned int Gui::_default_uniform_corner_rounding;
unsigned int Gui::_default_uniform_border_color;
unsigned int Gui::_default_uniform_border_thickness;


void Gui::new_frame(Frame& frame, Time frame_time_ms) {
	_initialize();
	_frame_time_ms = frame_time_ms;

	vec2i current_window_size(frame.window_width, frame.window_height);
	if (window_size != current_window_size){
		window_size = current_window_size;
		_projection_matrix = glm::ortho(0.0f, (float)window_size.x, 0.0f, (float)window_size.y);
		Gui::default_gui_renderer->update_uniform(Gui::_default_uniform_projection, Gui::_projection_matrix);
	}
	if (!_hover_happened)
		frame.set_cursor_type(Frame::Arrow);
	_hover_happened = false;

	render_queue.clear();
	_widget_next_id = 0;
}

void Gui::_initialize() {
	if (!_initialized){
		default_gui_renderer = default_program::gui_program_s();
		_default_uniform_screen_position = default_gui_renderer->define_get_uniform_id("screen_position");
		_default_uniform_projection = default_gui_renderer->define_get_uniform_id("projection");
		_default_uniform_rect_color = default_gui_renderer->define_get_uniform_id("rect_color");
		_default_uniform_rect_size = default_gui_renderer->define_get_uniform_id("rect_size");
		_default_uniform_corner_rounding = default_gui_renderer->define_get_uniform_id("corner_rounding");
		_default_uniform_border_color = default_gui_renderer->define_get_uniform_id("border_color");
		_default_uniform_border_thickness = default_gui_renderer->define_get_uniform_id("border_thickness");

	}
	_initialized = true;
}

void Gui::_destroy() {
	widget_table.clear();
}


void Gui::render(Time deltatime) {
	//for (auto widget : widget_table) {
	//	widget.render(deltatime);
	//}
	for (auto render_call : render_queue) {
		render_call(deltatime);
	}
}

Box& Gui::box(AABB2 aabb, Style style, Frame& frame) {
	// append to table if mentioned for the first time
	uint32_t next_id = _widget_next_id++;
	if (widget_table.size() >= next_id + 1) {
		if (widget_table[next_id]._id == next_id) {
			return widget_table[next_id];
		}
	}
	auto same_name = [next_id](Box box) { return box._id == next_id; };
	auto found_box = std::find_if(widget_table.begin(), widget_table.end(), same_name);
	if (found_box == widget_table.end()) {
		widget_table.push_back(Box (frame, style, aabb, next_id));
		int widget_count = widget_table.size() - 1;
		return widget_table[widget_count];
	}
	else {
		return (*found_box);
	}
}

/*
// css-like system

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

		Mesh_s box_mesh_s = Mesh_s(aabb.generate_model());
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

	Mesh_s box_mesh_s = Mesh_s(aabb.generate_model());
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
*/