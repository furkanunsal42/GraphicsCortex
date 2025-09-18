#pragma once

#include <cinttypes>
#include <vector>
#include <string>
#include <functional>
#include <memory>

#include "Math/AABB.h"
#include "Font.h"
#include "Tools/ImmediateRendering/ImmediateRenderer.h"

class Widget;
class WidgetStyle;

class GUI {
public:

	typedef uint32_t widget_id;
	constexpr static widget_id root_widget = 0;
	constexpr static widget_id invalid_widget = -1;
	constexpr static float render_only_when_dirty = -1;

	// Weak EventType variants doesn't require the widget to be top-most widget
	enum EventType {
		Hovered_Weak,
		Clicked_Weak,
		RightClicked_Weak,
		DoubleClicked_Weak,

		Hovered,
		Clicked,
		RightClicked,
		DoubleClicked,
	};

	GUI& get();
	ImmediateRenderer& get_immediate_renderer();

	Widget create_widget(Widget parent_widget = Widget::null_widget);
	void render(Widget widget);

private:

	GUI();

	widget_id next_widget_id = 1;
	widget_id generate_widget_id();

	widget_id create_widget(widget_id parent_id = invalid_widget);
	void release_widget(widget_id id);
	void render(widget_id id);

	void* attached_window_handle = nullptr;

	struct WidgetInfo {
		widget_id id = invalid_widget;
		AABB3 area;

		WidgetStyle style;
		std::function<void()> custom_on_render;
	};

	std::unordered_map<widget_id, WidgetInfo> widgets;

	std::shared_ptr<VertexAttributeBuffer> vab;
};

struct WidgetStyle {
	glm::vec4 color;
	glm::vec4 border_color;
	glm::vec4 border_thickness;

	std::string text;
	font_id font = FontBank::not_a_font;
	std::shared_ptr<Texture2D> texture = nullptr;

	int32_t z = 0;

	bool capture_cursor_event = true;
	bool pass_through_cursor_event = false;
	float render_target_fps = GUI::render_only_when_dirty;
};

class Widget {
public:
	GUI::widget_id id = -1;
	GUI::widget_id parent_id = 0;

	~Widget();

	WidgetStyle& style();

	void set_on_render_function(std::function<void()> render_function);
	void set_on_event_function(std::function<void(GUI::EventType)> event_function);

	static const Widget null_widget;

private:
	void* owner_gui_identifier = nullptr;
	Widget(void* owner_gui_identifier = nullptr, GUI::widget_id id = -1, GUI::widget_id parent_id = 0);
};

const Widget Widget::null_widget = Widget(nullptr, -1, 0);

class CustomWidget {
public:

	glm::vec2 get_size();
	 
};
