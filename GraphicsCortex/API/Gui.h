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

	enum LayoutType {
		Horizional,
		Vertical,
	};

	LayoutType type;

	void add_widget(const Vec2<float>& size);
	Vec2<float> get_widget_position();
};

namespace {
	typedef std::chrono::milliseconds Time;
}

class StaticStyle {
public:
	Vec3<float> color;
	Vec2<float> displacement;
	Vec2<float> rotation_euler;
	Vec4<float> corner_rounding; // corner indicies follows traditional coordinate system partition // 1 | 0
																									// --|-- 
																									// 2 | 3

	Vec4<float> padding;
	Vec4<float> margin;
	float border_thickness;
	Vec3<float> border_color;
	Frame::CursorType cursor_type = Frame::Arrow;

	Time color_change;
	Time displacement_change;
	Time rotation_change;
	Time corner_rounding_change;
	Time padding_change;
	Time margin_change;
	Time border_thickness_change;
	Time border_color_change;
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

class Ui {
public:

	Ui(Frame& frame);
	void new_frame();
	void begin(Layout::LayoutType type = Layout::Horizional);
	bool box(const std::string& id, const Vec2<float>& size, const Style& style);
	void end();

private:
	
	std::string _focused_id;
	std::vector<Layout> layouts;
	glm::mat4 projection_matrix;
	Program program;
	Frame& frame;
	
	Vec2<int> window_size;

	void _update_matrix(int screen_width, int screen_height);
	Frame::CursorState _cursor_state;
	bool _cursor_state_just_changed = false;
	bool _hovered = false;
};
