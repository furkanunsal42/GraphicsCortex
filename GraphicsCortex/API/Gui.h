#pragma once

#include "CortexMath.h"
#include "Frame.h"
#include "ShaderCompiler.h"

#include <functional>
#include <chrono>
#include <optional>

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

class StaticStyle {
public:
	std::optional<Vec3<float>> color;
	std::optional<Vec2<float>> displacement;
	std::optional<Vec2<float>> rotation_euler;
	std::optional<Vec4<float>> corner_rounding; // corner indicies follows traditional coordinate system partition	// 1 | 0
																													// --|-- 
																													// 2 | 3

	std::optional<Vec4<float>> padding;
	std::optional<Vec4<float>> margin;
	std::optional<Vec4<float>> border_thickness;
	std::optional<Vec3<float>> border_color;
	std::optional<Frame::CursorType> cursor_type = Frame::Arrow;
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

	StaticStyle on_hover;
	StaticStyle on_active;

private:
	std::optional<Time> _current_color_time;
	std::optional<Time> _current_displacement_time;
	std::optional<Time> _current_rotation_time;
	std::optional<Time> _current_corner_rounding_time;
	std::optional<Time> _current_padding_time;
	std::optional<Time> _current_margin_time;
	std::optional<Time> _current_border_thickness_time;
	std::optional<Time> _current_border_color_time;
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

class Ui {
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
