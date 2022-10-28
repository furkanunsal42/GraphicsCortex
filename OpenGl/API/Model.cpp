#include "Model.h"

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

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

void Model::load_model(const std::string& filepath) {
	
	clear_ram();

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

void Model::clear_ram() {
	vertex_data.clear();
	index_data.clear();
}