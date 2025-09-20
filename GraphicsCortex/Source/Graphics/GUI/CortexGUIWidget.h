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

class Widget {
public:

	Widget() = default;
	//~Widget();

	Widget create_child();
	void set_parent(Widget new_parent);
	Widget get_parent();

	glm::vec2& texcoord_min();
	glm::vec2& texcoord_max();
	glm::vec4& color();
	glm::vec4& border_color();
	glm::vec4& border_thickness();
	glm::vec2& position();
	glm::vec2& size();
	int32_t& z();

	std::shared_ptr<Texture2D>& texture();

	friend bool operator==(const Widget& a, const Widget& b);

	void set_on_render_function(std::function<void()> render_function);
	void set_on_event_function(std::function<void(GUIEventType)> event_function);

	static const Widget null_widget;

private:
	friend GUI;
	widget_t id = invalid_widget;
	void* owner_gui_identifier = nullptr;
	Widget(void* owner_gui_identifier, widget_t id);
};
