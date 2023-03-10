#pragma once

#include "CortexMath.h"
#include "Frame.h"
#include "ShaderCompiler.h"

#include <functional>
#include <chrono>
#include <optional>
#include <iostream>

#include "SharedPtr_Graphic.h"

class Layout {
public:
	Layout(uint32_t type, const AABB2& aabb);
	Layout(uint32_t type = Layout::Horizional, const Vec2<float>& position = {0, 0}, const Vec2<float>& window_size = {0, 0});

	Vec2<float> window_size;
	Vec2<float> position;
	Vec2<float> _raw_position;

	enum LayoutType {
		Horizional,
		Vertical,
	};

	LayoutType type;

	void add_widget(const Vec2<float>& size);
	Vec2<float> get_widget_position();
	Vec2<float> get_position();
	Vec2<float> get_raw_position();
	void clear();
	void update_position_by_style(const Vec4<float>& margin, const Vec4<float>& padding);


};

namespace {
	typedef double Time;
}

namespace gui {
	
	typedef vec3f color;
	typedef vec2f displacement;
	typedef vec2f rotation_euler;
	typedef vec4f corner_rounding;
	typedef vec4f padding;
	typedef vec4f margin;
	typedef vec4f border_thickness;
	typedef vec3f border_color;

	color colorcode(uint32_t hexcode);

}

class _interpolation{
public:

	_interpolation() = default;
		
	enum type{
		Linear,
		Polynomial
	};

	type interpolation_type = _interpolation::Linear;
	float power = 1.0f;
	
	template<typename T>
	T interpolate(T default_value, T target_value, Time max_time, Time current_time) {
		switch (interpolation_type) {
		case Linear:
			return _linear_interpolation(default_value, target_value, max_time, current_time);
			break;
		case Polynomial:
			return _polynomial_interpolation(default_value, target_value, max_time, current_time);
			break;
		}
	}

private:
	template<typename T>
	T _linear_interpolation(T default_value, T target_value, Time max_time, Time current_time) {
		return default_value + (target_value - default_value) / max_time * (std::min(current_time, max_time));
	}
	template<typename T>
	T _polynomial_interpolation(T default_value, T target_value, Time max_time, Time current_time) {
		return _linear_interpolation(default_value, target_value, std::pow(max_time, power), std::pow(current_time, power));
	}
};

namespace Interpolation {
	_interpolation linear();
	_interpolation polynomial(float power = 1.0f);
}

struct _widget_info;

struct Persentage {
public:
	enum RespectedAttribute {
		SIZE_X,
		SIZE_Y,
		PADDED_SIZE_X,
		PADDED_SIZE_Y,
		MAX_SIZE_DIM,
		MIN_SIZE_DIM,
		POSITION_X,
		POSITION_Y,
		PADDED_POSITION_X,
		PADDED_POSITION_Y,
		PARENT_WIDTH,
		PARENT_HEIGHT,
		AVAILABLE_WIDTH,
		AVAILABLE_HEIGHT,
	};

	static std::string RespectedAttribute_to_string(const RespectedAttribute& attribute);


	Persentage(float value, RespectedAttribute attribute_type);

	float get_value(const _widget_info& info);

	RespectedAttribute attribute_type;
	float value;
};


template<typename T>
struct StyleAttribute {
public:

	StyleAttribute() :
		use_persentage(false), value(T()) {}

	bool use_value = false;
	bool use_persentage = false;
	union {
		T value;
		Persentage persentage;
	};

	void operator=(const T& other) {
		value = other;
		use_value = true;
		use_persentage = false;
	}

	void operator=(const Persentage& other) {
		persentage = other;
		use_persentage = true;
		use_value = false;
	}

	bool exist() {
		return use_value || use_persentage;
	}

	T get_value(const _widget_info& info) {
		if (use_value)
			return value;
		else if (use_persentage) {
			return T(persentage.get_value(info));
		}
		else {
			return T();	// default case 
		}
	}
};


class StaticStyle {
public:
	
	// StyleAttribute<vec3f> color_attrib;	// work in progress
	
	std::optional<Vec3<float>> color;
	std::optional<Vec2<float>> displacement;
	std::optional<Vec2<float>> rotation_euler;	// not implemented 
	StyleAttribute<vec4f> corner_rounding; // corner indicies follows traditional coordinate system partition	// 1 | 0
																													// --|-- 
																													// 2 | 3

	std::optional<Vec4<float>> padding;
	std::optional<Vec4<float>> margin;
	std::optional<Vec4<float>> border_thickness;
	std::optional<Vec3<float>> border_color;
	std::optional<Frame::CursorType> cursor_type;
};

class Style : public StaticStyle {
public:
	
	std::optional<Time> color_change;
	std::optional<Time> displacement_change;
	std::optional<Time> rotation_change;
	std::optional<Time> corner_rounding_change;
	std::optional<Time> padding_change;
	std::optional<Time> margin_change;
	std::optional<Time> border_thickness_change;
	std::optional<Time> border_color_change;

	std::optional<_interpolation> color_interpolation;
	std::optional<_interpolation> displacement_interpolation;
	std::optional<_interpolation> rotation_interpolation;
	std::optional<_interpolation> corner_rounding_interpolation;
	std::optional<_interpolation> padding_interpolation;
	std::optional<_interpolation> margin_interpolation;
	std::optional<_interpolation> border_thickness_interpolation;
	std::optional<_interpolation> border_color_interpolation;

	StaticStyle on_hover;
	StaticStyle on_active;

private:
};

class FunctionalStyle : StaticStyle {

	void on_hover(std::function<Style(Time)> hover_function);
	void on_active(std::function<Style(Time)> active_function);
	void on_passive(std::function<Style(Time)> passive_function);

	void clear_hover();
	void clear_active();
	void clear_passive();

private:
	std::function<Style(Time)> _on_hover;
	std::function<Style(Time)> _on_active;
	std::function<Style(Time)> _on_passive;
};


// object based structure

struct _widget_info {
public:
	vec2f size;
	vec2f position;

	bool is_hovering;
	//Time last_update;

	Time _current_color_time = 0;
	Time _current_displacement_time = 0;
	Time _current_rotation_time = 0;
	Time _current_corner_rounding_time = 0;
	Time _current_padding_time = 0;
	Time _current_margin_time = 0;
	Time _current_border_thickness_time = 0;
	Time _current_border_color_time = 0;

	void increment_time(Time deltatime);
};

class Box {
public:
	Style style;

	Box(Frame& frame, Style style, AABB2 aabb);
	Box(Frame& frame, Style style, AABB2 aabb, Program_s custom_renderer);
	
	void set_position(Vec2<float> position);
	void set_size(Vec2<float> size);

	bool is_mouse_hover();

	void render(Time deltatime);

	Style overwrite_style;

	vec2f get_size();
	vec2f get_position();

private:

	Style _style;
	Frame& _frame_ref;
	AABB2 _aabb;				//aabb size will be padded and displaced
	vec2 _original_size;		//original size will not be padded		
	vec2 _original_position;	//original size will not be displaced
	Graphic_s _graphic_representation;

	_widget_info _info;
};

class Gui {	// similiar function with UI class, new implementation 
public:
	Gui() = delete;
	static void new_frame(Frame& frame);
	static glm::mat4 _projection_matrix;
	static Vec2<int> window_size;
	static bool _hover_happened;
	static Program_s default_gui_renderer;
private:
	static void _initialize();
	static bool _initialized;

	friend Box;
};

/*
// css-like layout structure

struct WidgetInfo {
public:
	WidgetInfo(bool was_hovered = false, bool was_active = false, Time hover_duration = 0, Time active_duration = 0, Time last_update = -1) : 
		was_hovered(was_hovered), was_active(was_active), hover_duration(hover_duration), active_duration(active_duration), last_update(last_update) {}

	bool was_hovered;
	bool was_active;
	Time hover_duration;
	Time active_duration;
	Time last_update;
};

class Ui {	// legacy
public:
	Ui(Frame& frame);
	void new_frame();
	void begin(Layout::LayoutType type = Layout::Horizional);
	bool box(const std::string& id, const Vec2<float>& size, const Style& style);
	void end(const Style& style);

private:
	
	Time last_update_ms;
	Time frame_time;
	
	//std::unordered_map<std::string, Time> animation_state;
	std::vector<std::function<void(bool& hover_out, AABB2& aabb_out, StaticStyle& style_to_use_out)>> _position_calculation_calls;
	std::vector<std::function<void(bool hover, AABB2 aabb, StaticStyle style_to_use)>> _render_calls;
	std::string _focused_id;
	std::string _hovered_id;
	std::vector<Layout> layouts;
	glm::mat4 projection_matrix;
	Program program;
	Frame& frame;

	Vec2<int> window_size;

	std::unordered_map<std::string, WidgetInfo> widget_info_table;

	void _update_matrix(int screen_width, int screen_height);
	Frame::CursorState _cursor_state;
	bool _cursor_state_just_changed = false;
	bool _hovered = false;
};
*/