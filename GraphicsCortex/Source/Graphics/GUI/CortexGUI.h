#pragma once

#include <cinttypes>
#include <vector>
#include <string>
#include <functional>
#include <memory>

#include "glm.hpp"
#include "Math/AABB.h"
#include "Font.h"
#include "Tools/ImmediateRendering/ImmediateRenderer.h"

#include "CortexGUIElement.h"
#include "CortexGUIWidget.h"

extern std::filesystem::path gui_renderer_shader_parent_path;

namespace widget {
	template<typename T> WidgetHandle<T> create();
	template<typename T> void release(WidgetHandle<T>& widget);
	template<typename T> bool does_exist(WidgetHandle<T>& widget);
}

class GUI {
public:
	static GUI& get();
	ImmediateRenderer& get_immediate_renderer();

	template<typename T> WidgetHandle<T> create_widget();
	template<typename T> void release_widget(WidgetHandle<T>& widget);
	template<typename T> bool does_widget_exist(WidgetHandle<T>& widget);

	Element create_element();
	Element create_element(Element& parent_element);
	void set_element_parent(Element& target_element, Element& new_parent);
	void release_element(Element& element);
	bool does_element_exist(Element& element);
	
	void render(Element& root_element);
	template<typename T>
	void render(WidgetHandle<T>& widget);

	Widget& get_widget_data(widget_t id);

private:
	friend Element;

	template <typename T>
	friend class WidgetHandle;

	GUI();
	const int32_t total_number_of_z_layers = 2048;

	bool shaders_compiled = false;
	void _compile_shaders();

	widget_t next_widget_id = 1;
	widget_t _generate_widget_id();
	
	template<typename T> 
	widget_t _create_widget();
	void _release_widget(widget_t widget);
	bool _does_widget_exist(widget_t widget);

	element_t next_element_id = 1;
	element_t _generate_element_id();

	element_t _create_element(element_t parent_id = invalid_element);
	void _set_element_parent(element_t target_element, element_t set_new_parent);
	void _release_element(element_t id);
	bool _does_element_exist(element_t id);
	
	void _render_tmp(element_t id);

	void _render(element_t id);
	void _render_and_partially_render_parents(element_t id);
	void _render_and_fully_render_parents(element_t id);

	void _traverse_children(element_t root_id, std::function<void(element_t, glm::vec2)> lambda);
	void _traverse_children_dfs(element_t root_id, std::function<void(element_t, glm::vec2, glm::vec4)> lambda);
	void _traverse_parents(element_t root_id, std::function<void(element_t)> lambda);

	void _init_vab(size_t begin_size_in_bytes = 1024 * 1024);
	void _resize_vab(size_t new_size_in_bytes);
	void _consolidate_vab();
	void _append_data_to_vab(void* data, size_t data_size_in_bytes);
	void _update_vab_to_render(element_t id);

	void* attached_window_handle = nullptr;

	struct ElementInfo {
		constexpr static size_t element_does_not_exist = -1;
		element_t id = invalid_element;
		element_t parent_id = invalid_element;
		std::vector<element_t> children;

		glm::vec2 position = glm::vec2(0);
		glm::vec2 size = glm::vec2(-1);
		glm::vec2 texcoord_min = glm::vec2(0);
		glm::vec2 texcoord_max = glm::vec2(1);
		glm::vec4 color = glm::vec4(1);
		glm::vec4 border_color = glm::vec4(1);
		glm::vec4 border_thickness = glm::vec4(0);
		int32_t z = 0;
		int32_t old_z = 0;
		
		std::shared_ptr<Texture2D> texture = nullptr;

		bool capture_cursor_event = true;
		bool pass_through_cursor_event = false;
		float render_target_fps = render_only_when_dirty;

		std::function<void()> custom_on_render;
		std::function<void(GUIEventType)> custom_on_event;

		size_t vab_begin = element_does_not_exist;
	
		bool vab_properties_changed = false;
		bool texture_changed = false;
	};

	struct Vertex {
		Vertex() = default;
		Vertex(
			glm::vec3 pos,
			glm::vec2 texcoord,
			glm::vec4 color
		);

		glm::vec3 position;
		glm::vec2 texcoord;
		glm::vec4 color;
	};

	std::unordered_map<widget_t, std::shared_ptr<Widget>> widgets;

	std::unordered_map<element_t, ElementInfo> elements;
	std::map<int32_t, std::vector<element_t>> z_to_element_table;

	size_t vab_total_size_in_bytes = 0;
	size_t vab_used_size_in_bytes = 0;
	std::shared_ptr<VertexAttributeBuffer> vab;

	std::shared_ptr<Program> gui_renderer;
	std::shared_ptr<Program> gui_renderer_texture;

	std::shared_ptr<ImmediateRenderer> immediate_renderer;
};

#include "CortexGUI_Templated.h"