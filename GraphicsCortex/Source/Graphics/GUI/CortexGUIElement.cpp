#include "CortexGUIElement.h"
#include "CortexGUI.h"
#include "WindowBoundGlobalResources.h"

const Element Element::null_element = Element(nullptr, invalid_element);

//Element::~Element()
//{
//	if (id == invalid_element)
//		return;
//
//	if (active_global_resources == nullptr || active_global_resources->GUI == nullptr)
//		return;
//
//	GUI::get()._release_element(id);
//}

Element::Element(void* owner_gui_identifier, element_t id) :
	owner_gui_identifier(owner_gui_identifier), id(id)
{

}

Element Element::create_child() {
	if (!GUI::get().does_element_exist(*this)) {
		std::cout << "[GUI Error] Element::create_child() is called on invalid Element" << std::endl;
		ASSERT(false);
	}

	return GUI::get().create_element(*this);
}

void Element::set_parent(Element new_parent)
{
	if (!GUI::get().does_element_exist(*this))
		return;

	GUI::get().set_element_parent(*this, new_parent);
}

Element Element::get_parent() {
	if (!GUI::get().does_element_exist(*this))
		return Element::null_element;

	return Element(owner_gui_identifier, GUI::get().elements[id].parent_id);
}

glm::vec2& Element::texcoord_min()
{
	if (!GUI::get().does_element_exist(*this)) {
		std::cout << "[GUI Error] Element::texcoord_min() is called on invalid Element" << std::endl;
		ASSERT(false);
	}

	GUI::get().elements[id].vab_properties_changed = true;

	return GUI::get().elements[id].texcoord_min;
}

glm::vec2& Element::texcoord_max()
{
	if (!GUI::get().does_element_exist(*this)) {
		std::cout << "[GUI Error] Element::texcoord_max() is called on invalid Element" << std::endl;
		ASSERT(false);
	}

	GUI::get().elements[id].vab_properties_changed = true;

	return GUI::get().elements[id].texcoord_max;
}

glm::vec4& Element::color()
{
	if (!GUI::get().does_element_exist(*this)) {
		std::cout << "[GUI Error] Element::color() is called on invalid Element" << std::endl;
		ASSERT(false);
	}

	GUI::get().elements[id].vab_properties_changed = true;

	return GUI::get().elements[id].color0;
}

glm::vec4& Element::border_thickness()
{
	if (!GUI::get().does_element_exist(*this)) {
		std::cout << "[GUI Error] Element::border_thickness() is called on invalid Element" << std::endl;
		ASSERT(false);
	}

	GUI::get().elements[id].vab_properties_changed = true;

	return GUI::get().elements[id].border_thickness;

}

glm::vec4& Element::border_rounding()
{
	if (!GUI::get().does_element_exist(*this)) {
		std::cout << "[GUI Error] Element::border_rounding() is called on invalid Element" << std::endl;
		ASSERT(false);
	}

	GUI::get().elements[id].vab_properties_changed = true;

	return GUI::get().elements[id].border_rounding;
}

glm::vec4& Element::border_color0()
{
	if (!GUI::get().does_element_exist(*this)) {
		std::cout << "[GUI Error] Element::border_color0() is called on invalid Element" << std::endl;
		ASSERT(false);
	}

	GUI::get().elements[id].vab_properties_changed = true;

	return GUI::get().elements[id].border_color0;
}

glm::vec4& Element::border_color1()
{
	if (!GUI::get().does_element_exist(*this)) {
		std::cout << "[GUI Error] Element::border_color1() is called on invalid Element" << std::endl;
		ASSERT(false);
	}

	GUI::get().elements[id].vab_properties_changed = true;

	return GUI::get().elements[id].border_color1;
}

glm::vec4& Element::border_color2()
{
	if (!GUI::get().does_element_exist(*this)) {
		std::cout << "[GUI Error] Element::border_color2() is called on invalid Element" << std::endl;
		ASSERT(false);
	}

	GUI::get().elements[id].vab_properties_changed = true;

	return GUI::get().elements[id].border_color2;
}

glm::vec4& Element::border_color3()
{
	if (!GUI::get().does_element_exist(*this)) {
		std::cout << "[GUI Error] Element::border_color3() is called on invalid Element" << std::endl;
		ASSERT(false);
	}

	GUI::get().elements[id].vab_properties_changed = true;

	return GUI::get().elements[id].border_color3;
}

glm::vec4& Element::shadow_thickness()
{
	if (!GUI::get().does_element_exist(*this)) {
		std::cout << "[GUI Error] Element::shadow_thickness() is called on invalid Element" << std::endl;
		ASSERT(false);
	}

	GUI::get().elements[id].vab_properties_changed = true;

	return GUI::get().elements[id].shadow_thickness;
}

glm::vec4& Element::shadow_color()
{
	if (!GUI::get().does_element_exist(*this)) {
		std::cout << "[GUI Error] Element::shadow_color() is called on invalid Element" << std::endl;
		ASSERT(false);
	}

	GUI::get().elements[id].vab_properties_changed = true;

	return GUI::get().elements[id].shadow_color;
}

glm::vec2& Element::position() {
	if (!GUI::get().does_element_exist(*this)) {
		std::cout << "[GUI Error] Element::position() is called on invalid Element" << std::endl;
		ASSERT(false);
	}

	GUI::get().elements[id].vab_properties_changed = true;

	return GUI::get().elements[id].position;
}

glm::vec2& Element::size() {
	if (!GUI::get().does_element_exist(*this)) {
		std::cout << "[GUI Error] Element::size() is called on invalid Element" << std::endl;
		ASSERT(false);
	}
	
	GUI::get().elements[id].vab_properties_changed = true;

	return GUI::get().elements[id].size;
}

int32_t& Element::z() {
	if (!GUI::get().does_element_exist(*this)) {
		std::cout << "[GUI Error] Element::z() is called on invalid Element" << std::endl;
		ASSERT(false);
	}

	GUI::get().elements[id].vab_properties_changed = true;

	return GUI::get().elements[id].z;
}

std::shared_ptr<Texture2D>& Element::texture()
{
	if (!GUI::get().does_element_exist(*this)) {
		std::cout << "[GUI Error] Element::texture() is called on invalid Element" << std::endl;
		ASSERT(false);
	}

	GUI::get().elements[id].texture_changed = true;

	return GUI::get().elements[id].texture;
}

bool operator==(const Element& a, const Element& b)
{
	bool same_id = a.id == b.id;
	bool same_gui = a.owner_gui_identifier == b.owner_gui_identifier;
	bool both_are_null = a.id == invalid_element && b.id == invalid_element;

	return (same_gui && same_id) || both_are_null;
}
