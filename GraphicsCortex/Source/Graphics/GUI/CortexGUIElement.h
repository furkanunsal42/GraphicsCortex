#pragma once

#include <cinttypes>
#include <string>
#include <functional>
#include <memory>

#include "Math/AABB.h"
#include "Font.h"

class GUI;

constexpr static float render_only_when_dirty = -1;
typedef uint32_t element_t;
constexpr static element_t root_element = 0;
constexpr static element_t invalid_element = -1;

// Weak EventType variants doesn't require the element to be top-most widget
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

class Element {
public:

	Element() = default;
	//~Element();

	Element create_child();
	void set_parent(Element new_parent);
	Element get_parent();

	glm::vec2& texcoord_min();
	glm::vec2& texcoord_max();
	glm::vec4& color();
	glm::vec4& border_thickness();
	glm::vec4& border_color0();
	glm::vec4& border_color1();
	glm::vec4& border_color2();
	glm::vec4& border_color3();
	glm::vec4& shadow_thickness();
	glm::vec4& shadow_color();
	glm::vec2& position();
	glm::vec2& size();
	int32_t& z();

	std::shared_ptr<Texture2D>& texture();

	friend bool operator==(const Element& a, const Element& b);

	void set_on_render_function(std::function<void()> render_function);
	void set_on_event_function(std::function<void(GUIEventType)> event_function);

	static const Element null_element;

private:
	friend GUI;
	element_t id = invalid_element;
	void* owner_gui_identifier = nullptr;
	Element(void* owner_gui_identifier, element_t id);
};
