#include <algorithm>

#include "SingleModel.h"
#include "Debuger.h"
#include "Buffer.h"

#include <iostream>
#include <fstream>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

SingleModel::SingleModel(SingleModel&& other) :
	verticies				(std::move(other.verticies				)),
	vertex_normals			(std::move(other.vertex_normals			)),
	vertex_tangents			(std::move(other.vertex_tangents		)),
	texture_coordinates_0	(std::move(other.texture_coordinates_0	)),
	texture_coordinates_1	(std::move(other.texture_coordinates_1	)),
	vertex_colors			(std::move(other.vertex_colors			)),
	bone_indicies			(std::move(other.bone_indicies			)),
	bone_weights			(std::move(other.bone_weights			)),
	indicies				(std::move(other.indicies				)),
	primitive				(other.primitive)
{
}

SingleModel& SingleModel::operator=(SingleModel&& other)
{
	std::swap(verticies				, other.verticies				);
	std::swap(vertex_normals		, other.vertex_normals			);
	std::swap(vertex_tangents		, other.vertex_tangents			);
	std::swap(texture_coordinates_0	, other.texture_coordinates_0	);
	std::swap(texture_coordinates_1	, other.texture_coordinates_1	);
	std::swap(vertex_colors			, other.vertex_colors			);
	std::swap(bone_indicies			, other.bone_indicies			);
	std::swap(bone_weights			, other.bone_weights			);
	std::swap(indicies				, other.indicies				);
	std::swap(primitive				, other.primitive				);

	return *this;
}

void SingleModel::clear()
{
	verticies.clear();
	vertex_normals.clear();
	vertex_tangents.clear();
	texture_coordinates_0.clear();
	texture_coordinates_1.clear();
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

std::unique_ptr<Buffer> SingleModel::create_uv0_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::vec2 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, texture_coordinates_0);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel::create_uv0_buffer(size_t vertex_offset_count) const
{
	return create_uv0_buffer(vertex_offset_count, 0, vertex_tangents.size());
}

std::unique_ptr<Buffer> SingleModel::create_uv1_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::vec2 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, texture_coordinates_1);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel::create_uv1_buffer(size_t vertex_offset_count) const
{
	return create_uv1_buffer(vertex_offset_count, 0, vertex_tangents.size());
}

// std::unique_ptr<Buffer> SingleModel::create_uv_merged_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
// {
// 	typedef glm::vec4 attribute_type;
// 	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
// 	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, texture_coordinates);
// 	return buffer;
// }
// 
// std::unique_ptr<Buffer> SingleModel::create_uv_merged_buffer(size_t vertex_offset_count) const
// {
// 	return create_uv_merged_buffer(vertex_offset_count, 0, texture_coordinates.size());
// }

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
