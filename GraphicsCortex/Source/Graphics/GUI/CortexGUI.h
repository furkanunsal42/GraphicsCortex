#pragma once

#include <cinttypes>
#include <vector>
#include <string>
#include <functional>
#include <memory>

#include "Math/AABB.h"
#include "Font.h"
#include "Tools/ImmediateRendering/ImmediateRenderer.h"

class GUI;
 
constexpr static float render_only_when_dirty = -1;
typedef uint32_t widget_id;
constexpr static widget_id root_widget = 0;
constexpr static widget_id invalid_widget = -1;

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

struct WidgetStyle {
	glm::vec4 color = glm::vec4(1);
	glm::vec4 border_color = glm::vec4(1);
	glm::vec4 border_thickness = glm::vec4(0);;

	font_id font = FontBank::not_a_font;
	std::shared_ptr<Texture2D> texture = nullptr;

	glm::ivec2 target_size = glm::vec2(-1);

	bool capture_cursor_event = true;
	bool pass_through_cursor_event = false;
	float render_target_fps = render_only_when_dirty;
};

class Widget {
public:
	widget_id id = -1;

	~Widget();

	Widget create_child();

	WidgetStyle& style();
	glm::vec2& position();
	glm::vec2& size();
	int32_t& z();
	std::string& text();

	void set_on_render_function(std::function<void()> render_function);
	void set_on_event_function(std::function<void(EventType)> event_function);

	//static const Widget null_widget;

private:
	friend GUI;
	void* owner_gui_identifier = nullptr;
	Widget(void* owner_gui_identifier = nullptr, widget_id id = -1);
};

//const Widget Widget::null_widget = Widget(nullptr, -1, 0);

class GUI {
public:

	static GUI& get();
	ImmediateRenderer& get_immediate_renderer();

	Widget create_widget();
	Widget create_widget(Widget& parent_widget);
	bool does_widget_exist(Widget& widget);

	void render(Widget& root_widget);

private:
	friend Widget;

	GUI();

	widget_id next_widget_id = 1;
	widget_id _generate_widget_id();

	widget_id _create_widget(widget_id parent_id = invalid_widget);
	void _release_widget(widget_id id);
	bool _does_widget_exist(widget_id id);
	
	void _traverse_children(widget_id root_id, std::function<void(widget_id, glm::vec2)> lambda);
	void _traverse_parents(widget_id root_id, std::function<void(widget_id)> lambda);

	void _render(widget_id id);

	void* attached_window_handle = nullptr;

	struct WidgetInfo {
		widget_id id = invalid_widget;
		widget_id parent_id = invalid_widget;
		std::vector<widget_id> children;

		glm::vec2 position = glm::vec2(0);
		glm::vec2 actual_size = glm::vec2(-1);
		int32_t z = 0;

		std::string text = "";

		WidgetStyle style;
		std::function<void()> custom_on_render;
		std::function<void(EventType)> custom_on_event;
	};

	std::unordered_map<widget_id, WidgetInfo> widgets;

	std::shared_ptr<ImmediateRenderer> immediate_renderer;
	std::shared_ptr<VertexAttributeBuffer> vab;
};




