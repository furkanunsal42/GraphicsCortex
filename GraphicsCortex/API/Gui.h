#pragma once
#include "CortexMath.h"
#include "Frame.h"
#include "ShaderCompiler.h"

#include <functional>
#include <optional>

class Layout {
public:
	Layout(uint32_t type, const AABB2& aabb);
	Layout(uint32_t type = Layout::Horizional, const Vec2<float>& position = {0, 0}, const Vec2<float>& window_size = {0, 0});

	Vec2<float> window_size;
	Vec2<float> position;

	enum LayoutType {
		Horizional,
		Vertical,
	};

	LayoutType type;

	void add_widget(const Vec2<float>& size);
	Vec2<float> get_widget_position();
	Vec2<float> get_position();
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

	std::optional<Time> color_change;
	std::optional<Time> displacement_change;
	std::optional<Time> rotation_change;
	std::optional<Time> corner_rounding_change;
	std::optional<Time> padding_change;
	std::optional<Time> margin_change;
	std::optional<Time> border_thickness_change;
	std::optional<Time> border_color_change;
};

class Style : public StaticStyle {
public:

	StaticStyle on_hover;
	StaticStyle on_active;
};

class FunctionalStyle : Style {

	void on_hover(std::function<Style(Time)> hover_function);
	void on_active(std::function<Style(Time)> active_function);
	void on_passive(std::function<Style(Time)> passive_function);

private:
	std::function<Style(Time)> _on_hover;
	std::function<Style(Time)> _on_active;
	std::function<Style(Time)> _on_passive;
};

class _widget {
public:
	_widget(const Ui& ui, const Style& style, Vec2<float> size = { 0, 0 }, Vec2<float> position = { 0, 0 });
	Style style;
	Vec2<float> size;
	Vec2<float> position;
	Ui& _ui;
	virtual void render(const Vec2<int>& cursor_position);
	virtual void update_position(const Vec2<float>& displacement);
};

class _container : public _widget {
public:
	enum LayoutType {
		Horizional,
		Vertical,
	};

	_container(const Ui& ui, const Style& style, _container::LayoutType type = _container::Horizional, Vec2<float> positinon = {0, 0});

	std::vector<std::reference_wrapper<_widget>> widgets;

	_container::LayoutType type;

	void render(const Vec2<int>& cursor_position);
	void update_position(const Vec2<float>& displacement);
	void _add_widget(const Vec2<float>& size);
	Vec2<float> _get_next_widget_position();
};

class _text_widget : public _widget {
public:
	std::string text;
};

class Ui {
public:
	Ui(Frame& frame);
	void new_frame();

	// positions and sizes of layouts and inner widgets can change upon hovering,
	// it creates a compilcated flow for the algorithm.
	// the algorithm flow for rendering each layout with it's widgets is the follows:
	// --
	// compute starting position for layout
	// compute sizes and positions of widgets (check for hover, if so update)
	// compute layout size
	// check layout for hover
	// if not render layout, render widgets
	// if so,
	// compute new position of layout
	// compute new positions and sizes of widgets (check for hover, if so update)
	// render layout
	// render widgets
	// --

	void begin(const Style& style, Layout::LayoutType type = Layout::Horizional);
	bool box(const std::string& id, const Vec2<float>& size, const Style& style);
	void end();
private:

	friend _widget;
	friend _container;

	Time last_update_ms;
	Time frame_time;
	
	//std::unordered_map<std::string, Time> animation_state;
	std::vector<std::function<void()>> _render_calls;
	
	std::string _focused_id;
	std::string _hovered_id;
	std::vector<Layout> layouts;
	std::vector<Style> _layout_styles;
	glm::mat4 projection_matrix;
	Program program;
	Frame& frame;

	Vec2<int> window_size;

	void _update_matrix(int screen_width, int screen_height);
	Frame::CursorState _cursor_state;
	bool _cursor_state_just_changed = false;
	bool _hovered = false;
};
