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

//StaticStyle& Style::get_hover(){
//	return _on_hover;
//}
//
//StaticStyle& Style::get_active() {
//	return _on_active;
//}

template<typename T>
T optional_get(std::vector<std::optional<T>> fallback_list, T null_value = T()) {
	for (std::optional<T>& property: fallback_list) {
		if (property)
			return property.value();
	}
	return null_value;
}

Ui::Ui(Frame& frame) :
	frame(frame)
{
	window_size = Vec2<int>(frame.window_width, frame.window_height);
	_update_matrix(frame.window_width, frame.window_height);
	program = default_program::gui_program();
}

void Ui::new_frame(){
	
	if (window_size.x != frame.window_width || window_size.y != frame.window_height) {
		window_size = Vec2<int>(frame.window_width, frame.window_height);
		_update_matrix(frame.window_width, frame.window_height);
	}

	layouts.clear();

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
	
	// determine aabbs
	Vec2<float> screen_position = layouts.back().get_widget_position();
	Vec2<float> screen_position_hover = screen_position;
	
	Vec4<float> _style_margin = optional_get<Vec4<float>>({ style.margin });
	Vec4<float> _style_padding = optional_get<Vec4<float>>({ style.padding });

	Vec4<float> _style_margin_hover = optional_get<Vec4<float>>({ style.on_hover.margin , style.margin });
	Vec4<float> _style_padding_hover = optional_get<Vec4<float>>({ style.on_hover.padding, style.padding });

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
		else{
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
	StaticStyle style_to_use = (StaticStyle)style;
	if (active) style_to_use = (StaticStyle)style.on_active;
	else if (hover) style_to_use = (StaticStyle)style.on_hover;
	
	// insert object to layout
	Vec3<float> color =				optional_get<Vec3<float>>({			style_to_use.color,				style.color				});
	Vec2<float> displacement =		optional_get<Vec2<float>>({			style_to_use.displacement,		style.displacement		});
	Vec2<float> rotation_euler =	optional_get<Vec2<float>>({			style_to_use.rotation_euler,	style.rotation_euler	});
	Vec4<float> corner_rounding =	optional_get<Vec4<float>>({			style_to_use.corner_rounding,	style.corner_rounding	});
	Vec4<float> padding =			optional_get<Vec4<float>>({			style_to_use.padding,			style.padding			});
	Vec4<float> margin =			optional_get<Vec4<float>>({			style_to_use.margin,			style.margin			});
	Vec4<float> border_thickness =	optional_get<Vec4<float>>({			style_to_use.border_thickness,	style.border_thickness	});
	Vec3<float> border_color =		optional_get<Vec3<float>>({			style_to_use.border_color,		style.border_color		});
	Frame::CursorType cursor_type = optional_get<Frame::CursorType>({	style_to_use.cursor_type,		style.cursor_type		});

	layouts.back().add_widget(Vec2<float>(size.x + margin.y + margin.w, size.y + margin.x + margin.z));
	Vec2<float> padded_size = size - Vec2<float>(padding.y + padding.w, padding.x + padding.z);

	// render

	if (hover) frame.set_cursor_type(cursor_type);

	Mesh box_mesh(aabb_to_use.generate_model());
	Mesh_s box_mesh_s = Mesh_s(box_mesh);
	Program_s program_s = Program_s(program);
	Graphic box_graphic;

	box_graphic.load_program(program_s);
	box_graphic.load_model(box_mesh_s);
	box_graphic.set_uniform("screen_position", screen_position.x, frame.window_height - screen_position.y);
	box_graphic.set_uniform("projection", projection_matrix);
	box_graphic.set_uniform("rect_color", color.x, color.y, color.z, 1.0f);
	box_graphic.set_uniform("rect_size", padded_size.x, padded_size.y);
	box_graphic.set_uniform("corner_rounding", corner_rounding.x, corner_rounding.y, corner_rounding.z, corner_rounding.w);
	box_graphic.set_uniform("border_color", border_color.x, border_color.y, border_color.z, 1.0f);
	box_graphic.set_uniform("border_thickness", border_thickness.x, border_thickness.y, border_thickness.z, border_thickness.w);
	

	box_graphic.update_matrix();
	box_graphic.update_uniforms();
	box_graphic.draw(false);
}

void Ui::end() {
	Vec2<float> size = layouts.back().window_size;
	layouts.pop_back();
	layouts.back().add_widget(size);
}

void Ui::_update_matrix(int screen_width, int screen_height) {
	projection_matrix = glm::ortho(0.0f, (float)screen_width, 0.0f, (float)screen_height);
}