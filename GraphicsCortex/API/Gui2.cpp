#include "Gui.h"
#include "Default_Programs.h"


// Style system
namespace {
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
		float text_size = style_attribute_get<float>({ target_style.text_size,			style.text_size }, widget_info, 12.0f);
		Vec3<float> color = style_attribute_get<Vec3<float>>({ target_style.color,				style.color }, widget_info);
		Vec2<float> displacement = style_attribute_get<Vec2<float>>({ target_style.displacement,		style.displacement }, widget_info);
		Vec2<float> rotation_euler = style_attribute_get<Vec2<float>>({ target_style.rotation_euler,	style.rotation_euler }, widget_info);
		Vec4<float> corner_rounding = style_attribute_get<Vec4<float>>({ target_style.corner_rounding,	style.corner_rounding }, widget_info);
		Vec4<float> padding = style_attribute_get<Vec4<float>>({ target_style.padding,			style.padding }, widget_info);
		Vec4<float> margin = style_attribute_get<Vec4<float>>({ target_style.margin,			style.margin }, widget_info);
		Vec4<float> border_thickness = style_attribute_get<Vec4<float>>({ target_style.border_thickness,	style.border_thickness }, widget_info);
		Vec3<float> border_color = style_attribute_get<Vec3<float>>({ target_style.border_color,		style.border_color }, widget_info);
		Frame::CursorType cursor_type = optional_get<Frame::CursorType>({ target_style.cursor_type,		style.cursor_type });

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

		return merged_style;
	}

	Style merge_static_style_with_style(const StaticStyle& static_style, const Style& style, const _widget_info& info) {
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
		
		StaticStyle on_hover  = merge_styles_by_priority({ override_style.on_hover, style.on_hover }, info);
		StaticStyle on_active = merge_styles_by_priority({ override_style.on_hover, style.on_hover }, info);

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

	StaticStyle get_final_style(const Style& override_style, const Style& style, _widget_info& info) {
		StaticStyle default_style = merge_styles_by_priority({ override_style, style }, info);
		StaticStyle hover_style = merge_styles_by_priority({ override_style.on_hover, style.on_hover, default_style }, info);

		Style base_style = merge_static_style_with_style(default_style, override_style, style, info);
		Style target_style = merge_static_style_with_style(hover_style, override_style, style, info);	// append timing information of style to style_to_use, TODO: merge timing information of _style and overwrite_style here
		StaticStyle interpolated_style = interpolate_styles(base_style, target_style, info, info.is_hovering);

		return interpolated_style;

	}
}


// new implementation of gui
std::shared_ptr<Program> Gui2::gui_program;
std::shared_ptr<Font> Gui2::font;

Gui2::Gui2(Frame& frame) : 
	frame_ref(frame)
{
	gui_program = default_program::gui_program_s().obj;
	font = std::make_shared<Font>("Fonts\\Roboto-Regular.ttf", 16);
}

void Gui2::new_frame(Time frame_time) {
	this->frame_time = frame_time;
	
	if (hoverings.size() > 0){
		unsigned int hover_id = hoverings[0].first;
		float hover_z = hoverings[0].second;
		for (std::pair<unsigned int, float> candidate : hoverings) {
			if (candidate.second > hover_z) {
				hover_id = candidate.first;
				hover_z = candidate.second;
			}
		}

		widget_info_table[hover_id].is_active_hovering = true;
	}

	z_index = 0;
	camera.perspective = false;
	camera.screen_width = frame_ref.window_width;
	camera.screen_height = frame_ref.window_height;
	camera.projection_matrix = glm::ortho(0.0f, (float)frame_ref.window_width, 0.0f, (float)frame_ref.window_height, -100.0f, 100.0f);
}

void Gui2::box(unsigned int id, vec2 position, vec2 size, Style style, std::u32string text_string, Style override_style, float z_index) {
	
	AABB2 aabb(position, size);
	if (widget_graphic_table.find(id) == widget_graphic_table.end()) {
		widget_graphic_table[id] = std::make_unique<Graphic>();
		widget_graphic_table[id]->load_model(Mesh_s());
		widget_graphic_table[id]->load_program(Gui2::gui_program);
	}

	std::unique_ptr<Graphic>& graphic = widget_graphic_table[id];

	graphic->mesh->load_model(aabb.generate_model());

	if (widget_info_table.find(id) == widget_info_table.end()) {
		_widget_info info;
		info.size = size;
		info.position = position;
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

	bool hover = aabb.does_contain(frame_ref.get_cursor_position());
	if (hover) hoverings.push_back(std::pair(id, z_index));

	info.is_active_hovering = false;
	info.is_hovering = hover;

	gui_program->update_uniform("screen_position", aabb.position.x, frame_ref.window_height - aabb.position.y);
	gui_program->update_uniform("rect_color", color.x, color.y, color.z, 1.0f);
	gui_program->update_uniform("rect_size", aabb.size.x, aabb.size.y);
	gui_program->update_uniform("corner_rounding", corner_rounding.x, corner_rounding.y, corner_rounding.z, corner_rounding.w);
	gui_program->update_uniform("border_color", border_color.x, border_color.y, border_color.z, 1.0f);
	gui_program->update_uniform("border_thickness", border_thickness.x, border_thickness.y, border_thickness.z, border_thickness.w);
	gui_program->update_uniform("z_index", z_index);

	camera.update_default_uniforms(*Gui2::gui_program);
	graphic->update_default_uniforms(*Gui2::gui_program);
	graphic->draw(false);

	if (text_string != U"") {
		if (widget_text_table.find(id) == widget_text_table.end()) {
			widget_text_table[id] = std::make_unique<Text>(font);
			widget_text_table[id]->set_text(text_string);
		}
		std::unique_ptr<Text>& text = widget_text_table[id];
		
		if (text->get_text32() != text_string) text->set_text(text_string);

		text->set_max_width(camera.screen_width);
		text->set_scale(0.02 * text_size);
		float text_height = (font->glyphs[u'l'].y1 - font->glyphs[u'l'].y0) * text->get_scale();
		text->graphic->set_position(glm::vec3((aabb.position.x) / camera.screen_width, (frame_ref.window_height - aabb.position.y - size.y / 2 - text_size / 2) / camera.screen_width, z_index));
		text->set_color(vec4(text_color.x, text_color.y, text_color.z, 1.0f));

		camera.projection_matrix = glm::ortho(0.0f, 1.0f, 0.0f, (float)frame_ref.window_height / frame_ref.window_width, -100.0f, 100.0f);
		text->update_default_uniforms(*text->graphic->renderer);
		camera.update_default_uniforms(*text->graphic->renderer);

		text->render();
		camera.projection_matrix = glm::ortho(0.0f, (float)frame_ref.window_width, 0.0f, (float)frame_ref.window_height, -100.0f, 100.0f);

	}

}

void Gui2::box(unsigned int id, vec2 position, vec2 size, Style style, std::u32string text) {
	box(id, position, size, style, text, override_style, z_index);
}

void Gui2::layout(unsigned int id, vec2 position, vec2 min_size, Style style, Layout::LayoutType layout_type) {
	if (current_layout != nullptr) {
		std::cout << "[GUI Error] another Gui::layout() is called without calling Gui::layout_end() for previous Gui::layout() call" << std::endl;
		return;
	}

	z_index++;
	
	this->position = position;
	current_layout = std::make_shared<layout_node>(id, min_size, style, override_style, layout_type, z_index);

	if (layout_stack.size() > 0) {
		std::cout << "[GUI Error] Gui::layout() is called but layout_stack is not empty " << std::endl;
	}
	layout_stack.clear();
	layout_stack.push_back(current_layout);
}

void Gui2::content(unsigned int id, vec2 size, Style style, std::u32string text) {
	if (std::shared_ptr<layout_node> node = layout_stack.back().lock()) {

		if (widget_info_table.find(id) != widget_info_table.end()) {
			_widget_info& info = widget_info_table[id];
			StaticStyle interpolated_style = get_final_style(override_style, style, info);
			vec4f margin = style_attribute_get<vec4f>(interpolated_style.margin, info);
			size = size + vec2(margin.y + margin.w, margin.x + margin.z);
		}

		content_info content(id, size, style, override_style, text, z_index + 1);
		node->contents.push_back(content);
		node->self_info.layout.add_widget(size);
		node->child_type_order.push_back(layout_node::content);
	}
	else {
		std::cout << "[GUI Error] Gui::content() is called but no Gui::layout() or Gui::layout_content() was called before" << std::endl;
		return;
	}
}

void Gui2::layout_content(unsigned int id, vec2 min_size, Style style, Layout::LayoutType layout_type) {
	if (std::shared_ptr<layout_node> node = layout_stack.back().lock()) {
		
		z_index++;

		std::shared_ptr<layout_node> new_layout = std::make_shared<layout_node>(id, min_size, style, override_style, layout_type, z_index);
		node->childs.push_back(new_layout);
		node->child_type_order.push_back(layout_node::layout);
		layout_stack.push_back(new_layout);
	}
	else {
		std::cout << "[GUI Error] Gui::layout_content() is called but no Gui::layout() or Gui::layout_content() was called before" << std::endl;
		return;
	}
}

void Gui2::layout_content_end() {
	if (std::shared_ptr<layout_node> node = layout_stack.back().lock()){

		z_index--;
		node->self_info.layout.window_size.x = std::max(node->self_info.layout.window_size.x, node->self_info.min_size.x);
		node->self_info.layout.window_size.y = std::max(node->self_info.layout.window_size.y, node->self_info.min_size.y);

		layout_stack.pop_back();
		if (std::shared_ptr<layout_node> parent = layout_stack.back().lock()) {
			
			vec2 size = node->self_info.layout.window_size;
			if (widget_info_table.find(node->self_info.id) != widget_info_table.end()) {
				_widget_info& info = widget_info_table[node->self_info.id];
				StaticStyle interpolated_style = get_final_style(node->self_info.override_style, node->self_info.style, info);
				vec4f margin = style_attribute_get<vec4f>(interpolated_style.margin, info);
				vec4f padding = style_attribute_get<vec4f>(interpolated_style.padding, info);
				size = size + vec2(margin.y + margin.w, margin.x + margin.z) + vec2(padding.y + padding.w, padding.x + padding.z);
			}
			
			parent->self_info.layout.add_widget(size);
		}
		else {
			std::cout << "[GUI Error] Gui::layout_content_end() is called but no Gui::layout() was called before" << std::endl;
			return;
		}
	}
	else {
		std::cout << "[GUI Error] Gui::layout_content_end() is called but no Gui::layout() or Gui::layout_content() was called before" << std::endl;
		return;
	}
}

std::vector<std::shared_ptr<Gui2::layout_node>> Gui2::get_layouts_in_descending_order() {

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

std::vector<std::shared_ptr<Gui2::layout_node>> Gui2::get_layouts_in_ascending_order() {

	std::vector<std::shared_ptr<layout_node>> layout_nodes = get_layouts_in_descending_order();

	std::vector<std::shared_ptr<layout_node>> layout_nodes_reversed;
	layout_nodes_reversed.reserve(layout_nodes.size());
	for (int i = layout_nodes.size() - 1; i >= 0; i--) {
		layout_nodes_reversed.push_back(layout_nodes[i]);
	}

	return layout_nodes_reversed;
}

void Gui2::layout_end() {
	if (current_layout == nullptr) {
		std::cout << "[GUI Error] Gui::layout_end() is called without corresponding Gui::layout()" << std::endl;
		return;
	}

	current_layout->self_info.layout.window_size.x = std::max(current_layout->self_info.layout.window_size.x, current_layout->self_info.min_size.x);
	current_layout->self_info.layout.window_size.y = std::max(current_layout->self_info.layout.window_size.y, current_layout->self_info.min_size.y);
	z_index--;
	
	// compute layout positions
	{
		std::vector<std::shared_ptr<layout_node>> temp_stack;

		vec2 global_padding;
		if (widget_info_table.find(current_layout->self_info.id) != widget_info_table.end()) {
			_widget_info& info = widget_info_table[current_layout->self_info.id];
			StaticStyle interpolated_style = get_final_style(current_layout->self_info.override_style, current_layout->self_info.style, info);
			vec4f margin = style_attribute_get<vec4f>(interpolated_style.margin, info);
			vec4f padding = style_attribute_get<vec4f>(interpolated_style.padding, info);
			global_padding = vec2(padding.y + padding.w, padding.x + padding.z);
		}
		current_layout->self_info.layout.position = position + global_padding;
		temp_stack.push_back(current_layout);

		while (temp_stack.size() > 0) {
			std::shared_ptr<layout_node> node = temp_stack.back();
			// reset the current node's layout size
			node->self_info.layout.window_size = vec2(0, 0);
			std::shared_ptr<layout_node> current_node = temp_stack.back();
			temp_stack.pop_back();

			// recount layout size and compute positions of child layouts
			int content_counter = 0;
			int layout_counter = 0;
			for (int i = 0; i < node->child_type_order.size(); i++) {
				layout_node::child_type type = node->child_type_order[i];
				if (type == layout_node::content) {
					content_info& content = node->contents[content_counter];
					node->self_info.layout.add_widget(node->contents[content_counter].size);
					content_counter++;
				}
				else if (type == layout_node::layout) {

					layout_info& layout = node->childs[layout_counter]->self_info;
					vec2 size = node->childs[layout_counter]->self_info.layout.window_size;
					vec2 position = node->self_info.layout.get_widget_position();
					if (widget_info_table.find(layout.id) != widget_info_table.end()) {
						_widget_info& info = widget_info_table[layout.id];
						StaticStyle interpolated_style = get_final_style(layout.override_style, layout.style, info);
						vec4f margin = style_attribute_get<vec4f>(interpolated_style.margin, info);
						vec4f padding = style_attribute_get<vec4f>(interpolated_style.padding, info);
						size = size + vec2(margin.y + margin.w, margin.x + margin.z) + vec2(padding.y + padding.w, padding.x + padding.z);
						position = position + vec2(margin.y, margin.x) + vec2(padding.y, padding.x);
					}

					size.x = std::max(layout.min_size.x, layout.layout.window_size.x);
					size.y = std::max(layout.min_size.y, layout.layout.window_size.y);

					node->childs[layout_counter]->self_info.layout.position = position;
					node->self_info.layout.add_widget(size);
					temp_stack.push_back(node->childs[layout_counter]);
					layout_counter++;
				}
			}
			if (widget_info_table.find(current_node->self_info.id) != widget_info_table.end()) {
				_widget_info& info = widget_info_table[current_node->self_info.id];
				StaticStyle interpolated_style = get_final_style(current_node->self_info.override_style, current_node->self_info.style, info);
				vec4f margin = style_attribute_get<vec4f>(interpolated_style.margin, info);
				vec4f padding = style_attribute_get<vec4f>(interpolated_style.padding, info);
				current_node->self_info.layout.window_size = current_node->self_info.layout.window_size + vec2(margin.y + margin.w, margin.x + margin.z) + vec2(padding.y + padding.w, padding.x + padding.z);
				current_node->self_info.layout.window_size.x = std::max(current_node->self_info.layout.window_size.x, current_node->self_info.min_size.x);
				current_node->self_info.layout.window_size.y = std::max(current_node->self_info.layout.window_size.y, current_node->self_info.min_size.y);
			}
		}

		vec2 global_padding_size;
		if (widget_info_table.find(current_layout->self_info.id) != widget_info_table.end()) {
			_widget_info& info = widget_info_table[current_layout->self_info.id];
			StaticStyle interpolated_style = get_final_style(current_layout->self_info.override_style, current_layout->self_info.style, info);
			vec4f margin = style_attribute_get<vec4f>(interpolated_style.margin, info);
			vec4f padding = style_attribute_get<vec4f>(interpolated_style.padding, info);
			global_padding_size = vec2(padding.y + padding.w, padding.x + padding.z);
		}
		current_layout->self_info.layout.window_size = current_layout->self_info.layout.window_size + global_padding;
		current_layout->self_info.layout.window_size.x = std::max(current_layout->self_info.layout.window_size.x, current_layout->self_info.min_size.x);
		current_layout->self_info.layout.window_size.y = std::max(current_layout->self_info.layout.window_size.y, current_layout->self_info.min_size.y);
	}

	// draw root
	box(current_layout->self_info.id, position, current_layout->self_info.layout.window_size, current_layout->self_info.style, U"", current_layout->self_info.override_style, current_layout->self_info.z_index);

	// draw layouts and their children from leaves to root of the tree
	std::vector<std::shared_ptr<layout_node>> layout_nodes_d = get_layouts_in_descending_order();
	for (std::shared_ptr<layout_node> node : layout_nodes_d) {
		vec2 known_parent_size = node->self_info.layout.window_size;
		node->self_info.layout.window_size = vec2(0, 0);
		int content_counter = 0;
		int layout_counter = 0;
		for (int i = 0; i < node->child_type_order.size(); i++) {
			layout_node::child_type type = node->child_type_order[i];
			if (type == layout_node::content) {
				content_info& content = node->contents[content_counter];

				vec2 size = content.size;
				vec2 position = node->self_info.layout.get_widget_position();
				if (widget_info_table.find(content.id) != widget_info_table.end()) {
					_widget_info& info = widget_info_table[content.id];
					StaticStyle interpolated_style = get_final_style(content.override_style, content.style, info);
					vec4f margin = style_attribute_get<vec4f>(interpolated_style.margin, info);
					size = size - vec2(margin.y + margin.w, margin.x + margin.z);
					position = position + vec2(margin.y, margin.x);
				}

				box(content.id, position, size, content.style, content.text, content.override_style, content.z_index);
				
				node->self_info.layout.add_widget(node->contents[content_counter].size);
				content_counter++;
			}
			else if (type == layout_node::layout) {
				layout_info& layout = node->childs[layout_counter]->self_info;
				
				vec2 size = layout.layout.window_size;
				vec2 position = node->self_info.layout.get_widget_position();
				if (widget_info_table.find(layout.id) != widget_info_table.end()) {
					_widget_info& info = widget_info_table[layout.id];
					StaticStyle interpolated_style = get_final_style(layout.override_style, layout.style, info);
					vec4f margin = style_attribute_get<vec4f>(interpolated_style.margin, info);
					vec4f padding= style_attribute_get<vec4f>(interpolated_style.padding, info);
					size = size - vec2(margin.y + margin.w, margin.x + margin.z);
					position = position + vec2(margin.y, margin.x);
				}
				
				size.x = std::max(layout.min_size.x, size.x);
				size.y = std::max(layout.min_size.y, size.y);
				
				box(layout.id, position, size, layout.style, U"", layout.override_style, layout.z_index);

				vec2 window_size = node->childs[layout_counter]->self_info.layout.window_size;
				window_size.x = std::max(layout.min_size.x, window_size.x);
				window_size.y = std::max(layout.min_size.y, window_size.y);
				node->self_info.layout.add_widget(window_size);
				layout_counter++;
			}
		}
		//std::cout << known_parent_size << " " << node->self_info.layout.window_size << std::endl;
	}

	current_layout = nullptr;
	layout_stack.clear();
}
