#include "CortexGUI.h"

#include <queue>

#include "PrimitiveRenderer.h"
#include "WindowBoundGlobalResources.h"

std::filesystem::path gui_renderer_shader_parent_path = "../GraphicsCortex/Source/GLSL/GUI/";

GUI& GUI::get()
{
	if (active_global_resources->GUI == nullptr)
		active_global_resources->GUI = std::unique_ptr<GUI>(new GUI());

	return *active_global_resources->GUI;
}

ImmediateRenderer& GUI::get_immediate_renderer()
{
	if (immediate_renderer == nullptr)
		immediate_renderer = std::make_shared<ImmediateRenderer>(1024);
	return *immediate_renderer;
}

Widget GUI::create_widget()
{
	widget_t id = _create_widget(invalid_widget);
	return Widget(attached_window_handle, id);
}

Widget GUI::create_widget(Widget& parent_widget)
{
	widget_t id = _create_widget(parent_widget.id);
	return Widget(attached_window_handle, id);
}

void GUI::set_widget_parent(Widget& target_widget, Widget& new_parent)
{
	_set_widget_parent(target_widget.id, new_parent.id);
}

void GUI::release_widget(Widget& widget)
{
	if (widget.id == invalid_widget)
		return;

	_release_widget(widget.id);
}

bool GUI::does_widget_exist(Widget& widget)
{
	return _does_widget_exist(widget.id) && widget.owner_gui_identifier == attached_window_handle;
}

void GUI::render(Widget& root_widget)
{
	_render(root_widget.id);
}

GUI::GUI()
{
}

void GUI::_compile_shaders() {
	if (shaders_compiled)
		return;

	gui_renderer = std::make_shared<Program>(Shader(gui_renderer_shader_parent_path / "gui_vertex.vert", gui_renderer_shader_parent_path / "gui_fragment.frag"));
	gui_renderer_texture = std::make_shared<Program>(Shader(gui_renderer_shader_parent_path / "gui_vertex.vert", gui_renderer_shader_parent_path / "gui_fragment_texture.frag"));
	
	shaders_compiled = true;
}

widget_t GUI::_generate_widget_id()
{
	widget_t id = next_widget_id;
	next_widget_id++;
	return id;
}

widget_t GUI::_create_widget(widget_t parent_id)
{
	if (!_does_widget_exist(parent_id) && parent_id != invalid_widget) {
		std::cout << "[GUI Error] GUI::_create_widget() is called with improper parent_id: " << parent_id << std::endl;
		ASSERT(false);
	}

	widget_t id = _generate_widget_id();
	
	WidgetInfo info;
	info.id = id;
	info.parent_id = parent_id;

	widgets[id] = info;
	if(_does_widget_exist(parent_id))
		widgets[parent_id].children.push_back(id);
	
	return id;
}

void GUI::_set_widget_parent(widget_t target_widget, widget_t new_parent)
{
	if (!_does_widget_exist(target_widget)) {
		std::cout << "[GUI Error] GUI::_set_widget_parent() is called but target_widget is not valid" << std::endl;
		ASSERT(false);
	}

	if (!_does_widget_exist(new_parent) && new_parent != invalid_widget) {
		std::cout << "[GUI Error] GUI::_set_widget_parent() is called but new_parent is not valid" << std::endl;
		ASSERT(false);
	}

	if (widgets[target_widget].parent_id == new_parent)
		return;


	widget_t old_parent = widgets[target_widget].parent_id;

	widgets[target_widget].parent_id = new_parent;

	if (old_parent != invalid_widget)
		std::erase(widgets[old_parent].children, target_widget);
	
	if (new_parent != invalid_widget)
		widgets[new_parent].children.push_back(target_widget);
}

void GUI::_release_widget(widget_t id)
{
	if (!_does_widget_exist(id))
		return;

	widget_t parent = widgets[id].parent_id;
	widgets.erase(id);

	if (_does_widget_exist(parent)) {
		std::vector<widget_t>& children_list = widgets[parent].children;
		auto iterator = std::find(children_list.begin(), children_list.end(), id);
		widgets[parent].children.erase(iterator);
	}
}

bool GUI::_does_widget_exist(widget_t id)
{
	return widgets.find(id) != widgets.end();
}

void GUI::_traverse_children(widget_t root_id, std::function<void(widget_t, glm::vec2)> lambda)
{
	if (!_does_widget_exist(root_id)) {
		std::cout << "[GUI Error] GUI::_traverse_children() is called with invalid root_id" << std::endl;
		ASSERT(false);
	}
	
	std::queue<widget_t> nodes;
	std::queue<glm::vec2> positions;
	nodes.push(root_id);
	positions.push(widgets[root_id].position);

	while (nodes.size() != 0) {
		
		widget_t node = nodes.front();
		glm::vec2 position = positions.front();

		lambda(node, position);

		nodes.pop();
		positions.pop();

		for (widget_t child : widgets[node].children) {
			nodes.push(child);
			positions.push(position + widgets[child].position);
		}
	}
}

void GUI::_traverse_parents(widget_t root_id, std::function<void(widget_t)> lambda)
{
	if (!_does_widget_exist(root_id)) {
		std::cout << "[GUI Error] GUI::_traverse_parents() is called with invalid root_id" << std::endl;
		ASSERT(false);
	}


	widget_t node = root_id;

	while (node != invalid_widget) {
		lambda(node);
		node = widgets[node].parent_id;
	}
}

void GUI::_init_vab(size_t begin_size_in_bytes)
{
	if (vab != nullptr)
		return;

	vab = std::make_shared<VertexAttributeBuffer>();
	vab->attach_vertex_buffer(
		0, 
		std::make_shared<Buffer>(begin_size_in_bytes), 
		sizeof(Vertex),
		0, 
		0
	);

	vab->push_attribute_format(0, 0, VertexAttributeBuffer::a_f32, 3, true); // position
	vab->push_attribute_format(1, 0, VertexAttributeBuffer::a_f32, 2, true); // texture_coord
	vab->push_attribute_format(2, 0, VertexAttributeBuffer::a_f32, 4, true); // color

	vab_total_size_in_bytes = begin_size_in_bytes;
	vab_used_size_in_bytes = 0;
}

void GUI::_resize_vab(size_t new_size_in_bytes)
{
	if (vab_total_size_in_bytes == new_size_in_bytes)
		return;

	if (new_size_in_bytes < vab_total_size_in_bytes) {
		std::cout << "[GUI Error] GUI::_resize_vab() is called but shrinking vab through GUI::_resize_vab() is not permitted, use GUI::_consolidate_vab() instead" << std::endl;
		ASSERT(false);
	}

	auto old_vab = vab;
	size_t old_used_size = vab_used_size_in_bytes;

	vab = nullptr;
	_init_vab(new_size_in_bytes);

	if (old_vab == nullptr)
		return;

	Buffer& old_buffer = *old_vab->get_buffer_slot(0).buffer;
	Buffer& new_buffer = *vab->get_buffer_slot(0).buffer;

	old_buffer.copy_to(new_buffer);
	
	vab_used_size_in_bytes = old_used_size;
	vab_total_size_in_bytes = new_size_in_bytes;
}

void GUI::_append_data_to_vab(void* data, size_t data_size_in_bytes)
{
	_init_vab();

	size_t size_needed = vab_used_size_in_bytes + data_size_in_bytes;

	if (size_needed > vab_total_size_in_bytes) {
		size_t size_to_resize = vab_total_size_in_bytes;
		
		while (size_to_resize < size_needed)
			size_to_resize *= 2;

		_resize_vab(size_to_resize);
	}

	Buffer& buffer = *vab->get_buffer_slot(0).buffer;
	buffer.load_data(vab_used_size_in_bytes, 0, data_size_in_bytes, data);

	vab_used_size_in_bytes += data_size_in_bytes;
}

void GUI::_update_vab_to_render(widget_t id)
{
	if (!_does_widget_exist(id)) {
		std::cout << "[GUI Error] GUI::_update_vab_to_render() is called with improper widget_id : " << id << std::endl;
		ASSERT(false);
	}

	std::vector<Vertex> vertices_to_add;

	_traverse_children(id, [&](widget_t child_id, glm::vec2 merged_position) {
		WidgetInfo& info = widgets[child_id];
		bool is_widget_present_in_vab = info.vab_begin != WidgetInfo::widget_does_not_exist;

		if (!is_widget_present_in_vab) {
			glm::vec3 position0 = glm::vec3(merged_position + glm::vec2(0, 0),				-info.z);
			glm::vec3 position1 = glm::vec3(merged_position + glm::vec2(0, info.size.y),	-info.z);
			glm::vec3 position2 = glm::vec3(merged_position + glm::vec2(info.size),			-info.z);
			glm::vec3 position3 = glm::vec3(merged_position + glm::vec2(info.size.x, 0),	-info.z);

			glm::vec2 texcoord0(info.texcoord_min.x, info.texcoord_max.y);
			glm::vec2 texcoord1(info.texcoord_min.x, info.texcoord_min.y);
			glm::vec2 texcoord2(info.texcoord_max.x, info.texcoord_min.y);
			glm::vec2 texcoord3(info.texcoord_max.x, info.texcoord_max.y);

			glm::vec4 color0(info.color);
			glm::vec4 color1(info.color);
			glm::vec4 color2(info.color);
			glm::vec4 color3(info.color);

			vertices_to_add.push_back(Vertex(position0, texcoord0, color0));
			vertices_to_add.push_back(Vertex(position1, texcoord1, color1));
			vertices_to_add.push_back(Vertex(position2, texcoord2, color2));
			vertices_to_add.push_back(Vertex(position0, texcoord0, color0));
			vertices_to_add.push_back(Vertex(position2, texcoord2, color2));
			vertices_to_add.push_back(Vertex(position3, texcoord3, color3));

			info.vab_begin = vab_used_size_in_bytes + vertices_to_add.size() * sizeof(Vertex);
		}
		});

	_append_data_to_vab(vertices_to_add.data(), vertices_to_add.size() * sizeof(Vertex));
	vertices_to_add.clear();

	_traverse_children(id, [&](widget_t child_id, glm::vec2 merged_position) {
		WidgetInfo& info = widgets[child_id];

		if (info.z != info.old_z) {
			std::erase(z_to_widget_table[info.old_z], info.id);
			z_to_widget_table[info.z].push_back(info.id);
			info.old_z = info.z;
		}

		if (info.vab_properties_changed) {

			glm::vec3 position0 = glm::vec3(merged_position + glm::vec2(0, 0),				-info.z);
			glm::vec3 position1 = glm::vec3(merged_position + glm::vec2(0, info.size.y),	-info.z);
			glm::vec3 position2 = glm::vec3(merged_position + glm::vec2(info.size),			-info.z);
			glm::vec3 position3 = glm::vec3(merged_position + glm::vec2(info.size.x, 0),	-info.z);

			glm::vec2 texcoord0(info.texcoord_min.x, info.texcoord_max.y);
			glm::vec2 texcoord1(info.texcoord_min.x, info.texcoord_min.y);
			glm::vec2 texcoord2(info.texcoord_max.x, info.texcoord_min.y);
			glm::vec2 texcoord3(info.texcoord_max.x, info.texcoord_max.y);

			glm::vec4 color0(info.color);
			glm::vec4 color1(info.color);
			glm::vec4 color2(info.color);
			glm::vec4 color3(info.color);

			Vertex vertices[6];
			vertices[0] = Vertex(position0, texcoord0, color0);
			vertices[1] = Vertex(position1, texcoord1, color1);
			vertices[2] = Vertex(position2, texcoord2, color2);
			vertices[3] = Vertex(position0, texcoord0, color0);
			vertices[4] = Vertex(position2, texcoord2, color2);
			vertices[5] = Vertex(position3, texcoord3, color3);

			Buffer& buffer = *vab->get_buffer_slot(0).buffer;
			buffer.load_data(info.vab_begin, 0, 6 * sizeof(Vertex), vertices);

			info.vab_properties_changed = false;
		}
		});
}

void GUI::_render_tmp(widget_t id)
{
	if (!_does_widget_exist(id)) {
		std::cout << "[GUI Error] GUI::_render_tmp() is called with improper widget_id : " << id << std::endl;
		ASSERT(false);
	}

	ImmediateRenderer& immediate = get_immediate_renderer();
	glm::vec2 total_viewport = primitive_renderer::get_viewport_size();

	glm::vec4 parent_viewport = glm::vec4(
		widgets[id].position.x,
		total_viewport.y - widgets[id].position.y - widgets[id].size.y,
		widgets[id].size.x,
		widgets[id].size.y
	);
	
	Camera camera;
	camera.projection_matrix = glm::ortho(
		0.0f,
		total_viewport.x,
		total_viewport.y,
		0.0f,
		total_number_of_z_layers / 2.0f,
		-total_number_of_z_layers / 2.0f
	);
	camera.view_matrix = glm::identity<glm::mat4>();

	_traverse_children(id, [&](widget_t child_id, glm::vec2 merged_position) {
		
		int32_t z = widgets[child_id].z;

		immediate.set_fill_color(widgets[child_id].color);
		immediate.draw_quad(
			glm::vec3(merged_position + glm::vec2(0, 0),						-z),
			glm::vec3(merged_position + glm::vec2(0, widgets[child_id].size.y),	-z),
			glm::vec3(merged_position + glm::vec2(widgets[child_id].size),		-z),
			glm::vec3(merged_position + glm::vec2(widgets[child_id].size.x, 0),	-z)
		);

		RenderParameters params(true);
		params.scissor_test = true;

		params.scissor_viewport = parent_viewport;
		immediate.render(camera, params);

		parent_viewport = glm::vec4(
			widgets[child_id].position.x,
			total_viewport.y - widgets[child_id].position.y - widgets[child_id].size.y,
			widgets[child_id].size.x,
			widgets[child_id].size.y
		);

		});
}


void GUI::_render(widget_t id)
{
	if (!_does_widget_exist(id)) {
		std::cout << "[GUI Error] GUI::_render() is called with improper widget_id : " << id << std::endl;
		ASSERT(false);
	}
	
	_compile_shaders();

	glm::vec4 previous_viewport = primitive_renderer::get_viewport_position_size();
	glm::vec2 total_viewport = primitive_renderer::get_viewport_size();

	glm::mat4 projection_matrix = glm::ortho(
		0.0f,
		total_viewport.x,
		total_viewport.y,
		0.0f,
		total_number_of_z_layers / 2.0f,
		-total_number_of_z_layers / 2.0f
	);

	gui_renderer->update_uniform("projection", projection_matrix);
	gui_renderer_texture->update_uniform("projection", projection_matrix);

	_update_vab_to_render(id);

	struct render_data {
		widget_t id;
		glm::vec2 merged_position;
	};
	std::map<int32_t, std::vector<render_data>> sorted_children;

	_traverse_children(id, [&](widget_t child_id, glm::vec2 merged_position) {
		render_data data;
		data.id = child_id;
		data.merged_position = merged_position;
		sorted_children[widgets[child_id].z].push_back(data);
		});

	for (auto it = sorted_children.begin(); it != sorted_children.end(); it++) {
		int32_t z = it->first;
		for (render_data& data : it->second) {
			widget_t& child_id = data.id;
			glm::vec2& merged_position = data.merged_position;

			bool widget_is_textured = widgets[child_id].texture != nullptr;

			Program& renderer = widget_is_textured ? *gui_renderer_texture : *gui_renderer;

			if (widget_is_textured)
				renderer.update_uniform("source_texture", *widgets[child_id].texture);

			RenderParameters params(true);
			params.depth_function = RenderParameters::DepthStencilFunction::LEQUAL;

			widget_t parent_id = widgets[child_id].parent_id;
			if (parent_id != invalid_widget) {

				glm::vec4 parent_viewport = glm::vec4(
					widgets[parent_id].position.x,
					total_viewport.y - widgets[parent_id].position.y - widgets[parent_id].size.y,
					widgets[parent_id].size.x,
					widgets[parent_id].size.y
				);
				params.scissor_test = true;
				params.scissor_viewport = parent_viewport;
			}

			primitive_renderer::render(
				renderer,
				*vab,
				PrimitiveType::triangle,
				params,
				widgets[child_id].vab_begin / sizeof(Vertex),
				6,
				1,
				0
			);
		}
	}

	primitive_renderer::set_viewport(previous_viewport);
}

GUI::Vertex::Vertex(
	glm::vec3 pos,
	glm::vec2 texcoord,
	glm::vec4 color
) :
	position(pos),
	texcoord(texcoord),
	color(color)
{
}