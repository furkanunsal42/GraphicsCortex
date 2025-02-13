#include <algorithm>
#include <iostream>
#include <stdint.h>

#include "StandardModel.h"
#include "Debuger.h"
#include "StandardBuffer.h"

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

std::unique_ptr<Buffer> Model2::create_vertex_buffer(size_t buffer_left_padding, size_t buffer_right_padding) const {
	size_t vertex_count = 0;
	for (auto& pair : _name_to_submodel)
		vertex_count += pair.second->read_verticies().size();
	
	typedef glm::vec3 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	
	size_t vertex_begin_pointer = 0;
	for (auto& pair : _name_to_submodel) {
		const std::vector<attribute_type>& data = pair.second->read_verticies();
		buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
		vertex_begin_pointer += data.size();
	}

	return buffer;
}

std::unique_ptr<Buffer> Model2::create_normal_buffer(size_t buffer_left_padding, size_t buffer_right_padding) const {
	size_t vertex_count = 0;
	for (auto& pair : _name_to_submodel)
		vertex_count += pair.second->read_vertex_normals().size();

	typedef glm::vec3 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);

	size_t vertex_begin_pointer = 0;
	for (auto& pair : _name_to_submodel) {
		const std::vector<attribute_type>& data = pair.second->read_vertex_normals();
		buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
		vertex_begin_pointer += data.size();
	}

	return buffer;
}

std::unique_ptr<Buffer> Model2::create_tangent_buffer(size_t buffer_left_padding, size_t buffer_right_padding) const {
	size_t vertex_count = 0;
	for (auto& pair : _name_to_submodel)
		vertex_count += pair.second->read_vertex_tangents().size();

	typedef glm::vec3 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);

	size_t vertex_begin_pointer = 0;
	for (auto& pair : _name_to_submodel) {
		const std::vector<attribute_type>& data = pair.second->read_vertex_tangents();
		buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
		vertex_begin_pointer += data.size();
	}

	return buffer;
}

//std::unique_ptr<Buffer> Model2::create_uv0_buffer(size_t buffer_left_padding, size_t buffer_right_padding) const;
//std::unique_ptr<Buffer> Model2::create_uv1_buffer(size_t buffer_left_padding, size_t buffer_right_padding) const;


std::unique_ptr<Buffer> Model2::create_uv_merged_buffer(size_t buffer_left_padding, size_t buffer_right_padding) const {
	size_t vertex_count = 0;
	for (auto& pair : _name_to_submodel)
		vertex_count += pair.second->read_texture_coordinates().size();

	typedef glm::vec4 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);

	size_t vertex_begin_pointer = 0;
	for (auto& pair : _name_to_submodel) {
		const std::vector<attribute_type>& data = pair.second->read_texture_coordinates();
		buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
		vertex_begin_pointer += data.size();
	}

	return buffer;
}


std::unique_ptr<Buffer> Model2::create_vertex_color_buffer(size_t buffer_left_padding, size_t buffer_right_padding) const {
	size_t vertex_count = 0;
	for (auto& pair : _name_to_submodel)
		vertex_count += pair.second->read_vertex_colors().size();

	typedef glm::vec4 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);

	size_t vertex_begin_pointer = 0;
	for (auto& pair : _name_to_submodel) {
		const std::vector<attribute_type>& data = pair.second->read_vertex_colors();
		buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
		vertex_begin_pointer += data.size();
	}

	return buffer;
}

std::unique_ptr<Buffer> Model2::create_bone_indicies_buffer(size_t buffer_left_padding, size_t buffer_right_padding) const {
	size_t vertex_count = 0;
	for (auto& pair : _name_to_submodel)
		vertex_count += pair.second->read_bone_indicies().size();

	typedef glm::ivec4 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);

	size_t vertex_begin_pointer = 0;
	for (auto& pair : _name_to_submodel) {
		const std::vector<attribute_type>& data = pair.second->read_bone_indicies();
		buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
		vertex_begin_pointer += data.size();
	}

	return buffer;
}

std::unique_ptr<Buffer> Model2::create_bone_weights_buffer(size_t buffer_left_padding, size_t buffer_right_padding) const {
	size_t vertex_count = 0;
	for (auto& pair : _name_to_submodel)
		vertex_count += pair.second->read_bone_weights().size();

	typedef glm::vec4 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);

	size_t vertex_begin_pointer = 0;
	for (auto& pair : _name_to_submodel) {
		const std::vector<attribute_type>& data = pair.second->read_bone_weights();
		buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
		vertex_begin_pointer += data.size();
	}

	return buffer;
}

std::unique_ptr<Buffer> Model2::create_index_buffer(size_t buffer_left_padding, size_t buffer_right_padding) const{
	size_t vertex_count = 0;
	for (auto& pair : _name_to_submodel)
		vertex_count += pair.second->read_indicies().size();

	uint32_t index_bytes_per_index = get_IndexType_bytes_per_index(_index_buffer_type);
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * index_bytes_per_index, Buffer::GPU_BUFFER);

	size_t vertex_begin_pointer = 0;
	for (auto& pair : _name_to_submodel) {

		if (_index_buffer_type == IndexType::i_ui8) {
			
			const std::vector<uint32_t>& data = pair.second->read_indicies();
			std::vector<uint8_t> data_refactored;
			data_refactored.resize(data.size());

			for (int i = 0; i < data.size(); i++)
				data_refactored[i] = (uint8_t)data[i];

			buffer->load_data(vertex_begin_pointer, 0, data_refactored.size(), data_refactored);
			vertex_begin_pointer += data_refactored.size();
		}

		else if (_index_buffer_type == IndexType::i_ui16) {

			const std::vector<uint32_t>& data = pair.second->read_indicies();
			std::vector<uint16_t> data_refactored;
			data_refactored.resize(data.size());

			for (int i = 0; i < data.size(); i++)
				data_refactored[i] = (uint16_t)data[i];

			buffer->load_data(vertex_begin_pointer, 0, data_refactored.size(), data_refactored);
			vertex_begin_pointer += data_refactored.size();
		}

		else if (_index_buffer_type == IndexType::i_ui32) {

			const std::vector<uint32_t>& data = pair.second->read_indicies();
			buffer->load_data(vertex_begin_pointer, 0, data.size(), data);
			vertex_begin_pointer += data.size();

		}
	}

	return buffer;
}
*/