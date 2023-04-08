#pragma once

#include "Config.h"

#include "Buffer.h"

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm.hpp>

#include <PxPhysics.h>
#include "Image.h"

#include <iostream>
#include <vector>
#include "Texture.h"
#include "ModelTextureTable.h"

class AssetImporter;

class Model {
public:
	std::vector<float> vertex_data;
	std::vector<unsigned int> index_data;
	std::vector<unsigned int> vertex_attribute_structure;

	Model(const std::string& file_path, float scale = 1.0f, unsigned int vertex_property_bits = ALL);
	Model(const aiScene* scene, float scale = 1.0f, unsigned int vertex_property_bits = ALL);

	Model(std::vector<float> verticies, std::vector<unsigned int> indicies, std::vector<unsigned int> vertex_attribute_structure = {3});

	Model(Model&& other) = default;
	Model& operator=(Model&& other) = default;

	enum vertex_property {
		COORD_X = 1,
		COORD_Y = 2,
		COORD_Z = 4,
		COORD_XYZ = 1 | 2 | 4,

		TEX_COORD_X = 8,
		TEX_COORD_Y = 16,
		TEX_COORD_XY = 8 | 16,

		TEX_COORD_Z_DIFFUSE = 32,
		TEX_COORD_Z_SPECULAR = 64,
		TEX_COORD_Z_NORMAL = 128,

		NORMAL_X = 256,
		NORMAL_Y = 512,
		NORMAL_Z = 1024,
		NORMAL_XYZ = 256 | 512 | 1024,

		ALL = 2047,
		PROPERTY_COUNT = 11,
	};

	void load_model(const std::string& file_path, float scale = 1.0f, unsigned int vertex_property_bits = ALL);

	template<typename T>
	std::enable_if_t<std::is_same<T, float>::value, std::vector<T>>
		const get_partial_data(const std::string& mask = "11100000") {
		unsigned int total_entry_per_vertex = 0;
		for (unsigned int structure_size : vertex_attribute_structure) {
			total_entry_per_vertex += structure_size;
		}

		if (total_entry_per_vertex < 1) {
			std::cout << "[Error] Model::get_partial_data(const std::string& mask) was called but vertex structure was empty" << std::endl;
			ASSERT(false);
		}

		if (mask.size() > total_entry_per_vertex) {
			std::cout << "[Error] Model::get_partial_data(const std::string& mask) was called but mask is bigger than structure" << std::endl;
			ASSERT(false);
		}

		std::vector<unsigned int> selected_structure;

		for (unsigned int i = 0; i < mask.size(); i++) {
			if (mask[i] == '1') {
				selected_structure.push_back(i);
			}
			else if (mask[i] != '0') {
				std::cout << "[Error] Model::get_partial_data(const std::string& mask) was called but mask contain non-binary characters. They are assumed to be 0" << std::endl;
			}
		}

		unsigned int partial_data_size = vertex_data.size() / total_entry_per_vertex * selected_structure.size();

		std::vector<T> partial_data;
		partial_data.reserve(partial_data_size);

		for (unsigned int i = 0; i < vertex_data.size(); i += total_entry_per_vertex) {
			for (const unsigned int& selected_index : selected_structure) {
				partial_data.push_back(vertex_data[i + selected_index]);
			}
		}

		return partial_data;
	}

	template<typename T>
	std::enable_if_t<std::is_same<T, glm::vec3>::value || std::is_same<T, physx::PxVec3>::value, std::vector<T>>
		get_partial_data(const std::string& mask = "11100000") {
		unsigned int total_entry_per_vertex = 0;
		for (unsigned int structure_size : vertex_attribute_structure) {
			total_entry_per_vertex += structure_size;
		}

		if (total_entry_per_vertex < 1) {
			std::cout << "[Error] Model::get_partial_data(const std::string& mask) was called but vertex structure was empty" << std::endl;
			ASSERT(false);
		}

		if (mask.size() > total_entry_per_vertex) {
			std::cout << "[Error] Model::get_partial_data(const std::string& mask) was called but mask is bigger than structure" << std::endl;
			ASSERT(false);
		}

		std::vector<unsigned int> selected_structure;

		for (unsigned int i = 0; i < mask.size(); i++) {
			if (mask[i] == '1') {
				selected_structure.push_back(i);
			}
			else if (mask[i] != '0') {
				std::cout << "[Error] Model::get_partial_data(const std::string& mask) was called but mask contain non-binary characters. They are assumed to be 0" << std::endl;
			}
		}

		if (selected_structure.size() != 3) {
			std::cout << "[Error] Model::get_partial_data<glm::vec3>(const std::string& mask) was called but selected structure size is not 3" << std::endl;
			ASSERT(false);
		}

		unsigned int partial_data_size = vertex_data.size() / total_entry_per_vertex * selected_structure.size();

		std::vector<T> partial_data;
		partial_data.reserve(partial_data_size);

		for (unsigned int i = 0; i < vertex_data.size(); i += total_entry_per_vertex) {
			T vector(vertex_data[i + selected_structure[0]], vertex_data[i + selected_structure[1]], vertex_data[i + selected_structure[2]]);
			partial_data.push_back(vector);
		}

		return partial_data;
	}


	void clear_ram();

	
	//ModelTextureTable _model_texture_table; // temp 


	friend AssetImporter;
private:

	void _load_model(const aiScene* scene, float scale, unsigned int vertex_property_bits);

};