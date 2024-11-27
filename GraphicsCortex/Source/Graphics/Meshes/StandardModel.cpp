#include <algorithm>

#include "StandardModel.h"
#include "Debuger.h"
#include "StandardBuffer.h"

size_t SingleModel2::s_generate_next_id() {
	size_t generated_id = s_next_id;
	s_next_id++;
	return generated_id;
}


bool SingleModel2::operator==(const SingleModel2& other) const
{
	return id == other.id;
}

void SingleModel2::operator=(const SingleModel2& other)
{
	verticies = other.verticies;
	vertex_normals = other.vertex_normals;
	texture_coordinates = other.texture_coordinates;
	vertex_colors = other.vertex_colors;
	indicies = other.indicies;
	primitive = other.primitive;
}

template<>
struct std::hash<SingleModel2> {
	size_t operator()(const SingleModel2& object) const {
		return std::hash<size_t>()(object.id);
	}
};

SingleModel2::SingleModel2(const SingleModel2& other)
{
	this->operator=(other);
}

std::unique_ptr<Buffer> SingleModel2::create_vertex_buffer(size_t vertex_offset, size_t vertex_count) const
{
	if (vertex_offset >= verticies.size()) return nullptr;
	if (vertex_count + vertex_offset > verticies.size()) return nullptr;

	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(glm::vec3), Buffer::MemoryType::GPU_BUFFER);

	buffer->set_data<glm::vec3>(0, vertex_offset, vertex_count, verticies);

	return buffer;
}

std::unique_ptr<Buffer> SingleModel2::create_vertex_buffer(size_t vertex_offset) const {
	return create_vertex_buffer(vertex_offset, verticies.size() - vertex_offset);
}


void Model2::clear()
{
	_submodels_to_name.clear();
	_name_to_submodels.clear();
	_next_submodel_name = 0;
	_nodes.clear();
}

size_t Model2::get_submodel_count() {
	return _name_to_submodels.size();
}

bool Model2::does_submodel_exist(uint32_t submodel_name)
{
	return _name_to_submodels.find(submodel_name) != _name_to_submodels.end();
}

bool Model2::does_submodel_exist(std::shared_ptr<SingleModel2> submodel)
{
	return _submodels_to_name.find(submodel) != _submodels_to_name.end();
}

uint32_t Model2::insert_submodel(std::shared_ptr<SingleModel2> submodel)
{
	if (_submodels_to_name.find(submodel) != _submodels_to_name.end())
		return _submodels_to_name[submodel];

	uint32_t name = _generate_submodel_name();
	_name_to_submodels[name] = submodel;
	_submodels_to_name[submodel] = name;
	return name;
}

void Model2::erase_submodel(uint32_t submodel_name)
{
	if (_name_to_submodels.find(submodel_name) == _name_to_submodels.end())
		return;

	std::shared_ptr<SingleModel2> submodel = _name_to_submodels[submodel_name];
	
	_name_to_submodels.erase(submodel_name);
	_submodels_to_name.erase(submodel);
}

void Model2::erase_submodel(std::shared_ptr<SingleModel2> submodel)
{
	if (_submodels_to_name.find(submodel) == _submodels_to_name.end())
		return;

	uint32_t name = _submodels_to_name[submodel];

	_submodels_to_name.erase(submodel);
	_name_to_submodels.erase(name);
}

std::shared_ptr<SingleModel2> Model2::get_submodel(uint32_t submodel_name)
{
	if (_name_to_submodels.find(submodel_name) == _name_to_submodels.end())
		return nullptr;

	return _name_to_submodels[submodel_name];
}

uint32_t Model2::get_submodel_name(std::shared_ptr<SingleModel2> submodel)
{
	if (_submodels_to_name.find(submodel) == _submodels_to_name.end())
		return -1;

	return _submodels_to_name[submodel];
}

bool Model2::set_submodel(uint32_t submodel_name, std::shared_ptr<SingleModel2> submodel)
{
	if (_name_to_submodels.find(submodel_name) == _name_to_submodels.end())
		return false;

	if (_submodels_to_name.find(submodel) != _submodels_to_name.end())
		return false;

	erase_submodel(submodel_name);

	_name_to_submodels[submodel_name] = submodel;
	_submodels_to_name[submodel] = submodel_name;

	return true;
}

size_t Model2::get_node_count()
{
	return _nodes.size();
}

bool Model2::does_node_exist(uint32_t node_name)
{
	return _nodes.find(node_name) != _nodes.end();
}

Model2::_ProxyNode Model2::operator[](uint32_t node_name)
{
	if (!does_node_exist(node_name))
		create_node(node_name);

	std::pair<Node, std::vector<uint32_t>>& node_info = _nodes[node_name];
	
	return _ProxyNode(*this, node_name, node_info.second, node_info.first.submodels);
}

void Model2::create_node(uint32_t node_name)
{
	_nodes[node_name] = std::pair(Node(), std::vector<uint32_t>());
	_nodes[node_name].first.name = node_name;
}

void Model2::delete_node(uint32_t node_name)
{
	_nodes.erase(node_name);
}

uint32_t Model2::_generate_submodel_name()
{
	return _next_submodel_name++;
}

Model2::_ProxyNode::_ProxyNode(Model2& owner_model, uint32_t name, std::vector<node_name>& childnodes, std::vector<submodel_name>& submodels) :
	_owner_model(owner_model), _node_name(name), childnodes(childnodes), submodels(submodels) {}

void Model2::_ProxyNode::operator=(const Model2::_ProxyNode& other) {
	if (&_owner_model != &other._owner_model) {
		
		std::cout << "[Model Error] Model::_ProxyNode::operator=(const Model::_ProxyNode& other) cannot be called between _ProxyNodes of distinct Models" << std::endl;
		ASSERT(false);

		// insert all relevant submodels to this
		//for (uint32_t _others_submodel_name : other.submodels) {
		//	_owner_model.insert_submodel(other._owner_model.get_submodel(_others_submodel_name));
		//}

		// define all other's child nodes to this


		// create same hierarchy in this
		return;
	}
	
	submodels = other.submodels;
	childnodes = other.childnodes;
}

bool Model2::_ProxyNode::add_submodel(uint32_t submodel_name)
{
	if(!_owner_model.does_submodel_exist(submodel_name))
		return false;

	std::vector<uint32_t>& submodels_ref = _owner_model._nodes[_node_name].first.submodels;

	if (std::find(submodels_ref.begin(), submodels_ref.end(), submodel_name) == submodels_ref.end())
		return false;

	submodels_ref.push_back(submodel_name);
	return true;
}

bool Model2::_ProxyNode::remove_submodel(uint32_t submodel_name)
{
	if (!_owner_model.does_submodel_exist(submodel_name))
		return false;

	std::vector<uint32_t>& submodels_ref = _owner_model._nodes[_node_name].first.submodels;

	auto erase_begin = std::remove(submodels_ref.begin(), submodels_ref.end(), submodel_name);
	if (erase_begin == submodels_ref.end())
		return false;

	submodels_ref.erase(erase_begin, submodels_ref.end());
	return true;
}

bool Model2::_ProxyNode::add_childnode(uint32_t node_name)
{
	if (!_owner_model.does_node_exist(node_name))
		return false;

	if (std::find(childnodes.begin(), childnodes.end(), node_name) != childnodes.end())
		return false;

	childnodes.push_back(node_name);
	return true;
}

bool Model2::_ProxyNode::remove_childnode(uint32_t node_name)
{
	if (!_owner_model.does_node_exist(node_name))
		return false;

	if (std::find(childnodes.begin(), childnodes.end(), node_name) == childnodes.end())
		return false;

	
	childnodes.erase(std::remove(childnodes.begin(), childnodes.end(), node_name));
	return true;
}

void Model2::_ProxyNode::clear()
{
	childnodes.clear();
	submodels.clear();
}
