#pragma once
#include <vector>

class ModelTextureTable {
public:

	void set_diffuse_index(int target_mesh_index, int diffuse_map_index);
	void set_specular_index(int target_mesh_index, int specular_map_index);
	void set_normal_index(int target_mesh_index, int normal_map_index);

	int get_mesh_diffuse(int target_mesh_index);
	int get_mesh_specular(int target_mesh_index);
	int get_mesh_normal(int target_mesh_index);

	void update_uniform_array();
	int*& get_uniform_array(int* array_size);

	void clear();
private:

	struct _TextureIndex {
		int diffuse_index = -1;
		int specular_index = -1;
		int normal_index = -1;
	};

	int _uniform_array_size = 0;
	int* _uniform_array = nullptr;
	std::vector<_TextureIndex> _table;
};
