#include "Gui.h"
#include "Default_Programs.h"

unsigned int compute_gui_id(std::string filename, int line) {
	std::hash<std::string> hash;
	return hash(filename) * 2 + 3 * line;
}

// Layout system

Layout::Layout(uint32_t type, const AABB2& aabb) :
	type((Layout::LayoutType)type), window_size(aabb.size), position(aabb.position) {}

Layout::Layout(uint32_t type, const Vec2<float>& position, const Vec2<float>& window_size) :
	type((Layout::LayoutType)type), window_size(window_size), position(position) {}

void Layout::add_widget(const Vec2<float>& size) {
	widget_positions.push_back(get_widget_position());

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

Vec2<float> Layout::get_centered_widget_position(unsigned int widget_index, const Vec2<float>& object_size) {
	vec2 widget_default_pos = widget_positions[widget_index];
	//return widget_default_pos;
	if (type == Layout::Horizional) {
		return Vec2<float>(widget_default_pos.x, widget_default_pos.y + window_size.y / 2 - object_size.y / 2);
	}
	else if (type == Layout::Vertical) {
		return Vec2<float>(widget_default_pos.x + window_size.x / 2 - object_size.x / 2, widget_default_pos.y);
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

// Style system
namespace{
	template<typename T>
	T optional_get(const std::vector<std::optional<T>>& fallback_list, T null_value = T()) {
		for (const std::optional<T>& _property : fallback_list) {
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
		Vec3<float> text_color = style_attribute_get<Vec3<float>>({ target_style.text_color,			style.text_color }, widget_info);
		float text_size = style_attribute_get<float>({ target_style.text_size,							style.text_size }, widget_info, 12.0f);
		Vec3<float> color = style_attribute_get<Vec3<float>>({ target_style.color,						style.color }, widget_info);
		Vec2<float> displacement = style_attribute_get<Vec2<float>>({ target_style.displacement,		style.displacement }, widget_info);
		Vec2<float> rotation_euler = style_attribute_get<Vec2<float>>({ target_style.rotation_euler,	style.rotation_euler }, widget_info);
		Vec4<float> corner_rounding = style_attribute_get<Vec4<float>>({ target_style.corner_rounding,	style.corner_rounding }, widget_info);
		Vec4<float> padding = style_attribute_get<Vec4<float>>({ target_style.padding,					style.padding }, widget_info);
		Vec4<float> margin = style_attribute_get<Vec4<float>>({ target_style.margin,					style.margin }, widget_info);
		Vec4<float> border_thickness = style_attribute_get<Vec4<float>>({ target_style.border_thickness,style.border_thickness }, widget_info);
		Vec3<float> border_color = style_attribute_get<Vec3<float>>({ target_style.border_color,		style.border_color }, widget_info);
		Frame::CursorType cursor_type = optional_get<Frame::CursorType>({ target_style.cursor_type,		style.cursor_type });
		Style::Stacking stacking_type = optional_get<Style::Stacking>({ target_style.stacking_type,		style.stacking_type});
		Style::TextAlligning text_allign_type = optional_get<Style::TextAlligning>({ target_style.text_allign_type,		style.text_allign_type});


		Vec3<float> text_color_default = style_attribute_get<Vec3<float>>({ style.text_color }, widget_info);
		float text_size_default = style_attribute_get<float>({ style.text_size }, widget_info, 12.0f);
		Vec3<float> color_default = style_attribute_get<Vec3<float>>({ style.color }, widget_info);
		Vec2<float> displacement_default = style_attribute_get<Vec2<float>>({ style.displacement }, widget_info);
		Vec2<float> rotation_euler_default = style_attribute_get<Vec2<float>>({ style.rotation_euler }, widget_info);
		Vec4<float> corner_rounding_default = style_attribute_get<Vec4<float>>({ style.corner_rounding }, widget_info);
		Vec4<float> padding_default = style_attribute_get<Vec4<float>>({ style.padding }, widget_info);
		Vec4<float> margin_default = style_attribute_get<Vec4<float>>({ style.margin }, widget_info);
		Vec4<float> border_thickness_default = style_attribute_get<Vec4<float>>({ style.border_thickness }, widget_info);
		Vec3<float> border_color_default = style_attribute_get<Vec3<float>>({ style.border_color }, widget_info);
		Frame::CursorType cursor_type_default = optional_get<Frame::CursorType>({ style.cursor_type });
		Style::Stacking stacking_type_default = optional_get<Style::Stacking>({ style.stacking_type });
		Style::TextAlligning text_allign_type_default = optional_get<Style::TextAlligning>({ style.text_allign_type });

		Time text_color_change = style.text_color_change.value_or(0.0);
		Time text_size_change = style.text_size_change.value_or(0.0);
		Time color_change = style.color_change.value_or(0.0);
		Time displacement_change = style.displacement_change.value_or(0.0);
		Time rotation_change = style.rotation_change.value_or(0.0);
		Time corner_rounding_change = style.corner_rounding_change.value_or(0.0);
		Time padding_change = style.padding_change.value_or(0.0);
		Time margin_change = style.margin_change.value_or(0.0);
		Time border_thickness_change = style.border_thickness_change.value_or(0.0);
		Time border_color_change = style.border_color_change.value_or(0.0);

		widget_info._current_text_color_time = std::min(text_color_change, widget_info._current_text_color_time);
		widget_info._current_text_size_time = std::min(text_size_change, widget_info._current_text_size_time);
		widget_info._current_color_time = std::min(color_change, widget_info._current_color_time);
		widget_info._current_displacement_time = std::min(displacement_change, widget_info._current_displacement_time);
		widget_info._current_rotation_time = std::min(rotation_change, widget_info._current_rotation_time);
		widget_info._current_corner_rounding_time = std::min(corner_rounding_change, widget_info._current_corner_rounding_time);
		widget_info._current_padding_time = std::min(padding_change, widget_info._current_padding_time);
		widget_info._current_margin_time = std::min(margin_change, widget_info._current_margin_time);
		widget_info._current_border_thickness_time = std::min(border_thickness_change, widget_info._current_border_thickness_time);
		widget_info._current_border_color_time = std::min(border_color_change, widget_info._current_border_color_time);

		widget_info._current_text_color_time = std::max((Time)0.0f, widget_info._current_text_color_time);
		widget_info._current_text_size_time = std::max((Time)0.0f, widget_info._current_text_size_time);
		widget_info._current_color_time = std::max((Time)0.0f, widget_info._current_color_time);
		widget_info._current_displacement_time = std::max((Time)0.0f, widget_info._current_displacement_time);
		widget_info._current_rotation_time = std::max((Time)0.0f, widget_info._current_rotation_time);
		widget_info._current_corner_rounding_time = std::max((Time)0.0f, widget_info._current_corner_rounding_time);
		widget_info._current_padding_time = std::max((Time)0.0f, widget_info._current_padding_time);
		widget_info._current_margin_time = std::max((Time)0.0f, widget_info._current_margin_time);
		widget_info._current_border_thickness_time = std::max((Time)0.0f, widget_info._current_border_thickness_time);
		widget_info._current_border_color_time = std::max((Time)0.0f, widget_info._current_border_color_time);

		StaticStyle result;

		if (text_color_change > 0)			result.text_color = style.text_color_interpolation.value_or(Interpolation::linear()).interpolate(text_color_default, text_color, text_color_change, widget_info._current_text_color_time);
		else if (hover)						result.text_color = text_color;
		else								result.text_color = text_color_default;
		if (text_size_change > 0)			result.text_size = style.text_size_interpolation.value_or(Interpolation::linear()).interpolate(text_size_default, text_size, text_size_change, widget_info._current_text_size_time);
		else if (hover)						result.text_size = text_size;
		else								result.text_size = text_size_default;
		if (color_change > 0)				result.color = style.color_interpolation.value_or(Interpolation::linear()).interpolate(color_default, color, color_change, widget_info._current_color_time);
		else if (hover)						result.color = color;
		else								result.color = color_default;
		if (displacement_change > 0)		result.displacement = style.displacement_interpolation.value_or(Interpolation::linear()).interpolate(displacement_default, displacement, displacement_change, widget_info._current_displacement_time);
		else if (hover)						result.displacement = displacement;
		else								result.displacement = displacement_default;
		if (rotation_change > 0)			result.rotation_euler = style.rotation_interpolation.value_or(Interpolation::linear()).interpolate(rotation_euler_default, rotation_euler, rotation_change, widget_info._current_rotation_time);
		else if (hover)						result.rotation_euler = rotation_euler;
		else								result.rotation_euler = rotation_euler_default;
		if (corner_rounding_change > 0)		result.corner_rounding = style.corner_rounding_interpolation.value_or(Interpolation::linear()).interpolate(corner_rounding_default, corner_rounding, corner_rounding_change, widget_info._current_corner_rounding_time);
		else if (hover)						result.corner_rounding = corner_rounding;
		else								result.corner_rounding = corner_rounding_default;
		if (padding_change > 0)				result.padding = style.padding_interpolation.value_or(Interpolation::linear()).interpolate(padding_default, padding, padding_change, widget_info._current_padding_time);
		else if (hover)						result.padding = padding;
		else								result.padding = padding_default;
		if (margin_change > 0)				result.margin = style.margin_interpolation.value_or(Interpolation::linear()).interpolate(margin_default, margin, margin_change, widget_info._current_margin_time);
		else if (hover)						result.margin = margin;
		else 								result.margin = margin_default;
		if (border_thickness_change > 0)	result.border_thickness = style.border_thickness_interpolation.value_or(Interpolation::linear()).interpolate(border_thickness_default, border_thickness, border_thickness_change, widget_info._current_border_thickness_time);
		else if (hover)						result.border_thickness = border_thickness;
		else 								result.border_thickness = border_thickness_default;
		if (border_color_change > 0)		result.border_color = style.border_color_interpolation.value_or(Interpolation::linear()).interpolate(border_color_default, border_color, border_color_change, widget_info._current_border_color_time);
		else if (hover)						result.border_color = border_color;
		else 								result.border_color = border_color_default;

		if (hover)	result.cursor_type = cursor_type;
		else		result.cursor_type = cursor_type_default;
		if (hover)	result.stacking_type = stacking_type;
		else		result.stacking_type = stacking_type_default;
		if (hover)	result.text_allign_type = text_allign_type;
		else		result.text_allign_type = text_allign_type_default;

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
		StaticStyle merged_style;
		
		for (const StaticStyle& style : styles) {
			if (style.text_color.exist()) {
				merged_style.text_color = style.text_color;
				break;
			}
		}
		for (const StaticStyle& style : styles) {
			if (style.text_size.exist()) {
				merged_style.text_size = style.text_size;
				break;
			}
		}
		for (const StaticStyle& style : styles) {
			if (style.color.exist()) {
				merged_style.color = style.color;
				break;
			}
		}
		for (const StaticStyle& style : styles) {
			if (style.displacement.exist()) {
				merged_style.displacement = style.displacement;
				break;
			}
		}
		for (const StaticStyle& style : styles) {
			if (style.rotation_euler.exist()) {
				merged_style.rotation_euler = style.rotation_euler;
				break;
			}
		}
		for (const StaticStyle& style : styles) {
			if (style.corner_rounding.exist()) {
				merged_style.corner_rounding = style.corner_rounding;
				break;
			}
		}
		for (const StaticStyle& style : styles) {
			if (style.padding.exist()) {
				merged_style.padding = style.padding;
				break;
			}
		}
		for (const StaticStyle& style : styles) {
			if (style.margin.exist()) {
				merged_style.margin = style.margin;
				break;
			}
		}
		for (const StaticStyle& style : styles) {
			if (style.border_thickness.exist()) {
				merged_style.border_thickness = style.border_thickness;
				break;
			}
		}
		for (const StaticStyle& style : styles) {
			if (style.border_color.exist()) {
				merged_style.border_color = style.border_color;
				break;
			}
		}
		for (const StaticStyle& style : styles) {
			if (style.cursor_type) {
				merged_style.cursor_type = style.cursor_type.value();
				break;
			}
		}
		for (const StaticStyle& style : styles) {
			if (style.stacking_type) {
				merged_style.stacking_type = style.stacking_type.value();
				break;
			}
		}
		for (const StaticStyle& style : styles) {
			if (style.text_allign_type) {
				merged_style.text_allign_type = style.text_allign_type.value();
				break;
			}
		}

		return merged_style;
	}

	Style merge_static_style_with_style(const StaticStyle& static_style, const Style& style, const _widget_info& info) {
		StaticStyle merged_style = merge_styles_by_priority({ static_style, style }, info);
		Style style_copy = style;
		
		style_copy.text_color = merged_style.text_color;
		style_copy.text_size = merged_style.text_size;
		style_copy.color = merged_style.color;
		style_copy.displacement = merged_style.displacement;
		style_copy.rotation_euler = merged_style.rotation_euler;
		style_copy.corner_rounding = merged_style.corner_rounding;
		style_copy.padding = merged_style.padding;
		style_copy.margin = merged_style.margin;
		style_copy.border_thickness = merged_style.border_thickness;
		style_copy.border_color = merged_style.border_color;
		style_copy.cursor_type = merged_style.cursor_type;
		style_copy.stacking_type = merged_style.stacking_type;
		style_copy.text_allign_type = merged_style.text_allign_type;

		return style_copy;
	}

	Style merge_static_style_with_style(const StaticStyle& static_style, const Style& override_style, const Style& style, const _widget_info& info) {
		StaticStyle merged_style = merge_styles_by_priority({ static_style, override_style, style }, info);
		Style style_copy;
		
		if (override_style.text_color_change.has_value()		)	style_copy.text_color_change		= override_style.text_color_change.value();
		else if(style.text_color_change.has_value()				)	style_copy.text_color_change		= style.text_color_change.value();
		if (override_style.text_size_change.has_value()			)	style_copy.text_size_change			= override_style.text_size_change.value();
		else if(style.text_size_change.has_value()				)	style_copy.text_size_change			= style.text_size_change.value();
		if (override_style.color_change.has_value()				)	style_copy.color_change				= override_style.color_change.value();
		else if(style.color_change.has_value()					)	style_copy.color_change				= style.color_change.value();
		if (override_style.displacement_change.has_value()		)	style_copy.displacement_change		= override_style.displacement_change.value();
		else if(style.displacement_change.has_value()			)	style_copy.displacement_change		= style.displacement_change.value();
		if (override_style.rotation_change.has_value()			)	style_copy.rotation_change			= override_style.rotation_change.value();
		else if(style.rotation_change.has_value()				)	style_copy.rotation_change			= style.rotation_change.value();
		if (override_style.corner_rounding_change.has_value()	)	style_copy.corner_rounding_change	= override_style.corner_rounding_change.value();
		else if(style.corner_rounding_change.has_value()		)	style_copy.corner_rounding_change	= style.corner_rounding_change.value();
		if (override_style.padding_change.has_value()			)	style_copy.padding_change			= override_style.padding_change.value();
		else if(style.padding_change.has_value()				)	style_copy.padding_change			= style.padding_change.value();
		if (override_style.margin_change.has_value()			)	style_copy.margin_change			= override_style.margin_change.value();
		else if(style.margin_change.has_value()					)	style_copy.margin_change			= style.margin_change.value();
		if (override_style.border_thickness_change.has_value()	)	style_copy.border_thickness_change	= override_style.border_thickness_change.value();
		else if(style.border_thickness_change.has_value()		)	style_copy.border_thickness_change	= style.border_thickness_change.value();
		if (override_style.border_color_change.has_value()		)	style_copy.border_color_change		= override_style.border_color_change.value();
		else if(style.border_color_change.has_value()			)	style_copy.border_color_change		= style.border_color_change.value();
		
		if (override_style.text_color_interpolation.has_value()			)	style_copy.text_color_interpolation = override_style.text_color_interpolation.value();
		else if(style.text_color_interpolation.has_value()				)	style_copy.text_color_interpolation = style.text_color_interpolation.value();
		if (override_style.text_size_interpolation.has_value()			)	style_copy.text_size_interpolation = override_style.text_size_interpolation.value();
		else if(style.text_size_interpolation.has_value()				)	style_copy.text_size_interpolation = style.text_size_interpolation.value();
		if (override_style.color_interpolation.has_value()				)	style_copy.color_interpolation = override_style.color_interpolation.value();
		else if(style.color_interpolation.has_value()					)	style_copy.color_interpolation = style.color_interpolation.value();
		if (override_style.displacement_interpolation.has_value()		)	style_copy.displacement_interpolation = override_style.displacement_interpolation.value();
		else if(style.displacement_interpolation.has_value()			)	style_copy.displacement_interpolation = style.displacement_interpolation.value();
		if (override_style.rotation_interpolation.has_value()			)	style_copy.rotation_interpolation = override_style.rotation_interpolation.value();
		else if(style.rotation_interpolation.has_value()				)	style_copy.rotation_interpolation = style.rotation_interpolation.value();
		if (override_style.corner_rounding_interpolation.has_value()	)	style_copy.corner_rounding_interpolation = override_style.corner_rounding_interpolation.value();
		else if(style.corner_rounding_interpolation.has_value()			)	style_copy.corner_rounding_interpolation = style.corner_rounding_interpolation.value();
		if (override_style.padding_interpolation.has_value()			)	style_copy.padding_interpolation = override_style.padding_interpolation.value();
		else if(style.padding_interpolation.has_value()					)	style_copy.padding_interpolation = style.padding_interpolation.value();
		if (override_style.margin_interpolation.has_value()				)	style_copy.margin_interpolation = override_style.margin_interpolation.value();
		else if(style.margin_interpolation.has_value()					)	style_copy.margin_interpolation = style.margin_interpolation.value();
		if (override_style.border_thickness_interpolation.has_value()	)	style_copy.border_thickness_interpolation = override_style.border_thickness_interpolation.value();
		else if(style.border_thickness_interpolation.has_value()		)	style_copy.border_thickness_interpolation = style.border_thickness_interpolation.value();
		if (override_style.border_color_interpolation.has_value()		)	style_copy.border_color_interpolation = override_style.border_color_interpolation.value();
		else if(style.border_color_interpolation.has_value()			)	style_copy.border_color_interpolation = style.border_color_interpolation.value();
		
		style_copy.on_hover  = merge_styles_by_priority({ override_style.on_hover, style.on_hover }, info);
		style_copy.on_active = merge_styles_by_priority({ override_style.on_active, style.on_active }, info);

		style_copy.text_color = merged_style.text_color;
		style_copy.text_size = merged_style.text_size;
		style_copy.color = merged_style.color;
		style_copy.displacement = merged_style.displacement;
		style_copy.rotation_euler = merged_style.rotation_euler;
		style_copy.corner_rounding = merged_style.corner_rounding;
		style_copy.padding = merged_style.padding;
		style_copy.margin = merged_style.margin;
		style_copy.border_thickness = merged_style.border_thickness;
		style_copy.border_color = merged_style.border_color;
		style_copy.cursor_type = merged_style.cursor_type;
		style_copy.stacking_type = merged_style.stacking_type;
		style_copy.text_allign_type = merged_style.text_allign_type;

		return style_copy;
	}

	StaticStyle get_final_style(const Style& override_style, const Style& style, _widget_info& info) {
		StaticStyle default_style = merge_styles_by_priority({ override_style, style }, info);
		StaticStyle hover_style = merge_styles_by_priority({ override_style.on_hover, style.on_hover, default_style }, info);

		Style base_style = merge_static_style_with_style(default_style, override_style, style, info);
		Style target_style = merge_static_style_with_style(hover_style, override_style, style, info);	// append timing information of style to style_to_use, TODO: merge timing information of _style and overwrite_style here
		StaticStyle interpolated_style = interpolate_styles(base_style, target_style, info, info.is_hovering);

		return interpolated_style;

	}
}

void StaticStyle::clear() {
	text_color = StyleAttribute<vec3f>();
	text_size = StyleAttribute<float>();
	color = StyleAttribute<vec3f>();
	displacement = StyleAttribute<vec2f>();
	rotation_euler = StyleAttribute<vec2f>();
	corner_rounding = StyleAttribute<vec4f>();
	padding = StyleAttribute<vec4f>();
	margin = StyleAttribute<vec4f>();
	border_thickness = StyleAttribute<vec4f>();
	border_color = StyleAttribute<vec3f>();
	cursor_type = std::optional<Frame::CursorType>();
	stacking_type = std::optional<Style::Stacking>();
	text_allign_type = std::optional<Style::TextAlligning>();
}


void Style::clear() {
	StaticStyle::clear();

	text_color_change = std::optional<Time>();
	text_size_change = std::optional<Time>();
	color_change = std::optional<Time>();
	displacement_change = std::optional<Time>();
	rotation_change = std::optional<Time>();
	corner_rounding_change = std::optional<Time>();
	padding_change = std::optional<Time>();
	margin_change = std::optional<Time>();
	border_thickness_change = std::optional<Time>();
	border_color_change = std::optional<Time>();

	text_color_interpolation = std::optional<_interpolation>();
	text_size_interpolation = std::optional<_interpolation>();
	color_interpolation = std::optional<_interpolation>();
	displacement_interpolation = std::optional<_interpolation>();
	rotation_interpolation = std::optional<_interpolation>();
	corner_rounding_interpolation = std::optional<_interpolation>();
	padding_interpolation = std::optional<_interpolation>();
	margin_interpolation = std::optional<_interpolation>();
	border_thickness_interpolation = std::optional<_interpolation>();
	border_color_interpolation = std::optional<_interpolation>();

	on_hover.clear();
	on_active.clear();
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
	case SIZE_X:
		return value * info.size.x;
		break;
	case SIZE_Y:
		return value * info.size.y;
		break;
	case PADDED_SIZE_X:
		return 0;
		break;
	case PADDED_SIZE_Y:
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
	case POSITION_X:
		return value * info.position.x;
		break;
	case POSITION_Y:
		return value * info.position.y;
		break;
	case PADDED_POSITION_X:
		return 0;
		break;
	case PADDED_POSITION_Y:
		return 0;
		break;
	case PARENT_WIDTH:
		return 0;
		break;
	case PARENT_HEIGHT:
		return 0;
		break;
	case AVAILABLE_WIDTH:
		return 0;
		break;
	case AVAILABLE_HEIGHT:
		return 0;
		break;
	}
	return 0;
}

// object based system

void _widget_info::increment_time(Time deltatime) {
	//last_update = now;
	if (is_hovering) {
		_current_text_color_time += deltatime;
		_current_text_size_time += deltatime;
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
		_current_text_color_time -= deltatime;
		_current_text_size_time -= deltatime;
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


// new implementation of gui
std::shared_ptr<Program> Gui::gui_program;
std::shared_ptr<Font> Gui::font;

Gui::Gui(Frame& frame) : 
	frame_ref(frame)
{
	gui_program = default_program::gui_program_s();
	font = std::make_shared<Font>("..\\GraphicsCortex\\Fonts\\Roboto-Regular.ttf", 16);
}

void Gui::new_frame(Time frame_time) {
	this->frame_time = frame_time;

	if (hoverings.size() > 0){
		unsigned int hover_id = hoverings[0].first;
		float hover_z = hoverings[0].second;
		int i = 0;
		int hover_index = 0;
		for (std::pair<unsigned int, float> candidate : hoverings) {
			if (candidate.second > hover_z) {
				hover_id = candidate.first;
				hover_z = candidate.second;
				hover_index = i;
			}
			i++;
		}

		widget_info_table[hover_id].is_active_hovering = true;
		frame_ref.set_cursor_type(hoverings_cursortypes[hover_index]);
	}
	else {
		frame_ref.set_cursor_type(Frame::Arrow);
	}

	hoverings.clear();
	hoverings_cursortypes.clear();

	z_index = 0;
	camera.perspective = false;
	camera.screen_width = frame_ref.window_width;
	camera.screen_height = frame_ref.window_height;
	camera.projection_matrix = glm::ortho(0.0f, (float)frame_ref.window_width, 0.0f, (float)frame_ref.window_height, -100.0f, 100.0f);
}

_widget_info& Gui::box(unsigned int id, vec2 position, vec2 size, Style style, std::u32string text_string, Style override_style, float z_index) {
	AABB2 aabb(position, size);
	if (widget_graphic_table.find(id) == widget_graphic_table.end()) {
		widget_graphic_table[id] = std::make_shared<Graphic>();
		widget_graphic_table[id]->set_mesh(std::make_shared<Mesh>());
		widget_graphic_table[id]->material->set_program(Gui::gui_program);
	}

	std::shared_ptr<Graphic>& graphic = widget_graphic_table[id];

	graphic->mesh->load_model(aabb.generate_model());

	if (!widget_info_table[id].properly_initialized) {
		_widget_info info;
		info.size = size;
		info.position = position;
		info.properly_initialized = true;
		widget_info_table[id] = info;
	}
	_widget_info& info = widget_info_table[id];
	info.increment_time(frame_time / 1000);
	
	StaticStyle interpolated_style = get_final_style(override_style, style, info);

	Vec3<float> text_color =		style_attribute_get<Vec3<float>>(interpolated_style.text_color,			info);
	float text_size =				style_attribute_get<float>(		 interpolated_style.text_size,			info);
	Vec3<float> color =				style_attribute_get<Vec3<float>>(interpolated_style.color,				info);
	Vec2<float> displacement =		style_attribute_get<Vec2<float>>(interpolated_style.displacement,		info);
	Vec2<float> rotation_euler =	style_attribute_get<Vec2<float>>(interpolated_style.rotation_euler,		info);
	Vec4<float> corner_rounding =	style_attribute_get<Vec4<float>>(interpolated_style.corner_rounding,	info);
	Vec4<float> padding =			style_attribute_get<Vec4<float>>(interpolated_style.padding,			info);
	Vec4<float> margin =			style_attribute_get<Vec4<float>>(interpolated_style.margin,				info);
	Vec4<float> border_thickness =	style_attribute_get<Vec4<float>>(interpolated_style.border_thickness,	info);
	Vec3<float> border_color =		style_attribute_get<Vec3<float>>(interpolated_style.border_color,		info);
	Frame::CursorType cursor_type = optional_get<Frame::CursorType>(interpolated_style.cursor_type);
	Style::TextAlligning text_allign_type = optional_get<Style::TextAlligning>(interpolated_style.text_allign_type);

	bool hover = aabb.does_contain(frame_ref.get_cursor_position());
	if (hover) hoverings.push_back(std::pair(id, z_index));
	if (hover) hoverings_cursortypes.push_back(cursor_type);

	info.is_active_hovering = false;
	info.is_hovering = hover;

	gui_program->update_uniform("screen_position", aabb.position.x, frame_ref.window_height - aabb.position.y);
	gui_program->update_uniform("rect_color", color.x, color.y, color.z, 1.0f);
	gui_program->update_uniform("rect_size", aabb.size.x, aabb.size.y);
	gui_program->update_uniform("corner_rounding", corner_rounding.x, corner_rounding.y, corner_rounding.z, corner_rounding.w);
	gui_program->update_uniform("border_color", border_color.x, border_color.y, border_color.z, 1.0f);
	gui_program->update_uniform("border_thickness", border_thickness.x, border_thickness.y, border_thickness.z, border_thickness.w);
	gui_program->update_uniform("z_index", z_index);

	camera.update_default_uniforms(*Gui::gui_program);
	graphic->update_default_uniforms(*Gui::gui_program);
	graphic->draw(false);

	if (text_string != U"") {
		if (widget_text_table.find(id) == widget_text_table.end()) {
			widget_text_table[id] = std::make_shared<Text>(font);
			widget_text_table[id]->set_text(text_string);
		}
		std::shared_ptr<Text>& text = widget_text_table[id];
		
		if (text->get_text32() != text_string) text->set_text(text_string);

		text->set_max_width(camera.screen_width);
		text->set_scale(0.02 * text_size);
		vec2 text_dimentions = text->get_size();
		if (text_allign_type == Style::Default)
			text->graphic->position = glm::vec3((aabb.position.x) / camera.screen_width, (frame_ref.window_height - text_size) / camera.screen_width, z_index);
		if (text_allign_type == Style::CenterX)
			text->graphic->position = glm::vec3((aabb.position.x + size.x / 2) / camera.screen_width - text_dimentions.x / 2, (frame_ref.window_height - aabb.position.y - text_size) / camera.screen_width, z_index);
		if (text_allign_type == Style::CenterY)
			text->graphic->position = glm::vec3((aabb.position.x) / camera.screen_width, (frame_ref.window_height - aabb.position.y - size.y / 2 - text_size / 2) / camera.screen_width, z_index);
		if (text_allign_type == Style::CenterXY)
			text->graphic->position = glm::vec3((aabb.position.x + size.x / 2) / camera.screen_width - text_dimentions.x / 2, (frame_ref.window_height - aabb.position.y - size.y / 2 - text_size / 2) / camera.screen_width, z_index);

		
		text->set_color(vec4(text_color.x, text_color.y, text_color.z, 1.0f));

		camera.projection_matrix = glm::ortho(0.0f, 1.0f, 0.0f, (float)frame_ref.window_height / frame_ref.window_width, -100.0f, 100.0f);
		text->update_default_uniforms(*text->graphic->material->program);
		camera.update_default_uniforms(*text->graphic->material->program);

		text->render();
		camera.projection_matrix = glm::ortho(0.0f, (float)frame_ref.window_width, 0.0f, (float)frame_ref.window_height, -100.0f, 100.0f);

	}

	return info;
}

_widget_info& Gui::box(const std::string& name, vec2 position, vec2 size, Style style, std::u32string text) {
	if (name_id_table.find(name) == name_id_table.end())
		name_id_table[name] = name_id_table.size();
	unsigned int id = name_id_table[name];

	return box(id, position, size, style, text, override_style, z_index);
}

vec4f Gui::get_margin_by_id(unsigned int id, const Style& override_style, const Style& style) {
	if (widget_info_table[id].properly_initialized) {
		_widget_info& info = widget_info_table[id];
		StaticStyle interpolated_style = get_final_style(override_style, style, info);
		vec4f margin = style_attribute_get<vec4f>(interpolated_style.margin, info);
		return margin;
	}
	return vec4(0, 0, 0, 0);
}

vec4f Gui::get_padding_by_id(unsigned int id, const Style& override_style, const Style& style) {
	if (widget_info_table[id].properly_initialized) {
		_widget_info& info = widget_info_table[id];
		StaticStyle interpolated_style = get_final_style(override_style, style, info);
		vec4f padding = style_attribute_get<vec4f>(interpolated_style.padding, info);
		return padding;
	}
	return vec4(0, 0, 0, 0);
}

Style::Stacking Gui::get_stacking_type_by_id(unsigned int id, const Style& override_style, const Style& style) {
	if (widget_info_table[id].properly_initialized) {
		_widget_info& info = widget_info_table[id];
		StaticStyle interpolated_style = get_final_style(override_style, style, info);
		Style::Stacking stacking_type = optional_get<Style::Stacking>(interpolated_style.stacking_type);
		return stacking_type;
	}
	return Style::Stack;
}

namespace std{
	vec2f max(const vec2f& vec_a, const vec2f& vec_b) {
		return vec2f(std::max(vec_a.x, vec_b.x), std::max(vec_a.y, vec_b.y));
	}
}


std::vector<std::shared_ptr<Gui::layout_node>> Gui::get_layouts_in_descending_order() {

	std::vector<std::shared_ptr<layout_node>> layout_nodes;
	std::vector<std::shared_ptr<layout_node>> temp_stack;

	// list layout nodes of tree in decending order
	temp_stack.push_back(current_layout);
	while (temp_stack.size() > 0) {
		std::shared_ptr<layout_node> node = temp_stack.back();
		temp_stack.pop_back();
		layout_nodes.push_back(node);
		for (std::shared_ptr<layout_node> child : node->childs)
			temp_stack.push_back(child);
	}

	return layout_nodes;
}

std::vector<std::shared_ptr<Gui::layout_node>> Gui::get_layouts_in_ascending_order() {

	std::vector<std::shared_ptr<layout_node>> layout_nodes = get_layouts_in_descending_order();

	std::vector<std::shared_ptr<layout_node>> layout_nodes_reversed;
	layout_nodes_reversed.reserve(layout_nodes.size());
	for (int i = layout_nodes.size() - 1; i >= 0; i--) {
		layout_nodes_reversed.push_back(layout_nodes[i]);
	}

	return layout_nodes_reversed;
}

_widget_info& Gui::layout(const std::string& name, vec2 position, vec2 min_size, Style style, Layout::LayoutType layout_type) {
	
	if (name_id_table.find(name) == name_id_table.end())
		name_id_table[name] = name_id_table.size();
	unsigned int id = name_id_table[name];

	if (current_layout != nullptr) {
		std::cout << "[GUI Error] another Gui::layout() is called without calling Gui::layout_end() for previous Gui::layout() call" << std::endl;
		return widget_info_table[id];
	}

	z_index++;
	
	this->position = position;
	current_layout = std::make_shared<layout_node>(id, min_size, style, override_style, layout_type, z_index);

	if (layout_stack.size() > 0) {
		std::cout << "[GUI Error] Gui::layout() is called but layout_stack is not empty " << std::endl;
	}
	layout_stack.clear();
	layout_stack.push_back(current_layout);

	return widget_info_table[id];
}

_widget_info& Gui::content(const std::string& name, vec2 size, Style style, std::u32string text) {
	if (name_id_table.find(name) == name_id_table.end())
		name_id_table[name] = name_id_table.size();
	unsigned int id = name_id_table[name];

	if (std::shared_ptr<layout_node> node = layout_stack.back().lock()) {
		content_info content(id, size, style, override_style, text, z_index + 1);
		node->contents.push_back(content);
		node->child_type_order.push_back(layout_node::content);
	}
	else {
		std::cout << "[GUI Error] Gui::content() is called but no Gui::layout() or Gui::layout_content() was called before" << std::endl;
		return widget_info_table[id];
	}

	return widget_info_table[id];
}

_widget_info& Gui::layout_content(const std::string& name, vec2 min_size, Style style, Layout::LayoutType layout_type) {
	if (name_id_table.find(name) == name_id_table.end())
		name_id_table[name] = name_id_table.size();
	unsigned int id = name_id_table[name];

	if (std::shared_ptr<layout_node> node = layout_stack.back().lock()) {
		
		z_index++;

		std::shared_ptr<layout_node> new_layout = std::make_shared<layout_node>(id, min_size, style, override_style, layout_type, z_index);
		node->childs.push_back(new_layout);
		node->child_type_order.push_back(layout_node::layout);
		layout_stack.push_back(new_layout);
	}
	else {
		std::cout << "[GUI Error] Gui::layout_content() is called but no Gui::layout() or Gui::layout_content() was called before" << std::endl;
		return widget_info_table[id];
	}

	return widget_info_table[id];
}

void Gui::layout_content_end() {
	if (std::shared_ptr<layout_node> node = layout_stack.back().lock()){

		z_index--;

		layout_stack.pop_back();
	}
	else {
		std::cout << "[GUI Error] Gui::layout_content_end() is called but no Gui::layout() or Gui::layout_content() was called before" << std::endl;
		return;
	}
}

void Gui::layout_end() {
	if (current_layout == nullptr) {
		std::cout << "[GUI Error] Gui::layout_end() is called without corresponding Gui::layout()" << std::endl;
		return;
	}

	//bool center_widgets = true;
	//bool inverted_direction = false;

	z_index--;

	// compute all sizes
	{
		std::vector<std::shared_ptr<layout_node>> layout_nodes_a = get_layouts_in_ascending_order();
		for (std::shared_ptr<layout_node> layout : layout_nodes_a) {

			layout->self_info.layout.window_size = 0;
			layout->self_info.layout.widget_positions.clear();

			int content_counter = 0;
			int layout_counter = 0;
			for (int i = 0; i < layout->child_type_order.size(); i++) {
				if (layout->child_type_order[i] == layout_node::content) {
					content_info& content = layout->contents[content_counter];
					
					vec4f margin = get_margin_by_id(content.id, content.override_style, content.style);
					vec4 padding = get_padding_by_id(content.id, content.override_style, content.style);
					vec2 size = content.size + vec2(margin.y + margin.w, margin.x + margin.z);
					layout->self_info.layout.add_widget(size);
					
					content_counter++;
				}
				if (layout->child_type_order[i] == layout_node::layout) {
					std::shared_ptr<layout_node> child = layout->childs[layout_counter];
		
					vec4 margin = get_margin_by_id(child->self_info.id, child->self_info.override_style, child->self_info.style);
					vec4 padding = get_padding_by_id(child->self_info.id, child->self_info.override_style, child->self_info.style);
					vec2f size = child->self_info.layout.window_size + vec2(margin.y + margin.w, margin.x + margin.z);
					layout->self_info.layout.add_widget(size);
					
					layout_counter++;
				}
			}
			vec4 parent_padding = get_padding_by_id(layout->self_info.id, layout->self_info.override_style, layout->self_info.style);
			vec4 parent_margin = get_padding_by_id(layout->self_info.id, layout->self_info.override_style, layout->self_info.style);
			layout->self_info.layout.window_size = layout->self_info.layout.window_size + vec2(parent_padding.y + parent_padding.w, parent_padding.x + parent_padding.z);
			layout->self_info.layout.window_size = std::max(layout->self_info.layout.window_size, layout->self_info.min_size + vec2(parent_padding.y + parent_padding.w, parent_padding.x + parent_padding.z));
		}
	}

	// compute layout positions
	{
		std::vector<std::shared_ptr<layout_node>> temp_stack;

		current_layout->self_info.layout.position = position;
		temp_stack.push_back(current_layout);

		while (temp_stack.size() > 0) {
			std::shared_ptr<layout_node> node = temp_stack.back();
			// reset the current node's layout size
			std::shared_ptr<layout_node> current_node = temp_stack.back();
			temp_stack.pop_back();

			// recount layout size and compute positions of child layouts
			int content_counter = 0;
			int layout_counter = 0;
			for (int i = 0; i < node->child_type_order.size(); i++) {
				layout_node::child_type type = node->child_type_order[i];
				if (type == layout_node::content) {
					content_info& content = node->contents[content_counter];
					
					vec2& self_position = node->self_info.layout.widget_positions[content_counter + layout_counter];
					vec2 self_size = content.size;
					vec2 parent_position = node->self_info.layout.position;

					vec4f margin = get_margin_by_id(content.id, content.override_style, content.style);
					vec4 padding = get_padding_by_id(content.id, content.override_style, content.style);
					vec4 parent_padding = get_padding_by_id(node->self_info.id, node->self_info.override_style, node->self_info.style);

					Style::Stacking parent_stacking_type = get_stacking_type_by_id(node->self_info.id, node->self_info.override_style, node->self_info.style);

					if (parent_stacking_type == Style::Center || parent_stacking_type == Style::Inverse) {
						for (int i = 0; i < ((parent_stacking_type == Style::Inverse) ? 2 : 1); i++)
							self_position = node->self_info.layout.get_centered_widget_position(content_counter + layout_counter, self_size);
						if (node->self_info.layout_type == Layout::Vertical)
							self_position = parent_position + self_position + vec2(margin.y, margin.x) + vec2(0, parent_padding.x);
						if (node->self_info.layout_type == Layout::Horizional)
							self_position = parent_position + self_position + vec2(margin.y, margin.x) + vec2(parent_padding.y, 0);
					}
					else
						self_position = parent_position + self_position + vec2(margin.y, margin.x) + vec2(parent_padding.y, parent_padding.x);
					
					
					content_counter++;
				}
				else if (type == layout_node::layout) {

					layout_info& layout = node->childs[layout_counter]->self_info;

					vec2 self_size = layout.layout.window_size;
					vec2& self_position_of_list = node->self_info.layout.widget_positions[content_counter + layout_counter];
					vec2& self_position_of_layout = node->childs[layout_counter]->self_info.layout.position;

					vec2 parent_position = node->self_info.layout.position;
					
					vec4 margin = get_margin_by_id(layout.id, layout.override_style, layout.style);
					vec4 padding = get_padding_by_id(layout.id, layout.override_style, layout.style);
					vec4 parent_padding = get_padding_by_id(node->self_info.id, node->self_info.override_style, node->self_info.style);

					Style::Stacking parent_stacking_type = get_stacking_type_by_id(node->self_info.id, node->self_info.override_style, node->self_info.style);

					if (parent_stacking_type == Style::Center || parent_stacking_type == Style::Inverse) {
						for (int i = 0; i < ((parent_stacking_type == Style::Inverse) ? 2 : 1); i++)
							self_position_of_list = node->self_info.layout.get_centered_widget_position(content_counter + layout_counter, self_size);
						if (node->self_info.layout_type == Layout::Vertical)
							self_position_of_list = parent_position + self_position_of_list + vec2(margin.y, margin.x) + vec2(0, parent_padding.x);
						if (node->self_info.layout_type == Layout::Horizional)
							self_position_of_list = parent_position + self_position_of_list + vec2(margin.y, margin.x) + vec2(parent_padding.y, 0);
					}
					else
						self_position_of_list = parent_position + self_position_of_list + vec2(margin.y, margin.x) + vec2(parent_padding.y, parent_padding.x);
					
					self_position_of_layout = self_position_of_list;

					temp_stack.push_back(node->childs[layout_counter]);
					layout_counter++;
				}
			}
		}
	}

	// draw root
	box(current_layout->self_info.id, position, current_layout->self_info.layout.window_size, current_layout->self_info.style, U"", current_layout->self_info.override_style, current_layout->self_info.z_index);

	// draw layouts and their children from leaves to root of the tree
	std::vector<std::shared_ptr<layout_node>> layout_nodes_d = get_layouts_in_descending_order();
	for (std::shared_ptr<layout_node> node : layout_nodes_d) {
		int content_counter = 0;
		int layout_counter = 0;
		for (int i = 0; i < node->child_type_order.size(); i++) {
			layout_node::child_type type = node->child_type_order[i];
			if (type == layout_node::content) {
				content_info& content = node->contents[content_counter];

				vec2 size = content.size;
				vec2 position = node->self_info.layout.widget_positions[content_counter + layout_counter];

				box(content.id, position, size, content.style, content.text, content.override_style, content.z_index);
				
				content_counter++;
			}
			else if (type == layout_node::layout) {
				layout_info& layout = node->childs[layout_counter]->self_info;
				
				vec4 margin = get_margin_by_id(layout.id, layout.override_style, layout.style);

				vec2f size = layout.layout.window_size;
				vec2 position = node->self_info.layout.widget_positions[content_counter + layout_counter];

				box(layout.id, position, size, layout.style, U"", layout.override_style, layout.z_index);

				layout_counter++;
			}
		}
	}

	current_layout = nullptr;
	layout_stack.clear();
}
