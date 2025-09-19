#pragma once

#include <cinttypes>
#include <vector>
#include <string>
#include <functional>
#include <memory>

#include "Math/AABB.h"
#include "Font.h"
#include "Tools/ImmediateRendering/ImmediateRenderer.h"

#include "CortexGUIWidget.h"

extern std::filesystem::path gui_renderer_shader_parent_path;

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
	const int32_t total_number_of_z_layers = 2048;

	bool shaders_compiled = false;
	void _compile_shaders();

	widget_t next_widget_id = 1;
	widget_t _generate_widget_id();

	widget_t _create_widget(widget_t parent_id = invalid_widget);
	void _release_widget(widget_t id);
	bool _does_widget_exist(widget_t id);
	
	void _render_tmp(widget_t id);

	void _render(widget_t id);
	void _render_and_partially_render_parents(widget_t id);
	void _render_and_fully_render_parents(widget_t id);

	void _traverse_children(widget_t root_id, std::function<void(widget_t, glm::vec2)> lambda);
	void _traverse_parents(widget_t root_id, std::function<void(widget_t)> lambda);

	void _init_vab(size_t begin_size_in_bytes = 1024 * 1024);
	void _resize_vab(size_t new_size_in_bytes);
	void _consolidate_vab();
	void _append_data_to_vab(void* data, size_t data_size_in_bytes);
	void _update_vab_to_render(widget_t id);

	void* attached_window_handle = nullptr;

	struct WidgetInfo {
		constexpr static size_t widget_does_not_exist = -1;
		widget_t id = invalid_widget;
		widget_t parent_id = invalid_widget;
		std::vector<widget_t> children;

		glm::vec2 position = glm::vec2(0);
		glm::vec2 size = glm::vec2(-1);
		int32_t z = 0;
		int32_t old_z = 0;

		std::string text = "";

		WidgetStyle style;
		std::function<void()> custom_on_render;
		std::function<void(GUIEventType)> custom_on_event;

		size_t vab_begin = widget_does_not_exist;
	
		bool vab_properties_changed = false;
		bool text_changed = false;
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

	std::unordered_map<widget_t, WidgetInfo> widgets;
	std::map<int32_t, std::vector<widget_t>> z_to_widget_table;

	size_t vab_total_size_in_bytes = 0;
	size_t vab_used_size_in_bytes = 0;
	std::shared_ptr<VertexAttributeBuffer> vab;

	std::shared_ptr<Program> gui_renderer;
	std::shared_ptr<ImmediateRenderer> immediate_renderer;
};
