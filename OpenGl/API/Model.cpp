#include "Model.h"

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include "Image.h"
#include <vector>
#include <map>

Assimp::Importer Model::asset_loader;

Model::Model() :
	vertex_buffer(ArrayBuffer()), index_buffer(IndexBuffer()) {}

Model::Model(const std::string& file_path) {
	load_model(file_path);
}


Model::Model(ArrayBuffer& verticies, IndexBuffer& indicies) :
	vertex_buffer(std::move(verticies)), index_buffer(std::move(indicies)) {}

Model::Model(ArrayBuffer&& verticies, IndexBuffer&& indicies) :
	vertex_buffer(verticies), index_buffer(indicies) {}

std::string compute_directory(const std::string& file_name) { // "C:\folder\folder\file" -> "C:\folder\folder\"
	std::string dir = file_name;
	for (int i = dir.size() - 1; i >= 0; i--) {
		if (dir[i] != '/' && dir[i] != '\\')
			dir.pop_back();
		else
			break;
	}
	return dir;
}

std::string compute_filename(const std::string& file_name) { // "C:\folder\folder\file" -> "file"
	std::string name = "";
	for (int i = file_name.size() - 1; i >= 0; i--) {
		if (file_name[i] != '/' && file_name[i] != '\\')
			name = file_name[i] + name;
		else
			break;
	}
	return name;
}

std::vector<char> compute_enabled_bits(unsigned int binary_bits, int bit_count) {
	ASSERT(bit_count <= 32);
	std::vector<char> bits(32);
	int mask = 1;
	for (int i = 0; i < bit_count; i++) {
		if ((binary_bits & mask) == mask)
			bits[i] = true;
		else
			bits[i] = false;
		mask *= 2;
	}
	return bits;
}

UnorderedMaterial Model::load_model(const std::string& filepath, float scale, unsigned int vertex_property_bits) {
	
	clear_ram(); 

	std::vector<char> enabled_bits = compute_enabled_bits(vertex_property_bits, PROPERTY_COUNT);

	const aiScene* imported_scene = asset_loader.ReadFile(filepath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_GenSmoothNormals);

	std::string load_error = asset_loader.GetErrorString();
	if (load_error != "")
		std::cout << "[Model Loading Error]: " << load_error << '\n';

	int vertex_count = 0;
	int index_count = 0;
	for (int i = 0; i < imported_scene->mNumMeshes; i++) {
		vertex_count += imported_scene->mMeshes[i]->mNumVertices;
		index_count += imported_scene->mMeshes[i]->mNumFaces;
	}
	index_count *= 3; // if all faces are triangles!

	unsigned int prefix_indicies_sum = 0;

	std::vector<std::string> image_paths;
	std::string dir = compute_directory(filepath);
	
	#define MAP_TYPE_COUNT 3
	unsigned int map_types[MAP_TYPE_COUNT] = { aiTextureType_DIFFUSE, aiTextureType_METALNESS, aiTextureType_NORMALS };
	for (int i = 0; i < imported_scene->mNumMeshes; i++) {
		
		// -- Generating Material --
		// 0: diffuse_index, 1: specular_index, 2: normal_index
		int map_indicies[MAP_TYPE_COUNT] = {-1, -1, -1};

		for (int j = 0; j < MAP_TYPE_COUNT; j++) {
			int material_index = imported_scene->mMeshes[i]->mMaterialIndex;
			aiString image_name;
			if (imported_scene->mMaterials[material_index]->GetTexture((aiTextureType)map_types[j], 0, &image_name) != AI_SUCCESS) {
				map_indicies[j] = -1; // texture not found
				continue;
			}

			std::string path = dir + "textures/" + compute_filename(std::string(image_name.C_Str()));
			
			bool image_exists = false;
			for (int k = 0; k < image_paths.size(); k++) {
				if (image_paths[k] == path) {
					image_exists = true;
					map_indicies[j] = k;
					break;
				}
			}
			if(!image_exists){
				map_indicies[j] = image_paths.size();
				image_paths.push_back(path);
			}
		}
		_model_texture_table.set_diffuse_index(i, map_indicies[0]);
		_model_texture_table.set_specular_index(i, map_indicies[1]);
		_model_texture_table.set_normal_index(i, map_indicies[2]);


		// -- Generating  Model -- 
		for (int j = 0; j < imported_scene->mMeshes[i]->mNumVertices; j++) {

			aiVector3D vertex = imported_scene->mMeshes[i]->mVertices[j];
			if (enabled_bits[0])
				vertex_data.push_back((float)vertex.x * scale);
			if (enabled_bits[1])
				vertex_data.push_back((float)vertex.y * scale);
			if (enabled_bits[2])
				vertex_data.push_back((float)vertex.z * scale);

			aiVector3D texcoords = imported_scene->mMeshes[i]->mTextureCoords[0][j];
			if (enabled_bits[3])
				vertex_data.push_back(texcoords.x);
			if (enabled_bits[4])
				vertex_data.push_back(texcoords.y);

			if (enabled_bits[5])
				vertex_data.push_back(map_indicies[0]);
			if (enabled_bits[6])
				vertex_data.push_back(map_indicies[1]);
			if (enabled_bits[7])
				vertex_data.push_back(map_indicies[2]);
			
			aiVector3D normal = imported_scene->mMeshes[i]->mNormals[j];
			normal.Normalize();
			if (enabled_bits[8])
				vertex_data.push_back((float)normal.x);
			if (enabled_bits[9])
				vertex_data.push_back((float)normal.y);
			if (enabled_bits[10])
				vertex_data.push_back((float)normal.z);
		}
		for (int j = 0; j < imported_scene->mMeshes[i]->mNumFaces; j++) {
			const aiFace& Face = imported_scene->mMeshes[i]->mFaces[j];

			index_data.push_back(prefix_indicies_sum + (unsigned int)Face.mIndices[0]);
			index_data.push_back(prefix_indicies_sum + (unsigned int)Face.mIndices[1]);
			index_data.push_back(prefix_indicies_sum + (unsigned int)Face.mIndices[2]);

		}
		prefix_indicies_sum += imported_scene->mMeshes[i]->mNumVertices;
	}

	int coord_dim = 0;
	int bit = 0;
	for (int i = COORD_X; i <= COORD_Z; i*=2){
		if (enabled_bits[bit])
			coord_dim++;
		bit++;
	}
	bit = 3;
	int tex_coord_dim = 0;
	for (int i = TEX_COORD_X; i <= TEX_COORD_Y; i *= 2) {
		if (enabled_bits[bit])
			tex_coord_dim++;
		bit++;
	}
	bit = 5;
	int map_index_dim = 0;
	for (int i = TEX_COORD_Z_DIFFUSE; i <= TEX_COORD_Z_NORMAL; i *= 2) {
		if (enabled_bits[bit])
			map_index_dim++;
		bit++;
	}
	bit = 8;
	int normals_dim = 0;
	for (int i = NORMAL_X; i <= NORMAL_Z; i *= 2) {
		if (enabled_bits[bit])
			normals_dim++;
		bit++;
	}

	vertex_buffer.vertex_attribute_structure.clear();
	vertex_buffer.initialize_buffer(vertex_data);
	std::cout << coord_dim << tex_coord_dim << map_index_dim << normals_dim << std::endl;
	vertex_buffer.push_attribute(coord_dim);		// position
	vertex_buffer.push_attribute(tex_coord_dim);	// texture uv
	vertex_buffer.push_attribute(map_index_dim);	// mesh index
	vertex_buffer.push_attribute(normals_dim);		// normals

	index_buffer.initialize_buffer(index_data, 3);

	UnorderedMaterial material(image_paths.size());
	for (int i = 0; i < image_paths.size(); i++) {
		material.set_texture(image_paths[i], 4, i);
	}
	return material;
}

void Model::clear_ram() {
	vertex_data.clear();
	index_data.clear();
	_model_texture_table.clear();
}