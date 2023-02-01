#pragma once
#include "CortexMath.h"
#include <functional>
#include <chrono>

class Layout {
public:
	Vec2<float> window_size;
	Vec2<float> position;

	void add_widget(const Widget& widget);
};

namespace {
	typedef std::chrono::milliseconds Time;
}

class Style {
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

	Time color_change;
	Time displacement_change;
	Time rotation_change;
	Time corner_rounding_change;
	Time padding_change;
	Time margin_change;
	Time border_thickness_change;
	Time border_color_change;
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


class Widget {
public:
	Widget(const std::string& id, Vec2<float> size);

private:
	Vec2<float> size;
	std::string id;



};