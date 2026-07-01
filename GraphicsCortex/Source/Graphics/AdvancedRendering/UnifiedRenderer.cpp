#include "UnifiedRenderer.h"
#include "WindowBoundGlobalResources.h"
#include "PrimitiveRenderer.h"
#include <gtc/packing.hpp>
#include <functional>

std::filesystem::path unified_renderer_shader_parent_path = "../GraphicsCortex/Source/GLSL/AdvancedRendering/UnifiedRenderer/";

UnifiedRenderer& UnifiedRenderer::get()
{
	if (active_global_resources->UnifiedRenderer == nullptr)
		active_global_resources->UnifiedRenderer = std::unique_ptr<UnifiedRenderer>(new UnifiedRenderer());

	return *active_global_resources->UnifiedRenderer;
}

uint32_t UnifiedRenderer::HandleGenerator::generate_id() {
	uint32_t id;
	if (!free_ids.empty()) {
		id = free_ids.back();
		free_ids.pop_back();
		free_id_set.erase(id);
	}
	else {
		id = next_id_to_generate++;
	}
	return id;
}

void UnifiedRenderer::HandleGenerator::free_id(uint32_t old_id) {
	
	if (old_id >= next_id_to_generate || old_id == UnifiedRenderer::invalid_id) return;

	if (free_id_set.find(old_id) != free_id_set.end()) {
		return;
	}

	free_ids.push_back(old_id);
	free_id_set.insert(old_id);
}

uint32_t UnifiedRenderer::create_render_pass(RenderParameters parameters)
{
	uint32_t render_pass = render_pass_handle_generator.generate_id();
	RenderPassInfo info;
	info.parameters		= parameters;
	passes[render_pass] = info;
	object_material_buffer[render_pass];
	return render_pass;
}

uint32_t UnifiedRenderer::create_render_pass(Shader shader, RenderParameters parameters)
{
	uint32_t render_pass = render_pass_handle_generator.generate_id();
	RenderPassInfo info;
	info.set_shader(shader);
	info.parameters		= parameters;
	passes[render_pass] = info;
	object_material_buffer[render_pass];
	return render_pass;
}

void UnifiedRenderer::release_render_pass(uint32_t render_pass_id)
{
	passes.erase(render_pass_id);
	object_material_buffer.erase(render_pass_id);
	render_pass_handle_generator.free_id(render_pass_id);
}

UnifiedRenderer::RenderPassInfo& UnifiedRenderer::get_render_pass(uint32_t render_pass_id)
{
	if (passes.find(render_pass_id) == passes.end()) {
		std::cout << "[UnifiedRenderer Error] get_render_pass() is called with an id that doesn't exist" << std::endl;
		ASSERT(false);
	}
	auto& pass = passes.at(render_pass_id);
	pass.owner = this;
	return pass;
}

void UnifiedRenderer::iterate_render_passes(std::function<void(uint32_t, RenderPassInfo&)> lambda)
{
	for (auto& pass : passes) {
		lambda(pass.first, pass.second);
	}
}

uint32_t UnifiedRenderer::create_texture(std::shared_ptr<Texture2D> texture)
{
	uint32_t texture_id = texture_handle_generator.generate_id();
	TextureInfo info;
	info.owner = this;
	info.set_texture(texture); // for bindless check
	textures[texture_id] = info;
	return texture_id;
}

uint32_t UnifiedRenderer::create_texture(glm::vec4 flat_color)
{
	uint32_t texture_id = texture_handle_generator.generate_id();
	TextureInfo info;
	info.owner		= this;
	info.color		= flat_color;
	textures[texture_id] = info;
	return texture_id;
}

void UnifiedRenderer::release_texture(uint32_t texture_id)
{
	textures.erase(texture_id);
	texture_handle_generator.free_id(texture_id);
}

UnifiedRenderer::TextureInfo& UnifiedRenderer::get_texture(uint32_t texture_id)
{
	if (textures.find(texture_id) == textures.end()) {
		std::cout << "[UnifiedRenderer Error] get_texture() is called with an id that doesn't exist" << std::endl;
		ASSERT(false);
	}
	auto& texture = textures.at(texture_id);
	texture.owner = this;
	return texture;
}

void UnifiedRenderer::iterate_textures(std::function<void(uint32_t, TextureInfo&)> lambda)
{
	for (auto& texture : textures) {
		lambda(texture.first, texture.second);
	}
}

uint32_t UnifiedRenderer::create_material()
{
	uint32_t material = material_handle_generator.generate_id();
	MaterialInfo info;
	info.owner = this;
	materials[material] = info;	
	return material;
}

void UnifiedRenderer::release_material(uint32_t material_id)
{
	materials.erase(material_id);
	material_handle_generator.free_id(material_id);
}

UnifiedRenderer::MaterialInfo& UnifiedRenderer::get_material(uint32_t material_id)
{
	if (materials.find(material_id) == materials.end()) {
		std::cout << "[UnifiedRenderer Error] get_material() is called with an id that doesn't exist" << std::endl;
		ASSERT(false);
	}
	auto& material = materials.at(material_id);
	material.owner = this;
	return material;
}

void UnifiedRenderer::iterate_materials(std::function<void(uint32_t, MaterialInfo&)> lambda)
{
	for (auto& material : materials) {
		lambda(material.first, material.second);
	}
}

uint32_t UnifiedRenderer::create_mesh(PrimitiveType mesh_primitive)
{
	uint32_t mesh = mesh_handle_generator.generate_id();
	MeshInfo info;
	info.owner = this;
	info.primitive	= mesh_primitive;
	info.self_id	= mesh;
	meshes[mesh] = info;
	return mesh;
}

void UnifiedRenderer::release_mesh(uint32_t mesh_id)
{
	meshes.erase(mesh_id);
	mesh_handle_generator.free_id(mesh_id);
}

UnifiedRenderer::MeshInfo& UnifiedRenderer::get_mesh(uint32_t mesh_id)
{
	if (meshes.find(mesh_id) == meshes.end()) {
		std::cout << "[UnifiedRenderer Error] get_mesh() is called with an id that doesn't exist" << std::endl;
		ASSERT(false);
	}
	auto& mesh = meshes.at(mesh_id);
	mesh.owner = this;
	return mesh;
}

void UnifiedRenderer::iterate_meshes(std::function<void(uint32_t, MeshInfo&)> lambda)
{
	for (auto& mesh : meshes) {
		lambda(mesh.first, mesh.second);
	}
}

uint32_t UnifiedRenderer::create_object(uint32_t mesh_id, glm::mat4 model_matrix, uint32_t parent_object_id)
{
	uint32_t object = object_handle_generator.generate_id();
	ObjectInfo info;
	info.owner = this;
	info.transform	= model_matrix;
	info.mesh		= mesh_id;
	info.self_id	= object;
	info.set_parent(parent_object_id);	// for child, sibling management
	objects[object] = info;
	return object;
}

void UnifiedRenderer::release_object(uint32_t object_id)
{
	objects.erase(object_id);
	object_handle_generator.free_id(object_id);
}

UnifiedRenderer::ObjectInfo& UnifiedRenderer::get_object(uint32_t object_id)
{
	if (objects.find(object_id) == objects.end()) {
		std::cout << "[UnifiedRenderer Error] get_objcet() is called with an id that doesn't exist" << std::endl;
		ASSERT(false);
	}
	auto& object = objects.at(object_id);
	object.owner = this;
	return object;
}

void UnifiedRenderer::iterate_objects(std::function<void(uint32_t, ObjectInfo&)> lambda)
{
	for (auto& object : objects) {
		lambda(object.first, object.second);
	}
}

uint32_t UnifiedRenderer::create_render_layer() {
	uint32_t layer = render_layer_handle_generator.generate_id();
	RenderLayerInfo info;
	info.owner = this;
	layers[layer] = info;
	return layer;
}

void UnifiedRenderer::release_render_layer(uint32_t render_layer_id) {
	layers.erase(render_layer_id);
	render_layer_handle_generator.free_id(render_layer_id);
}

UnifiedRenderer::RenderLayerInfo& UnifiedRenderer::get_render_layer(uint32_t render_layer_id) {
	if (layers.find(render_layer_id) == layers.end()) {
		std::cout << "[UnifiedRenderer Error] get_render_layer() is called with an id that doesn't exist" << std::endl;
		ASSERT(false);
	}
	auto& layer = layers.at(render_layer_id);
	layer.owner = this;
	return layer;

}

void UnifiedRenderer::iterate_render_layers(std::function<void(uint32_t, RenderLayerInfo&)> lambda) {
	for (auto& layer : layers) {
		lambda(layer.first, layer.second);
	}
}

UnifiedRenderer::VertexFormatData::VertexFormatData()
{
	for (int32_t i = 0; i < sizeof(attributes) / sizeof(uint32_t); i++)
		set_attribute(i, disabled, 0);
}

size_t UnifiedRenderer::VertexFormatHasher::operator()(const VertexFormatData& format) const
{
	size_t hash_val = 0;
	const uint32_t* data_ptr = reinterpret_cast<const uint32_t*>(&format);
	size_t count = sizeof(UnifiedRenderer::VertexFormatData) / sizeof(uint32_t);

	for (size_t i = 0; i < count; ++i) {
		hash_val ^= std::hash<uint32_t>()(data_ptr[i]) + 0x9e3779b9 + (hash_val << 6) + (hash_val >> 2);
	}
	return hash_val;
}

PrimitiveType UnifiedRenderer::MeshInfo::get_primitive()
{
	return primitive;
}

void UnifiedRenderer::MeshInfo::set_primitive(PrimitiveType primitive)
{
	std::cout << "[UnifiedRenderer Error] not implemented" << std::endl;
	ASSERT(false);
	//if (this->primitive == primitive)
	//	return;
	//this->primitive = primitive;
	//is_up_to_date = false;
	//owner->up_to_date = false;
	// different primitives aren't supported yet
}

void UnifiedRenderer::MeshInfo::allocate_vertices(size_t size_in_bytes)
{
	if (vertex_size_in_bytes == size_in_bytes)
		return;

	if (vertex_begin_v != DynamicBuffer::invalid_address)
		owner->unified_vertex_buffer.buffer.gpu_free(vertex_begin_v);

	vertex_begin_v			= owner->unified_vertex_buffer.buffer.gpu_malloc(size_in_bytes);
	vertex_size_in_bytes	= size_in_bytes;

	is_mesh_up_to_date = false;
	is_aabb_up_to_date = false;
	owner->a_mesh_modified = true;
}

void UnifiedRenderer::MeshInfo::allocate_indices(size_t size_in_bytes)
{
	if (index_size_in_bytes == size_in_bytes)
		return;

	if (index_begin_v != DynamicBuffer::invalid_address)
		owner->unified_index_buffer.buffer.gpu_free(index_begin_v);

	index_begin_v			= owner->unified_index_buffer.buffer.gpu_malloc(size_in_bytes);
	index_size_in_bytes		= size_in_bytes;

	is_mesh_up_to_date = false;
	is_aabb_up_to_date = false;
	owner->a_mesh_modified = true;
}

size_t UnifiedRenderer::MeshInfo::get_index_begin()
{
	return owner->unified_index_buffer.buffer.get_offset(index_begin_v);
}

size_t UnifiedRenderer::MeshInfo::get_index_size_in_bytes()
{
	return index_size_in_bytes;
}

size_t UnifiedRenderer::MeshInfo::get_vertex_begin()
{
	return owner->unified_vertex_buffer.buffer.get_offset(vertex_begin_v);
}

size_t UnifiedRenderer::MeshInfo::get_vertex_size_in_bytes()
{
	return vertex_size_in_bytes;
}

void UnifiedRenderer::MeshInfo::load_vertices(void* data, size_t size_in_bytes, size_t offset_in_bytes) {

	if (data == nullptr) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::MeshInfo::load_vertices() is called with nullptr" << std::endl;
		ASSERT(false);
	}

	if (size_in_bytes + offset_in_bytes > this->vertex_size_in_bytes) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::MeshInfo::load_vertices() is called with out of bound data" << std::endl;
		ASSERT(false);
	}

	owner->unified_vertex_buffer.buffer.get_buffer().load_data(offset_in_bytes, 0, size_in_bytes, data);

	is_aabb_up_to_date = false;
	owner->a_mesh_modified = true;
}

void UnifiedRenderer::MeshInfo::load_indices(void* data, size_t size_in_bytes, size_t offset_in_bytes) {

	if (data == nullptr) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::MeshInfo::load_indices() is called with nullptr" << std::endl;
		ASSERT(false);
	}

	if (size_in_bytes + offset_in_bytes > this->index_size_in_bytes) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::MeshInfo::load_indices() is called with out of bound data" << std::endl;
		ASSERT(false);
	}

	owner->unified_index_buffer.buffer.get_buffer().load_data(offset_in_bytes, 0, size_in_bytes, data);

	is_aabb_up_to_date = false;
	owner->a_mesh_modified = true;
}

void UnifiedRenderer::MeshInfo::get_vertices(void* data, size_t size_in_bytes, size_t offset_in_bytes) {

	if (data == nullptr) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::MeshInfo::get_vertices() is called with nullptr" << std::endl;
		ASSERT(false);
	}

	if (size_in_bytes + offset_in_bytes > this->vertex_size_in_bytes) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::MeshInfo::get_vertices() is called with out of bound data" << std::endl;
		ASSERT(false);
	}

	owner->unified_vertex_buffer.buffer.get_buffer().get_data(offset_in_bytes, 0, size_in_bytes, data);

}

void UnifiedRenderer::MeshInfo::get_indices(void* data, size_t size_in_bytes, size_t offset_in_bytes) {
	
	if (data == nullptr) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::MeshInfo::get_indices() is called with nullptr" << std::endl;
		ASSERT(false);
	}

	if (size_in_bytes + offset_in_bytes > this->vertex_size_in_bytes) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::MeshInfo::get_indices() is called with out of bound data" << std::endl;
		ASSERT(false);
	}

	owner->unified_index_buffer.buffer.get_buffer().get_data(offset_in_bytes, 0, size_in_bytes, data);

}

void UnifiedRenderer::MeshInfo::clear_vertices(uint8_t value, size_t size_in_bytes, size_t offset_in_bytes) {

	if (size_in_bytes + offset_in_bytes > this->vertex_size_in_bytes) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::MeshInfo::get_vertices() is called with out of bound data" << std::endl;
		ASSERT(false);
	}

	owner->unified_vertex_buffer.buffer.get_buffer().clear(offset_in_bytes, size_in_bytes, value);

	is_aabb_up_to_date = false;
	owner->a_mesh_modified = true;
}

void UnifiedRenderer::MeshInfo::clear_indices(uint8_t value, size_t size_in_bytes, size_t offset_in_bytes) {
	
	if (size_in_bytes + offset_in_bytes > this->vertex_size_in_bytes) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::MeshInfo::clear_indices() is called with out of bound data" << std::endl;
		ASSERT(false);
	}

	owner->unified_index_buffer.buffer.get_buffer().clear(offset_in_bytes, size_in_bytes, value);

	is_aabb_up_to_date = false;
	owner->a_mesh_modified = true;
}

AABB3 UnifiedRenderer::MeshInfo::get_aabb() {

	if (owner->mesh_buffer.get_buffer().get_buffer_size_in_bytes() < (self_id + 1) * sizeof(MeshData)) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::MeshInfo::get_aabb_min() is called but data doesn't exist in gpu" << std::endl;
		ASSERT(false);
	}

	MeshData data;
	owner->mesh_buffer.get_buffer().get_data(self_id * sizeof(MeshData), 0, sizeof(MeshData), &data);

	return AABB3(data.aabb_min, data.aabb_max);
}

void UnifiedRenderer::MeshInfo::invalidate_aabb()
{
	is_aabb_up_to_date = false;
	owner->a_mesh_modified = true;
}

void UnifiedRenderer::MeshInfo::set_vertex_stride(uint32_t stride_in_bytes)
{
	if (local_format.stride_in_bytes == stride_in_bytes)
		return;
	local_format.stride_in_bytes = stride_in_bytes;
	is_format_up_to_date = false;
	is_aabb_up_to_date = false;
	owner->a_mesh_modified = true;
}

uint32_t UnifiedRenderer::MeshInfo::get_vertex_stride()
{
	return local_format.stride_in_bytes;
}

void UnifiedRenderer::VertexFormatData::set_attribute(int32_t location, AttributeType type, uint16_t byte_offset)
{
	attributes[location] = (static_cast<uint32_t>(type) << 16) | byte_offset;

	if (type != disabled)
		active_attributes_mask |= (1 << location);
	else
		active_attributes_mask &= ~(1 << location);
}

bool UnifiedRenderer::VertexFormatData::operator==(const VertexFormatData& other) const
{
	if (stride_in_bytes != other.stride_in_bytes) return false;
	for (int32_t i = 0; i < sizeof(attributes) / sizeof(uint32_t); i++)
		if (attributes[i] != other.attributes[i]) return false;
	return true;
}

void UnifiedRenderer::MeshInfo::set_attribute(int32_t location, AttributeType type, uint16_t byte_offset) {
	local_format.set_attribute(location, type, byte_offset);
	is_aabb_up_to_date = false;
	is_format_up_to_date = false;
	owner->a_mesh_modified = true;
}

UnifiedRenderer::AttributeType UnifiedRenderer::MeshInfo::get_attributte_type(int32_t location)
{
	return (AttributeType)(local_format.attributes[location] >> 16);
}

uint16_t UnifiedRenderer::MeshInfo::get_attribute_offset(int32_t location)
{
	return (AttributeType)(local_format.attributes[location] & 0xffffull);
}

void UnifiedRenderer::TextureInfo::set_texture(texture_t texture) {
	if (texture->is_bindless == false) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::TextureInfo::set_texture() is called with a texture that is not bindless" << std::endl;
		ASSERT(false);
	}
	if (this->texture == texture)
		return;
	this->texture = texture;
	is_up_to_date = false;
	owner->a_texture_modified = true;
}

UnifiedRenderer::TextureInfo::texture_t UnifiedRenderer::TextureInfo::get_texture() {
	return texture;
}

void UnifiedRenderer::TextureInfo::set_color(glm::vec4 color) {
	if (this->color == color)
		return;
	this->color = color;
	is_up_to_date = false;
	owner->a_texture_modified = true;
}

glm::vec4 UnifiedRenderer::TextureInfo::get_color() {
	return color;
}

void UnifiedRenderer::MaterialInfo::push_back(uint32_t texture_id) {
	textures.push_back(texture_id);
	is_up_to_date = false;
	owner->a_material_modified = true;
}

void UnifiedRenderer::MaterialInfo::insert(uint32_t index, uint32_t texture_id) {
	textures.insert(textures.begin() + index, texture_id);
	is_up_to_date = false;
	owner->a_material_modified = true;
}

void UnifiedRenderer::MaterialInfo::erase(uint32_t index) {
	textures.erase(textures.begin() + index),
	is_up_to_date = false;
	owner->a_material_modified = true;
}

uint32_t UnifiedRenderer::MaterialInfo::find(uint32_t texture_id) {
	for (int32_t i = 0; i < textures.size(); i++)
		if (textures[i] == texture_id)
			return i;
	return -1;
}

uint32_t UnifiedRenderer::MaterialInfo::at(uint32_t index) {
	if (index >= textures.size()) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::MaterialInfo::at() is called with out of bounds index" << std::endl;
		ASSERT(false);
	}
	return textures[index];
}

uint32_t UnifiedRenderer::MaterialInfo::back() {
	if (textures.size() == 0) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::MaterialInfo::back() is called but no elements exist" << std::endl;
		ASSERT(false);
	}
	return textures.back();
}

size_t UnifiedRenderer::MaterialInfo::size() {
	return textures.size();
}

void UnifiedRenderer::ObjectInfo::set_transform(glm::mat4 model_matrix) {
	if (transform == model_matrix)
		return;
	transform = model_matrix;
	is_transform_up_to_date = false;
	owner->a_transform_modified = true;
}

glm::mat4 UnifiedRenderer::ObjectInfo::get_transform() {
	return transform;
}

void UnifiedRenderer::ObjectInfo::set_mesh(uint32_t mesh) {
	if (this->mesh == mesh)
		return;
	this->mesh = mesh;
	is_mesh_up_to_date = false;
	owner->an_object_modified = true;
}

uint32_t UnifiedRenderer::ObjectInfo::get_mesh() {
	return mesh;
}

void UnifiedRenderer::ObjectInfo::set_parent(uint32_t parent_id) {
	if (this->parent == parent_id)
		return;
	
	// remove from old parent
	if (this->parent != invalid_id) {
		if (owner->objects.find(this->parent) == owner->objects.end()) {
			std::cout << "[UnifiedRenderer Error] UnifiedRenderer::ObjectInfo::set_parent() is called but given old parent doesn't exist" << std::endl;
			ASSERT(false);
		}
		ObjectInfo& parent = owner->objects.at(this->parent);
		
		if (parent.child == self_id) {
			parent.child = this->sibling;
		}
		else {
			uint32_t sibling_id = parent.child;
			while (sibling_id != invalid_id) {
				if (owner->objects.find(sibling_id) == owner->objects.end()) {
					std::cout << "[UnifiedRenderer Error] UnifiedRenderer::ObjectInfo::set_parent() is called but a sibling doesn't exist" << std::endl;
					ASSERT(false);
				}

				ObjectInfo& sibling = owner->objects.at(sibling_id);

				if (sibling.sibling == self_id) {
					sibling.sibling = this->sibling;
					break;
				}

				sibling_id = sibling.sibling;
			}
		}
	}

	this->sibling = invalid_id;
	this->parent = parent_id;

	// insert itself as new parent's child
	if (parent_id != invalid_id) {
		if (owner->objects.find(parent_id) == owner->objects.end()) {
			std::cout << "[UnifiedRenderer Error] UnifiedRenderer::ObjectInfo::set_parent() is called but given parent_id doesn't exist" << std::endl;
			ASSERT(false);
		}
		ObjectInfo& parent = owner->objects.at(parent_id);
		
		if (parent.child == invalid_id)
		{
			parent.child = self_id;
		}
		else {
			uint32_t sibling_id = parent.child;
			while (true) {

				if (owner->objects.find(sibling_id) == owner->objects.end()) {
					std::cout << "[UnifiedRenderer Error] UnifiedRenderer::ObjectInfo::set_parent() is called but a sibling doesn't exist" << std::endl;
					ASSERT(false);
				}

				ObjectInfo& sibling = owner->objects.at(sibling_id);

				if (sibling.sibling == invalid_id) {
					sibling.sibling = self_id;
					break;
				}

				sibling_id = sibling.sibling;
			}
		}
		
	}

	is_transform_up_to_date = false;
	owner->a_transform_modified = true;
}

uint32_t UnifiedRenderer::ObjectInfo::get_parent() {
	return parent;
}

uint32_t UnifiedRenderer::ObjectInfo::get_child() {
	return child;
}

uint32_t UnifiedRenderer::ObjectInfo::get_sibling() {
	return sibling;
}

void UnifiedRenderer::RenderLayerInfo::object_add(uint32_t object_id, uint32_t material_id) {
	if (object_id_to_index.find(object_id) != object_id_to_index.end()) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::RenderLayerInfo::add_object() is called but given object is already added" << std::endl;
		ASSERT(false);
	}

	ObjectMaterialData data{
		.object_id = object_id,
		.material_id = material_id,
	};
	object_id_to_index[object_id] = objects.size();
	objects.push_back(data);
	
	is_up_to_date = false;
	owner->a_render_layer_modified = true;
}

void UnifiedRenderer::RenderLayerInfo::object_remove(uint32_t object_id) {
	if (object_id_to_index.find(object_id) == object_id_to_index.end())
		return;

	size_t index = object_id_to_index.at(object_id);
	
	objects.erase(objects.begin() + index);
	
	for (size_t i = index; i < objects.size(); i++) {
		if (object_id_to_index.find(objects[i].object_id) == object_id_to_index.end())
		{
			std::cout << "[UnifiedRenderer Error] UnifiedRenderer::RenderLayerInfo::object_remove() is called but an inconsistany occured within the data structure" << std::endl;
			ASSERT(false);
		}

		object_id_to_index[objects[i].object_id] -= 1;
	}

	is_up_to_date = false;
	owner->a_render_layer_modified = true;
}

bool UnifiedRenderer::RenderLayerInfo::object_does_exist(uint32_t object_id) {
	return object_id_to_index.find(object_id) != object_id_to_index.end();
}

uint32_t UnifiedRenderer::RenderLayerInfo::object_get_material(uint32_t object_id) {
	if (object_id_to_index.find(object_id) == object_id_to_index.end())
	{
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::RenderLayerInfo::object_get_material() is called but given object_id doesn't exist" << std::endl;
		ASSERT(false);
	}
	
	size_t index = object_id_to_index.at(object_id);

	if (index >= objects.size()) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::RenderLayerInfo::object_set_material() is called but an internal inconsistancy occurred" << std::endl;
		ASSERT(false);
	}

	return objects[index].material_id;
}

void UnifiedRenderer::RenderLayerInfo::object_set_material(uint32_t object_id, uint32_t new_material) {
	if (object_id_to_index.find(object_id) == object_id_to_index.end())
	{
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::RenderLayerInfo::object_get_material() is called but given object_id doesn't exist" << std::endl;
		ASSERT(false);
	}

	size_t index = object_id_to_index.at(object_id);

	if (index >= objects.size()) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::RenderLayerInfo::object_set_material() is called but an internal inconsistancy occurred" << std::endl;
		ASSERT(false);
	}

	objects[index].material_id = new_material;
	
	is_up_to_date = false;
	owner->a_render_layer_modified = true;
}

size_t UnifiedRenderer::RenderLayerInfo::object_count() {
	return objects.size();
}

void UnifiedRenderer::RenderPassInfo::set_render_layer(uint32_t render_layer_id) {
	render_layer = render_layer_id;
	is_up_to_date = false;
	owner->a_render_pass_modified = true;
}

uint32_t UnifiedRenderer::RenderPassInfo::get_render_layer() {
	return render_layer;
}

uint32_t UnifiedRenderer::RenderPassInfo::get_required_attributes_mask() {
	return required_attributes_mask;
}

void UnifiedRenderer::RenderPassInfo::require_attribute(int32_t location, GenericType generic_type, std::string attribute_name, bool flat) {
	if (location < 0 || location >= 16) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::RenderPassInfo::require_attribute() is called with a location out of bound. it must be between 0 and 16" << std::endl;
		ASSERT(false);
	}
	if (location == 0) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::RenderPassInfo::require_attribute() is called but location 0 is reserved for position, custom attribute on location 0 is not allowed" << std::endl;
		ASSERT(false);
	}

	required_attributes_mask |= (1 << location);
	attribute_infos[location] = AttributeInfo {
		.variable_name = attribute_name,
		.generic_type = generic_type,
		.is_flat = flat,
	};
}

bool UnifiedRenderer::RenderPassInfo::is_mesh_compatible(uint32_t mesh_mask) {
	return (mesh_mask & required_attributes_mask) == required_attributes_mask;
}

void UnifiedRenderer::RenderPassInfo::set_parameters(RenderParameters new_parameters) {
	parameters = new_parameters;
}

RenderParameters UnifiedRenderer::RenderPassInfo::get_parameters() {
	return parameters;
}

bool UnifiedRenderer::RenderPassInfo::is_shader_set()
{
	return varianted_program != nullptr;
}

void UnifiedRenderer::RenderPassInfo::set_shader(Shader shader) {
	if (varianted_program == nullptr)
		varianted_program = std::make_shared<VariantedProgram>();
	varianted_program->set_shader(shader);
}

namespace {
	
	UnifiedRenderer::GenericType generic_type_to_expended_generic_type(UnifiedRenderer::GenericType generic_type) {
		switch (generic_type) {
		case UnifiedRenderer::float_t:	return UnifiedRenderer::vec4_t;
		case UnifiedRenderer::vec2_t:	return UnifiedRenderer::vec4_t;
		case UnifiedRenderer::vec3_t:	return UnifiedRenderer::vec4_t;
		case UnifiedRenderer::vec4_t:	return UnifiedRenderer::vec4_t;
		case UnifiedRenderer::uint_t:	return UnifiedRenderer::uvec4_t;
		case UnifiedRenderer::uvec2_t:	return UnifiedRenderer::uvec4_t;
		case UnifiedRenderer::uvec4_t:	return UnifiedRenderer::uvec4_t;
		}

		ASSERT(false);
	}

	UnifiedRenderer::GenericType attribute_type_to_generic_type(UnifiedRenderer::AttributeType attribute_type) {
		switch (attribute_type) {
		case UnifiedRenderer::disabled:				{ ASSERT(false); };
		case UnifiedRenderer::f32_1:				return UnifiedRenderer::float_t;
		case UnifiedRenderer::f32_2:				return UnifiedRenderer::vec2_t;
		case UnifiedRenderer::f32_3:				return UnifiedRenderer::vec3_t;
		case UnifiedRenderer::f32_4:				return UnifiedRenderer::vec4_t;
		case UnifiedRenderer::f16_2:				return UnifiedRenderer::vec2_t;
		case UnifiedRenderer::f16_4:				return UnifiedRenderer::vec4_t;
		case UnifiedRenderer::unorm8x4:				return UnifiedRenderer::vec4_t;
		case UnifiedRenderer::unorm16x2:			return UnifiedRenderer::vec2_t;
		case UnifiedRenderer::unorm16x4:			return UnifiedRenderer::vec4_t;
		case UnifiedRenderer::snorm8x4:				return UnifiedRenderer::vec4_t;
		case UnifiedRenderer::snorm16x2:			return UnifiedRenderer::vec2_t;
		case UnifiedRenderer::snorm16x4:			return UnifiedRenderer::vec4_t;
		case UnifiedRenderer::snorm10_10_10_2:		return UnifiedRenderer::vec3_t;
		case UnifiedRenderer::u8_4:					return UnifiedRenderer::uvec4_t;
		case UnifiedRenderer::u16_4:				return UnifiedRenderer::uvec4_t;
		case UnifiedRenderer::u32_1:				return UnifiedRenderer::uint_t;
		}

		ASSERT(false);
	}

	std::string attribute_type_to_string(UnifiedRenderer::AttributeType attribute_type) {
		switch (attribute_type) {
		case UnifiedRenderer::disabled:				{ ASSERT(false); };
		case UnifiedRenderer::f32_1:				return "f32_1";
		case UnifiedRenderer::f32_2:				return "f32_2";
		case UnifiedRenderer::f32_3:				return "f32_3";
		case UnifiedRenderer::f32_4:				return "f32_4";
		case UnifiedRenderer::f16_2:				return "f16_2";
		case UnifiedRenderer::f16_4:				return "f16_4";
		case UnifiedRenderer::unorm8x4:				return "unorm8x4";
		case UnifiedRenderer::unorm16x2:			return "unorm16x2";
		case UnifiedRenderer::unorm16x4:			return "unorm16x4";
		case UnifiedRenderer::snorm8x4:				return "snorm8x4";
		case UnifiedRenderer::snorm16x2:			return "snorm16x2";
		case UnifiedRenderer::snorm16x4:			return "snorm16x4";
		case UnifiedRenderer::snorm10_10_10_2:		return "snorm10_10_10_2";
		case UnifiedRenderer::u8_4:					return "u8_4";
		case UnifiedRenderer::u16_4:				return "u16_4";
		case UnifiedRenderer::u32_1:				return "u32_1";
		}

		ASSERT(false);
	}

	std::string generic_type_to_glsl_type(UnifiedRenderer::GenericType generic_type) {
		switch (generic_type) {
		case UnifiedRenderer::float_t:	return "float";
        case UnifiedRenderer::vec2_t:	return "vec2";
        case UnifiedRenderer::vec3_t:	return "vec3";
        case UnifiedRenderer::vec4_t:	return "vec4";
        case UnifiedRenderer::uint_t:	return "uint";
        case UnifiedRenderer::uvec2_t:	return "uvec2";
		case UnifiedRenderer::uvec4_t:	return "uvec4";
		}

		ASSERT(false);
	}
}

UnifiedRenderer::RenderPassInfo::program_t UnifiedRenderer::RenderPassInfo::get_program() {
	if (varianted_program == nullptr) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::RenderPassInfo::get_program() is called but no shader was set beforehand" << std::endl;
		ASSERT(false);
	}

	varianted_program->begin_variant();

	std::string variables_definitions;
	for (int32_t i = 1; i < 16; i++) {
		
		if ((required_attributes_mask & (1 << i)) == 0) continue;

		GenericType generic_type			= attribute_infos[i].generic_type;
		GenericType expended_generic_type	= generic_type_to_expended_generic_type(generic_type);

		std::string expended_type_name	= generic_type_to_glsl_type(expended_generic_type);
		std::string type_name		= generic_type_to_glsl_type(generic_type);

		std::string variable_name	= attribute_infos[i].variable_name;
		std::string fetch_function	= "fetch_" + expended_type_name + "(addr, type)";

		// fetch generic expended type and cast it to attribute spesific type
		varianted_program->variant_define("PULL_ATTRIBUTE_" + std::to_string(i), 
			"{ " + variable_name + " = " + type_name + "(" + fetch_function + "); }"
		);

		bool is_flat = attribute_infos[i].is_flat;

		variables_definitions +=  (is_flat ? "out flat " : "out ") + type_name + " " + variable_name + "; ";
	}

	varianted_program->variant_define("custom_output_variables", variables_definitions);

	return varianted_program->get_current_variant();
}

void UnifiedRenderer::update_parent_object_matricies()
{
	if (!a_transform_modified) return;

	std::vector<uint32_t> stack;

	for (auto& [object_id, object] : objects) {
		if (!object.is_transform_up_to_date) {

			bool has_dirty_ancestor = false;
			uint32_t ancestor_id = object.parent;

			// Go up the chain to check if any parent is also dirty
			while (ancestor_id != invalid_id) {
				if (objects.find(ancestor_id) == objects.end()) {
					std::cout << "[UnifiedRenderer Error] update_parent_object_matricies() is called but a parent_id doesn not exist" << std::endl;
					ASSERT(false);
				}

				if (!objects.at(ancestor_id).is_transform_up_to_date) {
					has_dirty_ancestor = true;
					break;
				}
				ancestor_id = objects.at(ancestor_id).parent;
			}

			if (!has_dirty_ancestor) {
				stack.push_back(object_id);
			}
		}
	}

	while (!stack.empty()) {
		uint32_t current_id = stack.back();
		stack.pop_back();

		ObjectInfo& current = objects.at(current_id);

		if (current.parent == invalid_id) {
			current.model_matrix = current.transform;
		}
		else {
			if (objects.find(current.parent) == objects.end()) {
				std::cout << "[UnifiedRenderer Error] update_parent_object_matricies() is called but a parent_id doesn not exist" << std::endl;
				ASSERT(false);
			}
			ObjectInfo& parent = objects.at(current.parent);
			current.model_matrix = parent.model_matrix * current.transform;
		}

		current.is_transform_up_to_date = true;
		current.is_mesh_up_to_date		= false;	// force write to vram in later stage

		uint32_t child_id = current.child;
		while (child_id != invalid_id) {
			if (objects.find(child_id) == objects.end()) {
				std::cout << "[UnifiedRenderer Error] update_parent_object_matricies() is called but a child_id doesn not exist" << std::endl;
				ASSERT(false);
			}
			stack.push_back(child_id);
			child_id = objects.at(child_id).sibling;
		}
	}

	a_transform_modified = false;
}


void UnifiedRenderer::update_meshes()
{
	bool vb_invalidated	= unified_vertex_buffer.buffer.get_epoch() != unified_vertex_buffer.last_updated_epoch;
	bool ib_invalidated	= unified_index_buffer.buffer.get_epoch() != unified_index_buffer.last_updated_epoch;

	if (vb_invalidated || ib_invalidated) {
		a_mesh_modified = true;
		for (auto& [mesh_id, mesh] : meshes)
			mesh.is_mesh_up_to_date = false;
	}
	
	if (!a_mesh_modified) return;

	for (auto& [mesh_id, mesh] : meshes) {
		if (mesh.is_mesh_up_to_date && mesh.is_format_up_to_date) continue;

		if (!mesh.is_format_up_to_date) {
			
			if (format_to_id.find(mesh.local_format) == format_to_id.end())
				format_to_id[mesh.local_format] = vertex_format_handle_generator.generate_id();

			uint32_t format_id = format_to_id.at(mesh.local_format);

			vertex_format_buffer.require((format_id+1) * sizeof(VertexFormatData));
			vertex_format_buffer.get_buffer().load_data(format_id * sizeof(VertexFormatData), 0, sizeof(VertexFormatData), &mesh.local_format);

			mesh.format_id = format_id;
			
			mesh.is_format_up_to_date= true;
			mesh.is_mesh_up_to_date = false;	// Force mesh struct to update because format_id changed
		}

		if (!mesh.is_mesh_up_to_date) {

			MeshData data;
			data.vertex_begin	= mesh.get_vertex_begin();
			data.vertex_count	= mesh.vertex_size_in_bytes / mesh.local_format.stride_in_bytes;
			data.index_begin	= mesh.get_index_begin();
			data.index_count	= mesh.get_index_size_in_bytes() / sizeof(uint32_t);
			data.format_id		= mesh.format_id;
			data.aabb_min		= glm::vec4(glm::vec3(+std::numeric_limits<float>::max()), -1);
			data.aabb_max		= glm::vec4(glm::vec3(-std::numeric_limits<float>::max()), -1);

			mesh_buffer.require((mesh_id+1) * sizeof(MeshData));
			mesh_buffer.get_buffer().load_data(mesh_id * sizeof(MeshData), 0, sizeof(MeshData), &data);

			mesh.is_mesh_up_to_date = true;
		}
	}

	//a_mesh_modified = false; will be reset on the aabb_generation step
}

void UnifiedRenderer::update_materials()
{
	bool t_invalidated = texture_buffer.last_updated_epoch != texture_buffer.buffer.get_epoch();

	if (t_invalidated) {
		for (auto& [material_id, material] : materials) {
			material.is_up_to_date = false;
		}
		a_material_modified = true;
	}

	if (a_texture_modified && !t_invalidated) {
		for (auto& [material_id, material] : materials) {

			for (uint32_t tex_id : material.textures) {

				if (textures.find(tex_id) != textures.end()) {
					if (!textures.at(tex_id).is_up_to_date) {
						material.is_up_to_date = false;
						a_material_modified = true;
						break;
					}
				}
				else if (tex_id != invalid_id)
				{
					std::cout << "[UnifiedRenderer Error] UnifiedRenderer::update_materials() is called but a material references a texture_id that doesn't exist" << std::endl;
					ASSERT(false);
				}
			}
		}

		a_texture_modified = false;
	}

	if (!a_material_modified) return;

	for (auto& [material_id, material] : materials) {
		if (material.is_up_to_date) continue;

		size_t required_texture_size = material.textures.size() * sizeof(TextureData);

		if (required_texture_size > 0) {
			// only allocate/reallocate vram but defer loading later for possible defragmentation
			if (material.textures_address_v == DynamicBuffer::invalid_address) {
				material.textures_address_v = texture_buffer.buffer.gpu_malloc(required_texture_size);
			}
			else if (texture_buffer.buffer.get_size(material.textures_address_v) < required_texture_size) { // maybe try "!=" rather than "<"
				texture_buffer.buffer.gpu_free(material.textures_address_v);
				material.textures_address_v = texture_buffer.buffer.gpu_malloc(required_texture_size);
			}
		}
		else {
			if (material.textures_address_v != DynamicBuffer::invalid_address) {
				texture_buffer.buffer.gpu_free(material.textures_address_v);
				material.textures_address_v = DynamicBuffer::invalid_address;
			}
		}
	}

	for (auto& [material_id, material] : materials) {

		if (material.is_up_to_date) continue;

		// 1. Resolve offset and upload MaterialData safely
		MaterialData mat_data;

		mat_data.texture_id_begin =
			(material.textures_address_v != DynamicBuffer::invalid_address)
			? (uint32_t)(texture_buffer.buffer.get_offset(material.textures_address_v) / sizeof(TextureData))
			: invalid_id;

		mat_data.texture_count = (uint32_t)material.textures.size();

		material_buffer.require((material_id + 1) * sizeof(MaterialData));
		material_buffer.get_buffer().load_data(material_id * sizeof(MaterialData), 0, sizeof(MaterialData), &mat_data);

		std::vector<TextureData> texture_data;
		texture_data.reserve(material.textures.size());

		for (uint32_t texture_id : material.textures) {
			if (textures.find(texture_id) == textures.end()) {
				std::cout << "[UnifiedRenderer Error] update_materials() called but a referenced texture_id doesn't exist" << std::endl;
				ASSERT(false);
			}

			TextureInfo& texture_info = textures.at(texture_id);

			TextureData data;
			if (texture_info.texture != nullptr)
				std::memcpy(&data.handle, &texture_info.texture->texture_handle, sizeof(glm::uvec2));

			data.color.x = glm::packHalf2x16(glm::vec2(texture_info.color.r, texture_info.color.g));
			data.color.y = glm::packHalf2x16(glm::vec2(texture_info.color.b, texture_info.color.a));

			texture_data.push_back(data);
		}

		size_t required_texture_size = material.textures.size() * sizeof(TextureData);
		if (required_texture_size > 0) {
			size_t tex_offset = texture_buffer.buffer.get_offset(material.textures_address_v);
			texture_buffer.buffer.get_buffer().load_data(tex_offset, 0, required_texture_size, texture_data.data());
		}

		material.is_up_to_date = true;
	}

	for (auto& [tex_id, texture] : textures) {
		texture.is_up_to_date = true;
	}

	a_material_modified = false;
}

void UnifiedRenderer::update_objects()
{
	if (!an_object_modified) return;

	for (auto& [object_id, object] : objects) {
		if (object.is_mesh_up_to_date) continue;

		if (meshes.find(object.mesh) == meshes.end()) {
			std::cout << "[UnifiedRenderer Error] UnifiedRenderer::update_objects() is called but an object references a mesh_id that doesn't exist" << std::endl;
			ASSERT(false);
		}

		ObjectData data;
		data.model_matrix		= object.model_matrix;
		data.aabb_min_mesh_id	= glm::vec4(glm::vec3(std::numeric_limits<float>::max()), glm::uintBitsToFloat(object.mesh));
		data.aabb_max			= glm::vec4(glm::vec3(std::numeric_limits<float>::max()), -1);
			
		object_buffer.require((object_id+1) * sizeof(ObjectData));
		object_buffer.get_buffer().load_data(object_id * sizeof(ObjectData), 0, sizeof(ObjectData), &data);

		object.is_mesh_up_to_date		= true;
	}

	an_object_modified = false;
}

void UnifiedRenderer::update_render_passes() {

	if (a_render_layer_modified) {
		for (auto& [pass_id, pass] : passes) {
			
			if (pass.render_layer == invalid_id || layers.find(pass.render_layer) == layers.end()) {
				std::cout << "[UnifiedRenderer Error] UnifiedRenderer::update_render_passes() is called but a render pass references a render layer that doesn't exist" << std::endl;
				ASSERT(false);
			}

			RenderLayerInfo& layer = layers.at(pass.render_layer);
			
			if (!layer.is_up_to_date) {
				pass.is_up_to_date = false;
				layer.is_up_to_date = true;
				a_render_layer_modified = true;
			}
		}
	}

	a_render_layer_modified = false;

	if (!a_render_pass_modified) return;

	for (auto& [pass_id, pass] : passes) {
		if (pass.is_up_to_date) continue;

		RenderLayerInfo& layer = layers.at(pass.render_layer);

		size_t required_space = layer.objects.size() * sizeof(ObjectMaterialData);

		// check for material/object id validity here or at PassInfo add/remove

		if (required_space > 0) {
			object_material_buffer.at(pass_id).require(required_space);
			object_material_buffer[pass_id].get_buffer().load_data(0, 0, required_space, layer.objects.data());
		}

		pass.is_up_to_date = true;
	}

	a_render_pass_modified = false;
}

void UnifiedRenderer::compile_shaders() {
	if (are_shaders_compiled) return;

	cp_compute_aabb						= std::make_shared<ComputeProgram>(Shader(unified_renderer_shader_parent_path / "compute_aabb.comp"));
	cp_cull_and_generate_draw_commands	= std::make_shared<ComputeProgram>(Shader(unified_renderer_shader_parent_path / "cull_and_generate_draw_commands.comp"));

	are_shaders_compiled = true;
}

void UnifiedRenderer::compute_mesh_aabbs()
{
	if (!a_mesh_modified) return;

	compile_shaders();

	ComputeProgram& kernel = *cp_compute_aabb;

	struct MeshJob {
		uint32_t mesh_id = invalid_id;
		uint32_t chunk_start_index = 0;
	};

	std::vector<MeshJob> aabb_computation_jobs;
	uint32_t workgroup_counter = 0;
	uint32_t workgroup_size = kernel.get_work_group_size().x;

	for (auto& [mesh_id, mesh] : meshes) {
		if (!mesh.is_aabb_up_to_date) {

			aabb_computation_jobs.push_back(MeshJob{
				.mesh_id = mesh_id,
				.chunk_start_index = workgroup_counter
				});

			size_t vertex_count = mesh.vertex_size_in_bytes / mesh.local_format.stride_in_bytes;
			workgroup_counter += (vertex_count + (workgroup_size - 1)) / workgroup_size;

			mesh.is_aabb_up_to_date = true;
		}
	}
	
	mesh_job_buffer.require_and_shrink(sizeof(MeshJob)* aabb_computation_jobs.size());
	mesh_job_buffer.get_buffer().load_data(0, 0, sizeof(MeshJob)* aabb_computation_jobs.size(), aabb_computation_jobs.data());
	

	kernel.update_uniform_as_storage_buffer("mesh_job_buffer",			mesh_job_buffer.get_buffer());
	kernel.update_uniform_as_storage_buffer("mesh_buffer",				mesh_buffer.get_buffer());
	kernel.update_uniform_as_storage_buffer("format_buffer",			vertex_format_buffer.get_buffer());
	kernel.update_uniform_as_storage_buffer("unified_vertex_buffer",	unified_vertex_buffer.buffer.get_buffer());
	kernel.update_uniform("job_count", (uint32_t)aabb_computation_jobs.size());

	kernel.dispatch(workgroup_counter, 1, 1);

	a_mesh_modified = false;
}

void UnifiedRenderer::update_structures()
{
	update_parent_object_matricies();
	update_meshes();
	update_materials();
	update_objects();
	update_render_passes();
	compute_mesh_aabbs();

	unified_vertex_buffer.last_updated_epoch = unified_vertex_buffer.buffer.get_epoch();
	unified_index_buffer.last_updated_epoch = unified_index_buffer.buffer.get_epoch();
	texture_buffer.last_updated_epoch = texture_buffer.buffer.get_epoch();
}

void UnifiedRenderer::generate_command_buffer(uint32_t render_pass, const glm::mat4& projection_view_matrix, RenderOrder order)
{
	compile_shaders();

	if (order != unordered) {
		std::cout << "[UnifiedRenderer Error] generate_command_buffer with ordering is not supported" << std::endl;
		ASSERT(false);
	}

	if (passes.find(render_pass) == passes.end()) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::generate_command_buffer() is called but given render_pass doesn't exist" << std::endl;
		ASSERT(false);
	}

	RenderPassInfo& pass = passes.at(render_pass);
	
	if (layers.find(pass.render_layer) == layers.end()) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::generate_command_buffer() is called but given render_pass references a render_layer that doesn't exist" << std::endl;
		ASSERT(false);
	}
	
	RenderLayerInfo& layer = layers.at(pass.render_layer);

	if (layer.objects.empty()) return;

	std::array<glm::vec4, 6> frustum_planes;
	
	glm::mat4 tvp = glm::transpose(projection_view_matrix);

	frustum_planes[0] = tvp[3] + tvp[0]; // Left
	frustum_planes[1] = tvp[3] - tvp[0]; // Right
	frustum_planes[2] = tvp[3] + tvp[1]; // Bottom
	frustum_planes[3] = tvp[3] - tvp[1]; // Top
	frustum_planes[4] = tvp[3] + tvp[2]; // Near (OpenGL [-1, 1] Z clip space)
	frustum_planes[5] = tvp[3] - tvp[2]; // Far

	for (int i = 0; i < 6; ++i) {
		frustum_planes[i] /= glm::length(glm::vec3(frustum_planes[i]));
	}

	uint32_t object_count = (uint32_t)layer.objects.size();
	command_buffer[render_pass].require(object_count * sizeof(DrawElementsIndirectCommand));

	if (draw_command_counter == nullptr)
		draw_command_counter = std::make_shared<Buffer>(sizeof(uint32_t));

	draw_command_counter->clear((uint32_t)0);
	
	ComputeProgram& kernel = *cp_cull_and_generate_draw_commands;

	kernel.update_uniform_as_storage_buffer("mesh_buffer",				mesh_buffer.get_buffer()); // because object buffer's aabb is not up to date yet in this implementation
	kernel.update_uniform_as_storage_buffer("object_buffer",			object_buffer.get_buffer());
	kernel.update_uniform_as_storage_buffer("object_material_buffer",	object_material_buffer[render_pass].get_buffer());

	kernel.update_uniform_as_storage_buffer("draw_command_counter",		*draw_command_counter);
	kernel.update_uniform_as_storage_buffer("pass_command_buffer",		command_buffer[render_pass].get_buffer());

	kernel.update_uniform("total_pass_objects", object_count);
	for (int32_t i = 0; i < frustum_planes.size(); i++)
		kernel.update_uniform("frustum_planes[" + std::to_string(i) + "]", frustum_planes[i]);

	kernel.dispatch_thread(object_count, 1, 1);
}

void UnifiedRenderer::render(uint32_t render_pass, const Camera& camera, RenderOrder order)
{
	update_structures();

	glm::mat4 projection_view_matrix = camera.projection_matrix * camera.view_matrix;

	generate_command_buffer(render_pass, projection_view_matrix, order);

	if (render_pass == invalid_id || passes.find(render_pass) == passes.end()) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::render() is called but given render_pass_id doesn't exist" << std::endl;
		ASSERT(false);
	}
	
	RenderPassInfo& pass = passes.at(render_pass);

	if (layers.find(pass.render_layer) == layers.end()) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::render() is called but given render_pass references a render_layer that doesn't exist" << std::endl;
		ASSERT(false);
	}

	RenderLayerInfo& layer = layers.at(pass.render_layer);

	if (layer.objects.empty()) return;

	if (command_buffer.find(render_pass) == command_buffer.end()) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::render() is called but a command_buffer for the render_pass haven't created yet" << std::endl;
		ASSERT(false);
	}

	if (draw_command_counter == nullptr) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::render() is called but a draw_command_counter for the render_pass haven't created yet" << std::endl;
		ASSERT(false);
	}

	if (!pass.is_shader_set()) {
		std::cout << "[UnifiedRenderer Error] UnifiedRenderer::render() is called but a shader for the render_pass haven't specified yet" << std::endl;
		ASSERT(false);
	}
	
	Program& program = *pass.get_program();

	program.update_uniform_as_storage_buffer("mesh_buffer",				mesh_buffer.get_buffer());
	program.update_uniform_as_storage_buffer("object_buffer",			object_buffer.get_buffer());
	program.update_uniform_as_storage_buffer("object_material_buffer",	object_material_buffer.at(render_pass).get_buffer());
	program.update_uniform_as_storage_buffer("format_buffer",			vertex_format_buffer.get_buffer());
	program.update_uniform_as_storage_buffer("vertex_buffer",			unified_vertex_buffer.buffer.get_buffer());

	program.update_uniform_as_storage_buffer("material_buffer",			material_buffer.get_buffer());
	program.update_uniform_as_storage_buffer("texture_buffer",			texture_buffer.buffer.get_buffer());

	program.update_uniform("projection_view_matrix", projection_view_matrix);

	primitive_renderer::render(
		program,
		blank_vab,
		unified_index_buffer.buffer.get_buffer(),
		command_buffer.at(render_pass).get_buffer(),
		*draw_command_counter,
		PrimitiveType::triangle, IndexType::i_ui32,
		pass.parameters,
		0, 0, 0,
		0
	);
}