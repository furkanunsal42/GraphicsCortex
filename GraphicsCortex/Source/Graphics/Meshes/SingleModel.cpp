#include <algorithm>

#include "SingleModel.h"
#include "Debuger.h"
#include "Buffer.h"
#include "VertexAttributeBuffer.h"
#include "Mesh.h"

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

size_t SingleModel::get_min_vertex_count_nonzero() const
{
	size_t count = -1;

	if (verticies.size() != 0)				count = std::min(count, verticies.size());
	if (vertex_normals.size() != 0)			count = std::min(count, vertex_normals.size());
	if (vertex_tangents.size() != 0)		count = std::min(count, vertex_tangents.size());
	if (texture_coordinates_0.size() != 0)	count = std::min(count, texture_coordinates_0.size());
	if (texture_coordinates_1.size() != 0)	count = std::min(count, texture_coordinates_1.size());
	if (vertex_colors.size() != 0)		  	count = std::min(count, vertex_colors.size());
	if (bone_indicies.size() != 0)		  	count = std::min(count, bone_indicies.size());
	if (bone_weights.size() != 0)		  	count = std::min(count, bone_weights.size());

	if (count == -1) count = 0;

	return count;
}

size_t SingleModel::get_min_vertex_count() const
{
	size_t count = -1;

	count = std::min(count, verticies.size());
	count = std::min(count, vertex_normals.size());
	count = std::min(count, vertex_tangents.size());
	count = std::min(count, texture_coordinates_0.size());
	count = std::min(count, texture_coordinates_1.size());
	count = std::min(count, vertex_colors.size());
	count = std::min(count, bone_indicies.size());
	count = std::min(count, bone_weights.size());

	return count;
}

size_t SingleModel::get_max_vertex_count() const
{
	size_t count = 0;

	count = std::max(count, verticies.size());
	count = std::max(count, vertex_normals.size());
	count = std::max(count, vertex_tangents.size());
	count = std::max(count, texture_coordinates_0.size());
	count = std::max(count, texture_coordinates_1.size());
	count = std::max(count, vertex_colors.size());
	count = std::max(count, bone_indicies.size());
	count = std::max(count, bone_weights.size());

	return count;
}

size_t SingleModel::get_index_count() const
{
	return indicies.size();
}

size_t SingleModel::get_primitive_count() const
{
	return indicies.size() / get_PrimitiveType_index_count(primitive);
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

std::unique_ptr<Buffer> SingleModel::create_index_buffer(IndexType index_type, size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * get_IndexType_bytes_per_index(index_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / get_IndexType_bytes_per_index(index_type), vertex_offset_count, vertex_count, indicies);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel::create_index_buffer(IndexType index_type, size_t vertex_offset_count) const
{
	return create_index_buffer(index_type, vertex_offset_count, 0, indicies.size());
}

std::unique_ptr<VertexAttributeBuffer> SingleModel::create_vertex_attribute_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	size_t	verticies_count		=	verticies.size();
	size_t	normal_count		=	vertex_normals.size();
	size_t	tangent_count		=	vertex_tangents.size();
	size_t	uv0_count			=	texture_coordinates_0.size();
	size_t	uv1_count			=	texture_coordinates_1.size();
	size_t	vertex_color_count	=	vertex_colors.size();
	size_t	bone_indicies_count =	bone_indicies.size();
	size_t	bone_weights_count	=	bone_weights.size();
	size_t	index_count			=	indicies.size();

	std::unique_ptr<VertexAttributeBuffer> vab = std::make_unique<VertexAttributeBuffer>();
	
	typedef glm::vec3  vertex_attribute_type;
	typedef glm::vec3  normal_attribute_type;
	typedef glm::vec3  tangent_attribute_type;
	typedef glm::vec2  uv0_attribute_type;
	typedef glm::vec2  uv1_attribute_type;
	typedef glm::vec4  vertex_color_attribute_type;
	typedef glm::ivec4 bone_indicies_attribute_type;
	typedef glm::vec4  bone_weights_attribute_type;

	if (verticies_count != 0){
		vab->attach_vertex_buffer(Mesh::vab_vertex_slot, create_vertex_buffer(), sizeof(vertex_attribute_type), 0, 0);
		vab->set_attribute_format(Mesh::vab_vertex_slot, Mesh::vab_vertex_slot, VertexAttributeBuffer::a_f32, 3, 0, true);
	}

	if (normal_count != 0) {
		vab->attach_vertex_buffer(Mesh::vab_normal_slot, create_normal_buffer(), sizeof(normal_attribute_type), 0, 0);
		vab->set_attribute_format(Mesh::vab_normal_slot, Mesh::vab_normal_slot, VertexAttributeBuffer::a_f32, 3, 0, true);
	}

	if (tangent_count != 0) {
		vab->attach_vertex_buffer(Mesh::vab_tangent_slot, create_tangent_buffer(), sizeof(tangent_attribute_type), 0, 0);
		vab->set_attribute_format(Mesh::vab_tangent_slot, Mesh::vab_tangent_slot, VertexAttributeBuffer::a_f32, 3, 0, true);
	}

	if (uv0_count != 0) {
		vab->attach_vertex_buffer(Mesh::vab_uv0_slot, create_uv0_buffer(), sizeof(uv0_attribute_type), 0, 0);
		vab->set_attribute_format(Mesh::vab_uv0_slot, Mesh::vab_uv0_slot, VertexAttributeBuffer::a_f32, 2, 0, true);
	}

	if (uv1_count != 0) {
		vab->attach_vertex_buffer(Mesh::vab_uv1_slot, create_uv1_buffer(), sizeof(uv1_attribute_type), 0, 0);
		vab->set_attribute_format(Mesh::vab_uv1_slot, Mesh::vab_uv1_slot, VertexAttributeBuffer::a_f32, 2, 0, true);
	}

	if (vertex_color_count != 0) {
		vab->attach_vertex_buffer(Mesh::vab_vertex_color_slot, create_vertex_color_buffer(), sizeof(vertex_color_attribute_type), 0, 0);
		vab->set_attribute_format(Mesh::vab_vertex_color_slot, Mesh::vab_vertex_color_slot, VertexAttributeBuffer::a_f32, 4, 0, true);
	}

	if (bone_indicies_count != 0) {
		vab->attach_vertex_buffer(Mesh::vab_bone_indicies_slot, create_bone_indicies_buffer(), sizeof(bone_indicies_attribute_type), 0, 0);
		vab->set_attribute_format(Mesh::vab_bone_indicies_slot, Mesh::vab_bone_indicies_slot, VertexAttributeBuffer::a_i32, 4, 0, true);
	}

	if (bone_weights_count != 0) {
		vab->attach_vertex_buffer(Mesh::vab_bone_weights_slot, create_bone_weights_buffer(), sizeof(bone_weights_attribute_type), 0, 0);
		vab->set_attribute_format(Mesh::vab_bone_weights_slot, Mesh::vab_bone_weights_slot, VertexAttributeBuffer::a_f32, 4, 0, true);
	}

	return vab;
}

std::unique_ptr<VertexAttributeBuffer> SingleModel::create_vertex_attribute_buffer(size_t vertex_offset_count) const
{
	return create_vertex_attribute_buffer(vertex_offset_count, 0, indicies.size());
}
