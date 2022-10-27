#include "Model.h"

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

Assimp::Importer Model::asset_loader;

Model::Model() :
	vertex_buffer(ArrayBuffer()), index_buffer(IndexBuffer()) {}

Model::Model(ArrayBuffer& verticies, IndexBuffer& indicies) :
	vertex_buffer(std::move(verticies)), index_buffer(std::move(indicies)) {}

Model::Model(ArrayBuffer&& verticies, IndexBuffer&& indicies) :
	vertex_buffer(verticies), index_buffer(indicies) {}

void Model::load_model(const std::string& filepath) {
	
	vertex_data.clear();
	index_data.clear();

	const aiScene* imported_scene = asset_loader.ReadFile(filepath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_GenSmoothNormals);

	std::cout << asset_loader.GetErrorString();

	int vertex_count = 0;
	int index_count = 0;
	for (int i = 0; i < imported_scene->mNumMeshes; i++) {
		vertex_count += imported_scene->mMeshes[i]->mNumVertices;
		index_count += imported_scene->mMeshes[i]->mNumFaces;
	}
	index_count *= 3; // if all faces are triangles!

	unsigned int prefix_indicies_sum = 0;

	for (int i = 0; i < imported_scene->mNumMeshes; i++) {
		for (int j = 0; j < imported_scene->mMeshes[i]->mNumVertices; j++) {

			aiVector3D vertex = imported_scene->mMeshes[i]->mVertices[j];
			vertex_data.push_back((float)vertex.x);
			vertex_data.push_back((float)vertex.y);
			vertex_data.push_back((float)vertex.z);

			aiVector3D texcoords = imported_scene->mMeshes[i]->mTextureCoords[0][j];
			vertex_data.push_back(texcoords.x);
			vertex_data.push_back(texcoords.y);

			aiVector3D normal = imported_scene->mMeshes[i]->mNormals[j];
			normal.Normalize();
			vertex_data.push_back((float)normal.x);
			vertex_data.push_back((float)normal.y);
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
	vertex_buffer.vertex_attribute_structure.clear();
	vertex_buffer.initialize_buffer(vertex_data);
	vertex_buffer.push_attribute(3);	// position
	vertex_buffer.push_attribute(2);	// texture
	vertex_buffer.push_attribute(3);	// normals

	index_buffer.initialize_buffer(index_data, 3);
}

/*
std::vector<float> Model::get_partial_data(const std::string& mask) {
	unsigned int total_entry_per_vertex = 0;
	for (unsigned int structure_size : vertex_buffer.vertex_attribute_structure) {
		total_entry_per_vertex += structure_size;
	}
	
	if (total_entry_per_vertex < 1) {
		std::cout << "[Error] Model::get_partial_data(const std::string& mask) was called but vertex structure was empty" << std::endl;
		ASSERT(false);
	}

	if (mask.size() > total_entry_per_vertex ) {
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

	std::vector<float> partial_data;
	partial_data.reserve(partial_data_size);

	for (unsigned int i = 0; i < vertex_data.size(); i += total_entry_per_vertex) {
		for (const unsigned int& selected_index : selected_structure) {
			partial_data.push_back(vertex_data[i + selected_index]);
		}
	}

	return partial_data;
}
*/