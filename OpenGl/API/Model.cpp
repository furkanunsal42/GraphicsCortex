#include "Model.h"

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include "Image.h"
#include <vector>
#include <map>

#include "AssetImporter.h"

Model::Model(const std::string& file_path, float scale, unsigned int vertex_property_bits) {
	Model(std::move(AssetImporter::generate_model(file_path, scale, vertex_property_bits)));
}

Model::Model(const aiScene* scene, float scale, unsigned int vertex_property_bits) {
	_load_model(scene, scale, vertex_property_bits);
}

namespace {
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
}

void Model::_load_model(const aiScene* scene, float scale, unsigned int vertex_property_bits) {
	clear_ram();

	std::vector<char> enabled_bits = compute_enabled_bits(vertex_property_bits, PROPERTY_COUNT);

	int vertex_count = 0;
	int index_count = 0;
	for (int i = 0; i < scene->mNumMeshes; i++) {
		vertex_count += scene->mMeshes[i]->mNumVertices;
		index_count += scene->mMeshes[i]->mNumFaces;
	}
	index_count *= 3; // if all faces are triangles!

	unsigned int prefix_indicies_sum = 0;

	std::vector<std::string> image_names;


	unsigned int map_types[MAP_TYPE_COUNT] = { aiTextureType_DIFFUSE, aiTextureType_METALNESS, aiTextureType_NORMALS };
	for (int i = 0; i < scene->mNumMeshes; i++) {

		// -- Generating Material --
		// 0: diffuse_index, 1: specular_index, 2: normal_index
		int map_indicies[MAP_TYPE_COUNT] = { -1, -1, -1 };

		for (int j = 0; j < MAP_TYPE_COUNT; j++) {
			int material_index = scene->mMeshes[i]->mMaterialIndex;
			aiString image_name;
			if (scene->mMaterials[material_index]->GetTexture((aiTextureType)map_types[j], 0, &image_name) != AI_SUCCESS) {
				map_indicies[j] = -1; // texture not found
				continue;
			}

			bool image_exists = false;
			for (int k = 0; k < image_names.size(); k++) {
				if (image_names[k] == image_name.C_Str()) {
					image_exists = true;
					map_indicies[j] = k;
					break;
				}
			}
			if (!image_exists) {
				map_indicies[j] = image_names.size();
				image_names.push_back(image_name.C_Str());
			}
		}
		_model_texture_table.set_diffuse_index(i, map_indicies[0]);
		_model_texture_table.set_specular_index(i, map_indicies[1]);
		_model_texture_table.set_normal_index(i, map_indicies[2]);


		// -- Generating  Model -- 
		for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {

			aiVector3D vertex = scene->mMeshes[i]->mVertices[j];
			if (enabled_bits[0])
				vertex_data.push_back((float)vertex.x * scale);
			if (enabled_bits[1])
				vertex_data.push_back((float)vertex.y * scale);
			if (enabled_bits[2])
				vertex_data.push_back((float)vertex.z * scale);

			aiVector3D texcoords = scene->mMeshes[i]->mTextureCoords[0][j];
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

			aiVector3D normal = scene->mMeshes[i]->mNormals[j];
			normal.Normalize();
			if (enabled_bits[8])
				vertex_data.push_back((float)normal.x);
			if (enabled_bits[9])
				vertex_data.push_back((float)normal.y);
			if (enabled_bits[10])
				vertex_data.push_back((float)normal.z);
		}
		for (int j = 0; j < scene->mMeshes[i]->mNumFaces; j++) {
			const aiFace& Face = scene->mMeshes[i]->mFaces[j];

			index_data.push_back(prefix_indicies_sum + (unsigned int)Face.mIndices[0]);
			index_data.push_back(prefix_indicies_sum + (unsigned int)Face.mIndices[1]);
			index_data.push_back(prefix_indicies_sum + (unsigned int)Face.mIndices[2]);

		}
		prefix_indicies_sum += scene->mMeshes[i]->mNumVertices;
	}

	int coord_dim = 0;
	int bit = 0;
	for (int i = COORD_X; i <= COORD_Z; i *= 2) {
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
	
	vertex_attribute_structure.push_back(coord_dim);		// position
	vertex_attribute_structure.push_back(tex_coord_dim);	// texture uv
	vertex_attribute_structure.push_back(map_index_dim);	// mesh index
	vertex_attribute_structure.push_back(normals_dim);		// normals
}

void Model::clear_ram() {
	vertex_data.clear();
	index_data.clear();
	_model_texture_table.clear();
	vertex_attribute_structure.clear();
}