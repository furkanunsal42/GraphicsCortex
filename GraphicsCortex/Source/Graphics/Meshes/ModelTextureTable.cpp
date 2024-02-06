#include "ModelTextureTable.h"

void ModelTextureTable::set_diffuse_index(int target_mesh_index, int diffuse_map_index) {
	while (_table.size() <= target_mesh_index) {
		_table.push_back(_TextureIndex());
	}
	_table[target_mesh_index].diffuse_index = diffuse_map_index;
}

void ModelTextureTable::set_specular_index(int target_mesh_index, int specular_map_index) {
	while (_table.size() <= target_mesh_index) {
		_table.push_back(_TextureIndex());
	}
	_table[target_mesh_index].specular_index = specular_map_index;
}

void ModelTextureTable::set_normal_index(int target_mesh_index, int normal_map_index) {
	while (_table.size() <= target_mesh_index) {
		_table.push_back(_TextureIndex());
	}
	_table[target_mesh_index].normal_index = normal_map_index;
}

int ModelTextureTable::get_mesh_diffuse(int target_mesh_index) {
	if (_table.size() <= target_mesh_index)
		return -1;

	return _table[target_mesh_index].diffuse_index;
}

int ModelTextureTable::get_mesh_specular(int target_mesh_index) {
	if (_table.size() <= target_mesh_index)
		return -1;

	return _table[target_mesh_index].specular_index;
}

int ModelTextureTable::get_mesh_normal(int target_mesh_index) {
	if (_table.size() <= target_mesh_index)
		return -1;

	return _table[target_mesh_index].normal_index;
}

void ModelTextureTable::update_uniform_array() {
	if (_uniform_array != nullptr)
		delete _uniform_array;

	_uniform_array_size = _table.size() * 3;
	_uniform_array = new int[_uniform_array_size];
	
	for (int i = 0; i < _table.size(); i++) {
		_uniform_array[i * 3 + 0] = _table[i].diffuse_index;
		_uniform_array[i * 3 + 1] = _table[i].specular_index;
		_uniform_array[i * 3 + 2] = _table[i].normal_index;
	}
}

int*& ModelTextureTable::get_uniform_array(int* array_size) {
	if (array_size != nullptr)
		*array_size = _uniform_array_size;
	return _uniform_array;
}

void ModelTextureTable::clear() {
	if (_uniform_array != nullptr)
		delete _uniform_array;
	_uniform_array_size = 0;
	_table.clear();
}
