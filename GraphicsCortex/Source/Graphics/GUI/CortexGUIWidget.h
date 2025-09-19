#pragma once

#include <cinttypes>
#include <string>
#include <functional>
#include <memory>

#include "Math/AABB.h"
#include "Font.h"

class GUI;

constexpr static float render_only_when_dirty = -1;
typedef uint32_t widget_t;
constexpr static widget_t root_widget = 0;
constexpr static widget_t invalid_widget = -1;

// Weak EventType variants doesn't require the widget to be top-most widget
enum GUIEventType {
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
	widget_t id = invalid_widget;

	~Widget();

	Widget create_child();

	WidgetStyle& style();
	glm::vec2& position();
	glm::vec2& size();
	int32_t& z();
	std::string& text();

	void set_on_render_function(std::function<void()> render_function);
	void set_on_event_function(std::function<void(GUIEventType)> event_function);

	static const Widget null_widget;

private:
	friend GUI;
	void* owner_gui_identifier = nullptr;
	Widget(void* owner_gui_identifier = nullptr, widget_t id = invalid_widget);
};