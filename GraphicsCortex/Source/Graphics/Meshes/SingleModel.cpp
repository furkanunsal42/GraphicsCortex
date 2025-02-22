#include <algorithm>

#include "SingleModel.h"
#include "Debuger.h"
#include "Buffer.h"

#include <iostream>
#include <fstream>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

void SingleModel::load_model(const std::filesystem::path& path, uint32_t submodel_index)
{
	Assimp::Importer importer;

	std::string path_string((char*)path.c_str());
	const aiScene* scene = importer.ReadFile(path_string,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (scene == nullptr) {
		std::cout << "[Import Error] SingleModel::load_model() from \""<< path_string <<"\" failed with message: " << (importer.GetErrorString()) << std::endl;
		return;
	}

}

void SingleModel::load_model(const std::filesystem::path& path, uint32_t submodels_begin_index, uint32_t submodel_count)
{

}

void SingleModel::clear()
{
	verticies.clear();
	vertex_normals.clear();
	vertex_tangents.clear();
	texture_coordinates.clear();
	vertex_colors.clear();
	bone_indicies.clear();
	bone_weights.clear();
	indicies.clear();
}

std::unique_ptr<Buffer> SingleModel::create_vertex_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::vec3 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, verticies);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel::create_vertex_buffer(size_t vertex_offset_count) const
{
	return create_vertex_buffer(vertex_offset_count, 0, verticies.size());
}

std::unique_ptr<Buffer> SingleModel::create_normal_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::vec3 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, vertex_normals);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel::create_normal_buffer(size_t vertex_offset_count) const
{
	return create_normal_buffer(vertex_offset_count, 0, vertex_normals.size());
}

std::unique_ptr<Buffer> SingleModel::create_tangent_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::vec3 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, vertex_tangents);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel::create_tangent_buffer(size_t vertex_offset_count) const
{
	return create_tangent_buffer(vertex_offset_count, 0, vertex_tangents.size());
}

std::unique_ptr<Buffer> SingleModel::create_uv_merged_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::vec4 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, texture_coordinates);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel::create_uv_merged_buffer(size_t vertex_offset_count) const
{
	return create_uv_merged_buffer(vertex_offset_count, 0, texture_coordinates.size());
}

std::unique_ptr<Buffer> SingleModel::create_vertex_color_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::vec4 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, vertex_colors);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel::create_vertex_color_buffer(size_t vertex_offset_count) const
{
	return create_vertex_color_buffer(vertex_offset_count, 0, vertex_colors.size());
}

std::unique_ptr<Buffer> SingleModel::create_bone_indicies_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::ivec4 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, bone_indicies);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel::create_bone_indicies_buffer(size_t vertex_offset_count) const
{
	return create_bone_indicies_buffer(vertex_offset_count, 0, bone_indicies.size());
}

std::unique_ptr<Buffer> SingleModel::create_bone_weights_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::vec4 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, bone_weights);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel::create_bone_weights_buffer(size_t vertex_offset_count) const
{
	return create_bone_weights_buffer(vertex_offset_count, 0, bone_weights.size());
}

std::unique_ptr<Buffer> SingleModel::create_index_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef uint32_t attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, indicies);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel::create_index_buffer(size_t vertex_offset_count) const
{
	return create_index_buffer(vertex_offset_count, 0, indicies.size());
}
