#include "CortexGUI.h"

#include <queue>

#include "PrimitiveRenderer.h"
#include "WindowBoundGlobalResources.h"
#include "Window.h"

std::filesystem::path gui_renderer_shader_parent_path = "../GraphicsCortex/Source/GLSL/GUI/";

GUI& GUI::get()
{
	if (active_global_resources->GUI == nullptr)
		active_global_resources->GUI = std::unique_ptr<GUI>(new GUI());

	return *active_global_resources->GUI;
}

void GUI::release() {
	active_global_resources->GUI = nullptr;
}

ImmediateRenderer& GUI::get_immediate_renderer()
{
	if (immediate_renderer == nullptr)
		immediate_renderer = std::make_shared<ImmediateRenderer>(1024);
	return *immediate_renderer;
}

Element GUI::create_element()
{
	element_t id = _create_element(invalid_element);
	return Element(attached_window_handle, id);
}

Element GUI::create_element(Element& parent_element)
{
	element_t id = _create_element(parent_element.id);
	return Element(attached_window_handle, id);
}

void GUI::set_element_parent(Element& target_element, Element& new_parent)
{
	_set_element_parent(target_element.id, new_parent.id);
}

void GUI::release_element(Element& element)
{
	if (element.id == invalid_element)
		return;

	_release_element(element.id);
}

bool GUI::does_element_exist(Element& element)
{
	return _does_element_exist(element.id) && element.owner_gui_identifier == attached_window_handle;
}

void GUI::render(Element& root_element)
{
	_render(root_element.id);
}

Widget& GUI::get_widget_data(widget_t id)
{
	if (!_does_widget_exist(id)) {
		ASSERT(false);
		std::cout << "[GUI Error] GUI::get_widget_data() is not permitted to be called with an invalid widget_id: " << id << std::endl;
	}

	return *widgets[id];
}

Window* GUI::get_window()
{
	if (active_window == nullptr) {
		std::cout << "[GUI Error] GUI::get_window() is called but no active windows were present" << std::endl;
		ASSERT(false);
	}
	if (active_window->get_handle() != attached_window_handle) {
		std::cout << "[GUI Error] GUI::get_window() is called active window and GUI::attached_window_handle doesn't match" << std::endl;
		ASSERT(false);
	}
	return active_window;
}

GUI::GUI()
{
	if (active_window == nullptr) {
		std::cout << "[GUI error] GUI cannot be initialized when an active Window is not present" << std::endl;
		ASSERT(false);
	}
	attached_window_handle = active_window->get_handle();
}

void GUI::_compile_shaders() {
	if (shaders_compiled)
		return;

	gui_renderer = std::make_shared<Program>(Shader(gui_renderer_shader_parent_path / "gui_vertex.vert", gui_renderer_shader_parent_path / "gui_geometry.geom", gui_renderer_shader_parent_path / "gui_fragment.frag"));
	gui_renderer_texture = std::make_shared<Program>(Shader(gui_renderer_shader_parent_path / "gui_vertex.vert", gui_renderer_shader_parent_path / "gui_geometry.geom", gui_renderer_shader_parent_path / "gui_fragment_texture.frag"));
	
	shaders_compiled = true;
}

widget_t GUI::_generate_widget_id()
{
	widget_t id = next_element_id;
	next_element_id++;
	return id;
}

void GUI::_release_widget(widget_t widget)
{
	if (!_does_widget_exist(widget))
		return;

	widgets.erase(widget);
}

bool GUI::_does_widget_exist(widget_t widget)
{
	return widgets.find(widget) != widgets.end();
}

element_t GUI::_generate_element_id()
{
	element_t id = next_element_id;
	next_element_id++;
	return id;
}

element_t GUI::_create_element(element_t parent_id)
{
	if (!_does_element_exist(parent_id) && parent_id != invalid_element) {
		std::cout << "[GUI Error] GUI::_create_element() is called with improper parent_id: " << parent_id << std::endl;
		ASSERT(false);
	}

	element_t id = _generate_element_id();
	
	ElementInfo info;
	info.id = id;
	info.parent_id = parent_id;

	elements[id] = info;
	if(_does_element_exist(parent_id))
		elements[parent_id].children.push_back(id);
	
	return id;
}

void GUI::_set_element_parent(element_t target_element, element_t new_parent)
{
	if (!_does_element_exist(target_element)) {
		std::cout << "[GUI Error] GUI::_set_element_parent() is called but target_element is not valid" << std::endl;
		ASSERT(false);
	}

	if (!_does_element_exist(new_parent) && new_parent != invalid_element) {
		std::cout << "[GUI Error] GUI::_set_element_parent() is called but new_parent is not valid" << std::endl;
		ASSERT(false);
	}

	if (elements[target_element].parent_id == new_parent)
		return;


	element_t old_parent = elements[target_element].parent_id;

	elements[target_element].parent_id = new_parent;

	if (old_parent != invalid_element)
		std::erase(elements[old_parent].children, target_element);
	
	if (new_parent != invalid_element)
		elements[new_parent].children.push_back(target_element);
}

void GUI::_release_element(element_t id)
{
	if (!_does_element_exist(id))
		return;

	element_t parent = elements[id].parent_id;
	elements.erase(id);

	if (_does_element_exist(parent)) {
		std::vector<element_t>& children_list = elements[parent].children;
		auto iterator = std::find(children_list.begin(), children_list.end(), id);
		elements[parent].children.erase(iterator);
	}
}

bool GUI::_does_element_exist(element_t id)
{
	return elements.find(id) != elements.end();
}

void GUI::_traverse_children(element_t root_id, std::function<void(element_t, glm::vec2)> lambda)
{
	if (!_does_element_exist(root_id)) {
		std::cout << "[GUI Error] GUI::_traverse_children() is called with invalid root_id" << std::endl;
		ASSERT(false);
	}
	
	std::queue<element_t> nodes;
	std::queue<glm::vec2> positions;
	nodes.push(root_id);
	positions.push(elements[root_id].position);

	while (nodes.size() != 0) {
		
		element_t node = nodes.front();
		glm::vec2 position = positions.front();

		lambda(node, position);

		nodes.pop();
		positions.pop();

		for (element_t child : elements[node].children) {
			nodes.push(child);
			positions.push(position + elements[child].position);
		}
	}
}

namespace {
	glm::vec4 compute_viewports_intersection(const glm::vec4& a, const glm::vec4& b) {
		
		glm::vec4 aabb_a(a.x, a.y, a.x + a.z, a.y + a.w);
		glm::vec4 aabb_b(b.x, b.y, b.x + b.z, b.y + b.w);

		glm::vec4 intersect(
			glm::max(aabb_a.x, aabb_b.x),
			glm::max(aabb_a.y, aabb_b.y),
			glm::min(aabb_a.z, aabb_b.z),
			glm::min(aabb_a.w, aabb_b.w)
		);

		glm::vec4 viewport(
			glm::floor(intersect.x),
			glm::floor(intersect.y),
			glm::ceil(glm::max(intersect.z - intersect.x, 0.0f)),
			glm::ceil(glm::max(intersect.w - intersect.y, 0.0f))
		);

		return viewport;
	}
}

void GUI::_traverse_children_dfs(element_t root_id, std::function<void(element_t, glm::vec2, glm::vec4)> lambda)
{
	if (!_does_element_exist(root_id)) {
		std::cout << "[GUI Error] GUI::_traverse_children() is called with invalid root_id" << std::endl;
		ASSERT(false);
	}

	glm::vec2 total_viewport = primitive_renderer::get_viewport_size();
	glm::vec4 parent_viewport = glm::vec4(
		elements[root_id].position.x,
		total_viewport.y - elements[root_id].position.y - elements[root_id].size.y,
		elements[root_id].size.x,
		elements[root_id].size.y
	);

	std::vector<element_t> nodes;
	std::vector<glm::vec2> positions;
	std::vector<glm::vec4> clip_viewports;
	nodes.push_back(root_id);
	positions.push_back(elements[root_id].position);
	clip_viewports.push_back(parent_viewport);

	while (nodes.size() != 0) {

		element_t node = nodes.back();
		glm::vec2 position = positions.back();
		glm::vec4 clip_viewport = clip_viewports.back();

		lambda(node, position, clip_viewport);

		nodes.pop_back();
		positions.pop_back();
		clip_viewports.pop_back();

		for (element_t child : elements[node].children) {
			glm::vec2 child_position = position + elements[child].position;
			
			glm::vec4 child_viewport = glm::vec4(
				child_position.x,
				total_viewport.y - child_position.y - elements[child].size.y,
				elements[child].size.x,
				elements[child].size.y
			);

			nodes.push_back(child);
			positions.push_back(position + elements[child].position);
			clip_viewports.push_back(compute_viewports_intersection(clip_viewport, child_viewport));
		}
	}
}

void GUI::_traverse_parents(element_t root_id, std::function<void(element_t)> lambda)
{
	if (!_does_element_exist(root_id)) {
		std::cout << "[GUI Error] GUI::_traverse_parents() is called with invalid root_id" << std::endl;
		ASSERT(false);
	}


	element_t node = root_id;

	while (node != invalid_element) {
		lambda(node);
		node = elements[node].parent_id;
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

	vab->push_attribute_format(0, 0,  VertexAttributeBuffer::a_f32, 4, true); // position_size
	vab->push_attribute_format(1, 0,  VertexAttributeBuffer::a_f32, 4, true); // texcoord_begin_end
	vab->push_attribute_format(2, 0,  VertexAttributeBuffer::a_f32, 4, true); // color0
	vab->push_attribute_format(3, 0,  VertexAttributeBuffer::a_f32, 4, true); // color1
	vab->push_attribute_format(4, 0,  VertexAttributeBuffer::a_f32, 4, true); // color2
	vab->push_attribute_format(5, 0,  VertexAttributeBuffer::a_f32, 4, true); // gradient_direction_z_pad
	vab->push_attribute_format(6, 0,  VertexAttributeBuffer::a_f32, 4, true); // border_thickness
	vab->push_attribute_format(7, 0,  VertexAttributeBuffer::a_f32, 4, true); // border_rounding
	vab->push_attribute_format(8, 0,  VertexAttributeBuffer::a_f32, 4, true); // border_color0
	vab->push_attribute_format(9, 0,  VertexAttributeBuffer::a_f32, 4, true); // border_color1
	vab->push_attribute_format(10, 0, VertexAttributeBuffer::a_f32, 4, true); // border_color2
	vab->push_attribute_format(11, 0, VertexAttributeBuffer::a_f32, 4, true); // border_color3
	vab->push_attribute_format(12, 0, VertexAttributeBuffer::a_f32, 4, true); // shadow_thickness
	vab->push_attribute_format(13, 0, VertexAttributeBuffer::a_f32, 4, true); // shadow_color

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

void GUI::_update_vab_to_render(element_t id)
{
	if (!_does_element_exist(id)) {
		std::cout << "[GUI Error] GUI::_update_vab_to_render() is called with improper element_id : " << id << std::endl;
		ASSERT(false);
	}

	std::vector<Vertex> vertices_to_add;

	_traverse_children(id, [&](element_t child_id, glm::vec2 merged_position) {
		ElementInfo& info = elements[child_id];
		bool is_element_present_in_vab = info.vab_begin != ElementInfo::element_does_not_exist;

		if (!is_element_present_in_vab) {

			Vertex vertex;
			vertex.position_size			= glm::vec4(merged_position, info.size);
			vertex.texcoord_begin_end		= glm::vec4(info.texcoord_min, info.texcoord_max);
			vertex.color0					= glm::vec4(info.color0);
			vertex.color1					= glm::vec4(info.color1);
			vertex.color2					= glm::vec4(info.color2);
			vertex.gradient_direction_z_pad	= glm::vec4(glm::vec2(0), -info.z, 0);
			vertex.border_thickness			= glm::vec4(info.border_thickness);
			vertex.border_rounding			= glm::vec4(info.border_rounding);
			vertex.border_color0			= glm::vec4(info.border_color0);
			vertex.border_color1			= glm::vec4(info.border_color1);
			vertex.border_color2			= glm::vec4(info.border_color2);
			vertex.border_color3			= glm::vec4(info.border_color3);
			vertex.shadow_thickness			= glm::vec4(info.shadow_thickness);
			vertex.shadow_color				= glm::vec4(info.shadow_color);

			vertices_to_add.push_back(vertex);
			
			info.vab_begin = vab_used_size_in_bytes + vertices_to_add.size() * sizeof(Vertex);
		}
		});

	_append_data_to_vab(vertices_to_add.data(), vertices_to_add.size() * sizeof(Vertex));
	vertices_to_add.clear();

	_traverse_children(id, [&](element_t child_id, glm::vec2 merged_position) {
		ElementInfo& info = elements[child_id];

		if (info.z != info.old_z) {
			std::erase(z_to_element_table[info.old_z], info.id);
			z_to_element_table[info.z].push_back(info.id);
			info.old_z = info.z;
		}

		if (info.vab_properties_changed) {

			Vertex vertex;
			vertex.position_size			= glm::vec4(merged_position, info.size);
			vertex.texcoord_begin_end		= glm::vec4(info.texcoord_min, info.texcoord_max);
			vertex.color0					= glm::vec4(info.color0);
			vertex.color1					= glm::vec4(info.color1);
			vertex.color2					= glm::vec4(info.color2);
			vertex.gradient_direction_z_pad	= glm::vec4(glm::vec2(0), -info.z, 0);
			vertex.border_thickness			= glm::vec4(info.border_thickness);
			vertex.border_rounding			= glm::vec4(info.border_rounding);
			vertex.border_color0			= glm::vec4(info.border_color0);
			vertex.border_color1			= glm::vec4(info.border_color1);
			vertex.border_color2			= glm::vec4(info.border_color2);
			vertex.border_color3			= glm::vec4(info.border_color3);
			vertex.shadow_thickness			= glm::vec4(info.shadow_thickness);
			vertex.shadow_color				= glm::vec4(info.shadow_color);

			Buffer& buffer = *vab->get_buffer_slot(0).buffer;
			buffer.load_data(info.vab_begin, 0, sizeof(Vertex), &vertex);

			info.vab_properties_changed = false;
		}
		});
}

void GUI::_render_tmp(element_t id)
{
	if (!_does_element_exist(id)) {
		std::cout << "[GUI Error] GUI::_render_tmp() is called with improper element_id : " << id << std::endl;
		ASSERT(false);
	}

	ImmediateRenderer& immediate = get_immediate_renderer();
	glm::vec2 total_viewport = primitive_renderer::get_viewport_size();

	glm::vec4 parent_viewport = glm::vec4(
		elements[id].position.x,
		total_viewport.y - elements[id].position.y - elements[id].size.y,
		elements[id].size.x,
		elements[id].size.y
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

	_traverse_children(id, [&](element_t child_id, glm::vec2 merged_position) {
		
		int32_t z = elements[child_id].z;

		immediate.set_fill_color(elements[child_id].color0);
		immediate.draw_quad(
			glm::vec3(merged_position + glm::vec2(0, 0),						-z),
			glm::vec3(merged_position + glm::vec2(0, elements[child_id].size.y),	-z),
			glm::vec3(merged_position + glm::vec2(elements[child_id].size),		-z),
			glm::vec3(merged_position + glm::vec2(elements[child_id].size.x, 0),	-z)
		);

		RenderParameters params(true);
		params.scissor_test = true;

		params.scissor_viewport = parent_viewport;
		immediate.render(camera, params);

		parent_viewport = glm::vec4(
			elements[child_id].position.x,
			total_viewport.y - elements[child_id].position.y - elements[child_id].size.y,
			elements[child_id].size.x,
			elements[child_id].size.y
		);

		});
}


void GUI::_render(element_t id)
{
	if (!_does_element_exist(id)) {
		std::cout << "[GUI Error] GUI::_render() is called with improper element_id : " << id << std::endl;
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

	if (elements[id].size.x < 0 && elements[id].parent_id == invalid_element)
		elements[id].size.x = total_viewport.x;
	if (elements[id].size.y < 0 && elements[id].parent_id == invalid_element)
		elements[id].size.y = total_viewport.y;

	if (glm::any(glm::lessThan(elements[id].size, glm::vec2(0)))) {
		std::cout << "[GUI Error] GUI::_render() is called on a child widget with a negative size, widgets with sizes relative to other widgets cannot be the root of a _render() call" << std::endl;
		ASSERT(false);
	}
	
	if (glm::any(glm::equal(elements[id].size, glm::vec2(0))))
		return;

	_update_vab_to_render(id);

	struct render_data {
		element_t id;
		glm::vec2 merged_position;
		glm::vec4 clip_viewport;
	};
	std::map<int32_t, std::vector<render_data>> sorted_children;

	_traverse_children_dfs(id, [&](element_t child_id, glm::vec2 merged_position, glm::vec4 clip_viewport) {
		render_data data;
		data.id = child_id;
		data.merged_position = merged_position;
		data.clip_viewport = clip_viewport;
		sorted_children[elements[child_id].z].push_back(data);
		});

	for (auto it = sorted_children.begin(); it != sorted_children.end(); it++) {
		int32_t z = it->first;
		for (render_data& data : it->second) {
			element_t& child_id = data.id;
			glm::vec2& merged_position = data.merged_position;
			glm::vec4& clip_viewport = data.clip_viewport;

			bool element_is_textured = elements[child_id].texture != nullptr;

			Program& renderer = element_is_textured ? *gui_renderer_texture : *gui_renderer;

			if (element_is_textured)
				renderer.update_uniform("source_texture", *elements[child_id].texture);

			RenderParameters params(true);
			params.depth_function = RenderParameters::DepthStencilFunction::LEQUAL;
			params.scissor_test = true;
			params.scissor_viewport = clip_viewport;

			//element_t parent_id = elements[child_id].parent_id;
			//if (parent_id != invalid_element) {
			//
			//	glm::vec4 parent_viewport = glm::vec4(
			//		elements[parent_id].position.x,
			//		total_viewport.y - elements[parent_id].position.y - elements[parent_id].size.y,
			//		elements[parent_id].size.x,
			//		elements[parent_id].size.y
			//	);
			//	params.scissor_test = true;
			//	params.scissor_viewport = parent_viewport;
			//}

			primitive_renderer::render(
				renderer,
				*vab,
				PrimitiveType::point,
				params,
				elements[child_id].vab_begin / sizeof(Vertex),
				1,
				1,
				0
			);
		}
	}

	primitive_renderer::set_viewport(previous_viewport);
}
