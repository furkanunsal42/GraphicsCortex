#include <algorithm>

#include "StandardModel.h"
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

	std::pair<Node, std::vector<Node>>& node_info = _nodes[node_name];
	
	return _ProxyNode(*this, node_name, node_info.second, node_info.first.submodels);
}

void Model2::create_node(uint32_t node_name)
{
	_nodes[node_name] = std::pair(Node(), std::vector<Node>());
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

Model2::_ProxyNode::_ProxyNode(Model2& owner_model, uint32_t name, std::vector<Node>& childnodes, std::vector<submodel_name>& submodels) :
	_owner_model(owner_model), _node_name(name), childnodes(childnodes), submodels(submodels) {}

bool Model2::_ProxyNode::add_submodel(uint32_t submodel_name)
{
	if(!_owner_model.does_submodel_exist(submodel_name))
		return false;

	_owner_model._nodes[_node_name].first.submodels.push_back(submodel_name);
	return true;
}

bool Model2::_ProxyNode::remove_submodel(uint32_t submodel_name)
{
	if (!_owner_model.does_submodel_exist(submodel_name))
		return false;

	std::vector<uint32_t>& submodels = _owner_model._nodes[_node_name].first.submodels;
	submodels.erase(std::remove(submodels.begin(), submodels.end(), submodel_name), submodels.end());
	
	return true;
}
