#include <algorithm>
#include <iostream>
#include <stdint.h>

#include "Model.h"
#include "Debuger.h"
#include "Buffer.h"
#include "VertexAttributeBuffer.h"
#include "Mesh.h"

Model::Model(Model&& other) :
	single_models(std::move(other.single_models)),
	index_buffer_type(other.index_buffer_type),
	_name_to_node(std::move(other._name_to_node)),
	_next_node_name(other._next_node_name)
{

}

Model& Model::operator=(Model&& other)
{
	std::swap(single_models, other.single_models);
	std::swap(index_buffer_type, other.index_buffer_type);
	std::swap(_name_to_node, other._name_to_node);
	std::swap(_next_node_name, other._next_node_name);

	return *this;
}

void Model::clear()
{
	_next_node_name = root_node_name;
	single_models.clear();
	_name_to_node.clear();
	index_buffer_type = IndexType::i_ui32;
}

bool Model::does_model_exist(model_t model_name)
{
	return (model_name >= 0) && (model_name < single_models.size());
}

size_t Model::get_model_count() {
	return single_models.size();
}

model_t Model::add_model(const SingleModel& single_model)
{
	single_models.push_back(single_model);
	return single_models.size() - 1;
}

model_t Model::add_model(SingleModel&& single_model)
{
	single_models.emplace_back(single_model);
	return single_models.size() - 1;
}

SingleModel* Model::get_model(model_t submodel_name)
{
	if (!does_model_exist(submodel_name))
		return nullptr;

	return &single_models[submodel_name];
}

std::span<SingleModel> Model::get_models()
{
	return std::span<SingleModel>(single_models);
}

void Model::set_index_type(IndexType type)
{
	index_buffer_type = type;
}

IndexType Model::get_index_type()
{
	return index_buffer_type;
}

size_t Model::get_node_count()
{
	return _name_to_node.size();
}

bool Model::does_node_exist(node_t node_name)
{
	return (_name_to_node.find(node_name) != _name_to_node.end());
}

node_t Model::add_node(node_t parent)
{
	if (_name_to_node.find(parent) == _name_to_node.end() && parent != null_node_name) {
		std::cout << "[Model Error] Model::add_node() is called with a parent node that doesn't exist" << std::endl;
		ASSERT(false);
	}

	node_t name = _generate_node_name();
	_name_to_node[name] = Node(this, name, parent);
	
	if (parent != null_node_name)
		_name_to_node[parent].add_child(name);
	
	return name;
}

Model::Node* Model::get_node(node_t node_name)
{
	auto iterator = _name_to_node.find(node_name);
	if (iterator == _name_to_node.end())
		return nullptr;

	return &_name_to_node.at(node_name);
}

Model::Node& Model::operator[](node_t node_name)
{
	Node* node = get_node(node_name);
	if (node == nullptr) {
		std::cout << "[Model Error] Model::operator[]() is called with a node name that doesn't exist: " << node_name << std::endl;
		ASSERT(false);
	}

	return *get_node(node_name);
}

node_t Model::_generate_node_name()
{
	node_t id = _next_node_name;
	_next_node_name++;
	return id;
}


/*
size_t Model2::get_submodel_count() const {
	if (_submodel_to_name.size() != _name_to_submodel.size()) {
		std::cout << "[Model2 Error] internal error on the submodel datastructure, _submodel_to_name and _name_to_submodel maps are inconsistant" << std::endl;
		ASSERT(false);
	}

	return _submodel_to_name.size();
}

bool Model2::does_submodel_exist(std::shared_ptr<SingleModel2> submodel) const {
	if (!submodel) return false;
	return _submodel_to_name.find(submodel) != _submodel_to_name.end();
}

bool Model2::does_submodel_exist(uint32_t submodel_name) const {
	if (submodel_name == null_submodel_name) return false;
	return _name_to_submodel.find(submodel_name) != _name_to_submodel.end();
}

uint32_t Model2::insert_submodel(std::shared_ptr<SingleModel2> submodel) {
	if (!submodel) return null_submodel_name;

	auto iterator = _submodel_to_name.find(submodel);
	if (iterator != _submodel_to_name.end()) return iterator->second;

	uint32_t name = _generate_submodel_name();
	_submodel_to_name[submodel] = name;
	_name_to_submodel[name] = submodel;

	return name;
}

void Model2::erase_submodel(std::shared_ptr<SingleModel2> submodel) {
	if (!submodel) return;

	auto iterator = _submodel_to_name.find(submodel);
	uint32_t name = iterator->second;

	_submodel_to_name.erase(iterator);
	_name_to_submodel.erase(name);
}

void Model2::erase_submodel(uint32_t submodel_name) {
	if (submodel_name == null_submodel_name) return;

	auto iterator = _name_to_submodel.find(submodel_name);
	std::shared_ptr<SingleModel2> pointer = iterator->second;

	_name_to_submodel.erase(iterator);
	_submodel_to_name.erase(pointer);
}


//uint32_t Model2::insert_submodel(SingleModel2&& submodel){
//	std::shared_ptr<SingleModel2> submodel_ptr = std::make_shared<SingleModel2>(submodel);	// ?
//	return insert_submodel(submodel_ptr);
//}


//bool Model2::set_submodel(uint32_t submodel_name, std::shared_ptr<SingleModel2> new_submodel){
//	
//}

//bool Model2::set_submodel(uint32_t submodel_name, SingleModel2&& new_submodel){
//	
//}

uint32_t Model2::get_submodel_name(std::shared_ptr<SingleModel2> submodel) const {
	if (!submodel) return null_submodel_name;

	auto iterator = _submodel_to_name.find(submodel);
	if (iterator == _submodel_to_name.end()) {
		std::cout << "[Model Warning] Model::get_submodel_name() is called but given submodel doesn't exist in Model, you can call Model2::does_submodel_exist() before try to get a submodel" << std::endl;
		return null_submodel_name;
	}

	return iterator->second;
}

std::shared_ptr<SingleModel2> Model2::get_submodel(uint32_t submodel_name) {
	if (submodel_name == null_submodel_name) return nullptr;

	auto iterator = _name_to_submodel.find(submodel_name);
	if (iterator == _name_to_submodel.end()) {
		std::cout << "[Model Warning] Model::get_submodel() is called but given submodel_name doesn't exist in Model, you can call Model2::does_submodel_exist() before try to get a submodel" << std::endl;
		return nullptr;
	}

	return iterator->second;
}

uint32_t Model2::_generate_submodel_name() {
	uint32_t submodel_name = _next_submodel_name;
	_next_submodel_name++;
	return submodel_name;
}

void Model2::set_index_type(IndexType index_type) {
	_index_buffer_type = index_type;
}

IndexType Model2::get_index_type(){
	return _index_buffer_type;
}

// nodes
size_t Model2::get_node_count() {
	return _name_to_node.size();
}

bool Model2::does_node_exist(uint32_t node_name) {
	if (node_name == null_node_name) return false;
	return _name_to_node.find(node_name) != _name_to_node.end();
}

Model2::_ProxyNode Model2::operator[](uint32_t node_name) {
	return get_node(node_name);
}

Model2::_ProxyNode Model2::get_node(uint32_t node_name) {
	if (node_name == null_node_name) {
		std::cout << "[Model Error] Model::get_node() is called with null_node_name" << std::endl;
		ASSERT(false);
	}

	auto iterator = _name_to_node.find(node_name);
	if (iterator == _name_to_node.end()) {
		create_node(node_name);
		iterator = _name_to_node.find(node_name);
	}

	Node& node = iterator->second;
	return Model2::_ProxyNode(*this, node.name, node.childnodes, node.submodels, node.transform);
}

void Model2::create_node(uint32_t node_name) {
	if (node_name == null_node_name) return;
	_name_to_node[node_name] = Node(node_name);
}

void Model2::delete_node(uint32_t node_name) {
	if (node_name == null_node_name) return;
	_name_to_node.erase(node_name);
}

Model2::Node::Node(uint32_t node_name) :
	name(node_name) {}


Model2::_ProxyNode::_ProxyNode(Model2& owner_model, uint32_t name, std::vector<uint32_t>& childnodes, std::vector<uint32_t>& submodels, glm::mat4& transform) : 
	_owner_model(owner_model), _node_name(name), childnodes(childnodes), _childnodes(childnodes), submodels(submodels), _submodels(submodels), transform(transform) { }

bool Model2::_ProxyNode::add_submodel(std::shared_ptr<SingleModel2> submodel){
	if (submodel == nullptr) return false;
	
	uint32_t submodel_name = _owner_model.insert_submodel(submodel);

	return add_submodel(submodel_name);
}

bool Model2::_ProxyNode::add_submodel(uint32_t submodel_name){
	if (submodel_name == null_submodel_name) return false;
	if (!_owner_model.does_submodel_exist(submodel_name)) return false;

	auto iterator = std::find(_submodels.begin(), _submodels.end(), submodel_name);
	if (iterator != _submodels.end()) return false;

	_submodels.push_back(submodel_name);
	return true;
}

bool Model2::_ProxyNode::remove_submodel(std::shared_ptr<SingleModel2> submodel){
	if (submodel == nullptr) return false;

	if (!_owner_model.does_submodel_exist(submodel)) return false;
	uint32_t name = _owner_model.get_submodel_name(submodel);

	return remove_submodel(name);
}

bool Model2::_ProxyNode::remove_submodel(uint32_t submodel_name){
	if (submodel_name == null_submodel_name) return false;
	if (!_owner_model.does_submodel_exist(submodel_name)) return false;

	auto iterator = std::remove(_submodels.begin(), _submodels.end(), submodel_name);
	if (iterator == _submodels.end()) return false;
	
	_submodels.erase(iterator);
	return true;
}


bool Model2::_ProxyNode::add_childnode(uint32_t node_name){
	if (node_name == null_node_name) return false;
	if (node_name == root_node_name) return false;
	if (!_owner_model.does_node_exist(node_name)) return false;

	auto iterator = std::find(_childnodes.begin(), _submodels.end(), node_name);
	if (iterator != _childnodes.end()) return false;

	_childnodes.push_back(node_name);
	return true;
}

bool Model2::_ProxyNode::remove_childnode(uint32_t node_name){
	if (node_name == null_node_name) return false;
	if (node_name == root_node_name) return false;
	if (!_owner_model.does_node_exist(node_name)) return false;

	auto iterator = std::remove(_childnodes.begin(), _submodels.end(), node_name);
	if (iterator == _childnodes.end()) return false;

	_childnodes.erase(iterator);
	return true;
}

void Model2::_ProxyNode::clear(){
	transform = glm::mat4(1.0f);
	_childnodes.clear();
	_submodels.clear();
}

namespace {
	uint32_t _hash(uint32_t x) {
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = (x >> 16) ^ x;
		return x;
	}

	uint32_t _unhash(uint32_t x) {
		x = ((x >> 16) ^ x) * 0x119de1f3;
		x = ((x >> 16) ^ x) * 0x119de1f3;
		x = (x >> 16) ^ x;
		return x;
	}

	uint64_t _hash(uint64_t x) {
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ull;
		x = (x ^ (x >> 27)) * 0x94d049bb133111ebull;
		x = x ^ (x >> 31);
		return x;
	}

	uint64_t _unhash(uint64_t x) {
		x = (x ^ (x >> 31) ^ (x >> 62)) * 0x319642b2d24d8ec3ull;
		x = (x ^ (x >> 27) ^ (x >> 54)) * 0x96de1b173f119089ull;
		x = x ^ (x >> 30) ^ (x >> 60);
		return x;
	}
}

uint64_t Model2::_update_childnode_hahs() {
	_nodehash_to_node.clear();
	return _update_childnode_hash(root_node_name);
}


size_t Model2::_update_childnode_hash(uint32_t node_name) {

	if (!does_node_exist(node_name))
		return 0;

	Node& node = _name_to_node[node_name];

	uint64_t hash = 0;
	for (uint32_t& submodel_name : node.submodels) {
		hash = hash << 1;
		hash ^= _hash((uint64_t)get_submodel(submodel_name).get());
	}

	for (uint32_t& childnode : node.childnodes) {
		hash = hash << 1;
		hash ^= _update_childnode_hash(childnode);
	}
}
*/

std::unique_ptr<Buffer> Model::create_vertex_buffer() const {
	size_t vertex_count = 0;
	for (const SingleModel& submodel : single_models)
		vertex_count += submodel.verticies.size();
	
	typedef glm::vec3 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	
	size_t vertex_begin_pointer = 0;
	for (const SingleModel& submodel : single_models) {
		const std::vector<attribute_type>& data = submodel.verticies;
		buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
		vertex_begin_pointer += data.size();
	}

	return buffer;
}

std::unique_ptr<Buffer> Model::create_normal_buffer() const {
	size_t vertex_count = 0;
	for (const SingleModel& submodel : single_models)
		vertex_count += submodel.vertex_normals.size();

	typedef glm::vec3 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);

	size_t vertex_begin_pointer = 0;
	for (const SingleModel& submodel : single_models) {
		const std::vector<attribute_type>& data = submodel.vertex_normals;
		buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
		vertex_begin_pointer += data.size();
	}

	return buffer;
}

std::unique_ptr<Buffer> Model::create_tangent_buffer() const {
	size_t vertex_count = 0;
	for (const SingleModel& submodel : single_models)
		vertex_count += submodel.vertex_tangents.size();

	typedef glm::vec3 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);

	size_t vertex_begin_pointer = 0;
	for (const SingleModel& submodel : single_models) {
		const std::vector<attribute_type>& data = submodel.vertex_tangents;
		buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
		vertex_begin_pointer += data.size();
	}

	return buffer;
}

std::unique_ptr<Buffer> Model::create_uv0_buffer() const
{
	size_t vertex_count = 0;
	for (const SingleModel& submodel : single_models)
		vertex_count += submodel.texture_coordinates_0.size();

	typedef glm::vec2 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);

	size_t vertex_begin_pointer = 0;
	for (const SingleModel& submodel : single_models) {
		const std::vector<attribute_type>& data = submodel.texture_coordinates_0;
		buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
		vertex_begin_pointer += data.size();
	}

	return buffer;
}

std::unique_ptr<Buffer> Model::create_uv1_buffer() const
{
	size_t vertex_count = 0;
	for (const SingleModel& submodel : single_models)
		vertex_count += submodel.texture_coordinates_1.size();

	typedef glm::vec2 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);

	size_t vertex_begin_pointer = 0;
	for (const SingleModel& submodel : single_models) {
		const std::vector<attribute_type>& data = submodel.texture_coordinates_1;
		buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
		vertex_begin_pointer += data.size();
	}

	return buffer;
}

//std::unique_ptr<Buffer> Model::create_uv_merged_buffer() const {
//	size_t vertex_count = 0;
//	for (const SingleModel& submodel : single_models)
//		vertex_count += submodel.texture_coordinates.size();
//
//	typedef glm::vec4 attribute_type;
//	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
//
//	size_t vertex_begin_pointer = 0;
//	for (const SingleModel& submodel : single_models) {
//		const std::vector<attribute_type>& data = submodel.texture_coordinates;
//		buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
//		vertex_begin_pointer += data.size();
//	}
//
//	return buffer;
//}

std::unique_ptr<Buffer> Model::create_vertex_color_buffer() const {
	size_t vertex_count = 0;
	for (const SingleModel& submodel : single_models)
		vertex_count += submodel.vertex_colors.size();

	typedef glm::vec4 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);

	size_t vertex_begin_pointer = 0;
	for (const SingleModel& submodel : single_models) {
		const std::vector<attribute_type>& data = submodel.vertex_colors;
		buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
		vertex_begin_pointer += data.size();
	}

	return buffer;
}

std::unique_ptr<Buffer> Model::create_bone_indicies_buffer() const {
	size_t vertex_count = 0;
	for (const SingleModel& submodel : single_models)
		vertex_count += submodel.bone_indicies.size();

	typedef glm::ivec4 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);

	size_t vertex_begin_pointer = 0;
	for (const SingleModel& submodel : single_models) {
		const std::vector<attribute_type>& data = submodel.bone_indicies;
		buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
		vertex_begin_pointer += data.size();
	}

	return buffer;
}

std::unique_ptr<Buffer> Model::create_bone_weights_buffer() const {
	size_t vertex_count = 0;
	for (const SingleModel& submodel : single_models)
		vertex_count += submodel.bone_weights.size();

	typedef glm::vec4 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);

	size_t vertex_begin_pointer = 0;
	for (const SingleModel& submodel : single_models) {
		const std::vector<attribute_type>& data = submodel.bone_weights;
		buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
		vertex_begin_pointer += data.size();
	}

	return buffer;
}

std::unique_ptr<Buffer> Model::create_index_buffer() const {
	size_t vertex_count = 0;
	for (const SingleModel& submodel : single_models)
		vertex_count += submodel.indicies.size();

	uint32_t index_bytes_per_index = get_IndexType_bytes_per_index(index_buffer_type);
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * index_bytes_per_index, Buffer::GPU_BUFFER);

	size_t vertex_begin_pointer = 0;
	for (const SingleModel& submodel : single_models) {

		if (index_buffer_type == IndexType::i_ui8) {
			
			const std::vector<uint32_t>& data = submodel.indicies;
			std::vector<uint8_t> data_refactored;
			data_refactored.resize(data.size());

			for (int i = 0; i < data.size(); i++)
				data_refactored[i] = (uint8_t)data[i];

			buffer->load_data(vertex_begin_pointer, 0, data_refactored.size(), data_refactored);
			vertex_begin_pointer += data_refactored.size();
		}

		else if (index_buffer_type == IndexType::i_ui16) {

			const std::vector<uint32_t>& data = submodel.indicies;
			std::vector<uint16_t> data_refactored;
			data_refactored.resize(data.size());

			for (int i = 0; i < data.size(); i++)
				data_refactored[i] = (uint16_t)data[i];

			buffer->load_data(vertex_begin_pointer, 0, data_refactored.size(), data_refactored);
			vertex_begin_pointer += data_refactored.size();
		}

		else if (index_buffer_type == IndexType::i_ui32) {

			const std::vector<uint32_t>& data = submodel.indicies;
			buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
			vertex_begin_pointer += data.size();

		}
	}

	return buffer;
}

std::unique_ptr<VertexAttributeBuffer> Model::create_vertex_attribute_buffer() const
{
	size_t vertex_count = 0;
	size_t normal_count = 0;
	size_t tangent_count = 0;
	size_t uv0_count = 0;
	size_t uv1_count = 0;
	size_t vertex_color_count = 0;
	size_t bone_indicies_count = 0;
	size_t bone_weights_count = 0;
	size_t index_count = 0;

	for (const SingleModel& submodel : single_models) {
		vertex_count		+=	submodel.verticies.size();
		normal_count		+=	submodel.vertex_normals.size();
		tangent_count		+=	submodel.vertex_tangents.size();
		uv0_count			+=	submodel.texture_coordinates_0.size();
		uv1_count			+=	submodel.texture_coordinates_1.size();
		vertex_color_count	+=	submodel.vertex_colors.size();
		bone_indicies_count +=	submodel.bone_indicies.size();
		bone_weights_count	+=	submodel.bone_weights.size();
		index_count			+=	submodel.indicies.size();
	}

	std::unique_ptr<VertexAttributeBuffer> vab = std::make_unique<VertexAttributeBuffer>();
	
	typedef glm::vec3  vertex_attribute_type;
	typedef glm::vec3  normal_attribute_type;
	typedef glm::vec3  tangent_attribute_type;
	typedef glm::vec2  uv0_attribute_type;
	typedef glm::vec2  uv1_attribute_type;
	typedef glm::vec4  vertex_color_attribute_type;
	typedef glm::ivec4 bone_indicies_attribute_type;
	typedef glm::vec4  bone_weights_attribute_type;

	if (vertex_count != 0){
		vab->attach_vertex_buffer(Mesh::vab_vertex_slot, create_vertex_buffer(), sizeof(vertex_attribute_type), 0, 0);
		vab->set_attribute_format(Mesh::vab_vertex_slot, Mesh::vab_vertex_slot, VertexAttributeBuffer::a_f32, 3, 0, true);
	}

	if (normal_count != 0) {
		vab->attach_vertex_buffer(Mesh::vab_normal_slot, create_normal_buffer(), sizeof(normal_attribute_type), 0, 0);
		vab->set_attribute_format(Mesh::vab_normal_slot, Mesh::vab_normal_slot, VertexAttributeBuffer::a_f32, 3, 0, true);
	}

	if (tangent_count != 0) {
		vab->attach_vertex_buffer(Mesh::vab_tangent_slot, create_tangent_buffer(), sizeof(tangent_attribute_type), 0, 0);
		vab->set_attribute_format(Mesh::vab_tangent_slot, Mesh::vab_tangent_slot, VertexAttributeBuffer::a_f32, 3, 0, true);
	}

	if (uv0_count != 0) {
		vab->attach_vertex_buffer(Mesh::vab_uv0_slot, create_uv0_buffer(), sizeof(uv0_attribute_type), 0, 0);
		vab->set_attribute_format(Mesh::vab_uv0_slot, Mesh::vab_uv0_slot, VertexAttributeBuffer::a_f32, 2, 0, true);
	}

	if (uv1_count != 0) {
		vab->attach_vertex_buffer(Mesh::vab_uv1_slot, create_uv1_buffer(), sizeof(uv1_attribute_type), 0, 0);
		vab->set_attribute_format(Mesh::vab_uv1_slot, Mesh::vab_uv1_slot, VertexAttributeBuffer::a_f32, 2, 0, true);
	}

	if (vertex_color_count != 0) {
		vab->attach_vertex_buffer(Mesh::vab_vertex_color_slot, create_vertex_color_buffer(), sizeof(vertex_color_attribute_type), 0, 0);
		vab->set_attribute_format(Mesh::vab_vertex_color_slot, Mesh::vab_vertex_color_slot, VertexAttributeBuffer::a_f32, 4, 0, true);
	}

	if (bone_indicies_count != 0) {
		vab->attach_vertex_buffer(Mesh::vab_bone_indicies_slot, create_bone_indicies_buffer(), sizeof(bone_indicies_attribute_type), 0, 0);
		vab->set_attribute_format(Mesh::vab_bone_indicies_slot, Mesh::vab_bone_indicies_slot, VertexAttributeBuffer::a_i32, 4, 0, true);
	}

	if (bone_weights_count != 0) {
		vab->attach_vertex_buffer(Mesh::vab_bone_weights_slot, create_bone_weights_buffer(), sizeof(bone_weights_attribute_type), 0, 0);
		vab->set_attribute_format(Mesh::vab_bone_weights_slot, Mesh::vab_bone_weights_slot, VertexAttributeBuffer::a_f32, 4, 0, true);
	}

	return vab;
}


Model::Node::Node(Model* owner, uint32_t node_name, node_t parent, glm::mat4 transform) :
	owner(owner), name(node_name), parent(parent), transform(transform)
{
	if (owner == nullptr) {
		std::cout << "[Model Error] Model::Node is initialized with null referance to it's Model" << std::endl;
		ASSERT(false);
	}
}

node_t Model::Node::get_name()
{
	return name;
}

node_t Model::Node::get_parent()
{
	return parent;
}

void Model::Node::set_parent(node_t parent)
{
	this->parent = parent;
}

std::span<node_t> Model::Node::get_children()
{
	return std::span<node_t>(children);
}

void Model::Node::add_child(node_t child)
{
	if (!owner->does_node_exist(child)) {
		std::cout << "[Model Error] Model::Node::add_child() is called with a child that doesn't exist" << std::endl;
		ASSERT(false);
	}

	if (std::find(children.begin(), children.end(), child) == children.end()) {
		children.push_back(child);
	}
}

node_t Model::Node::create_child()
{
	return owner->add_node(name);
}

std::span<model_t> Model::Node::get_submodels()
{
	return std::span<model_t>(submodels);
}

void Model::Node::add_submodel(model_t model)
{
	if (!owner->does_model_exist(model)) {
		std::cout << "[Model Error] Model::Node::add_submodel() is called with a model that doesn't exist" << std::endl;
		ASSERT(false);
	}

	submodels.push_back(model);
}

glm::mat4 Model::Node::get_transform()
{
	return transform;
}

void Model::Node::set_transform(glm::mat4 transform)
{
	this->transform = transform;
}

Model* Model::Node::get_model()
{
	return owner;
}
