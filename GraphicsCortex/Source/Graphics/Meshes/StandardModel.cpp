#include <algorithm>

#include "StandardModel.h"
#include "Debuger.h"
#include "StandardBuffer.h"

std::vector<glm::vec3>& SingleModel2::unlock_verticies()
{
	_revision_counter_verticies++;
	return _verticies;
}

std::vector<glm::vec3>& SingleModel2::unlock_vertex_normals()
{
	_revision_counter_vertex_normals++;
	return _vertex_normals;
}

std::vector<glm::vec3>& SingleModel2::unlock_vertex_tangents()
{
	_revision_counter_vertex_tangents++;
	return _vertex_tangents;
}

std::vector<glm::vec4>& SingleModel2::unlock_texture_coordinates()
{
	_revision_counter_texture_coordinates++;
	return _texture_coordinates;
}

std::vector<glm::vec4>& SingleModel2::unlock_vertex_colors()
{
	_revision_counter_vertex_colors++;
	return _vertex_colors;
}

std::vector<glm::ivec4>& SingleModel2::unlock_bone_indicies()
{
	_revision_counter_bone_indicies++;
	return _bone_indicies;
}

std::vector<glm::vec4>& SingleModel2::unlock_bone_weights()
{
	_revision_counter_bone_weights++;
	return _bone_weights;
}

std::vector<uint32_t>& SingleModel2::unlock_indicies()
{
	_revision_counter_indicies++;
	return _indicies;
}

void SingleModel2::set_primitive(PrimitiveType primitive)
{
	if (primitive == _primitive) return;

	_revision_counter_primitive++;
	_primitive = primitive;
}

const std::vector<glm::vec3>& SingleModel2::read_verticies() const
{
	return _verticies;
}

const std::vector<glm::vec3>& SingleModel2::read_vertex_normals() const
{
	return _vertex_normals;
}

const std::vector<glm::vec3>& SingleModel2::read_vertex_tangents() const
{
	return _vertex_tangents;
}

const std::vector<glm::vec4>& SingleModel2::read_texture_coordinates() const
{
	return _texture_coordinates;
}

const std::vector<glm::vec4>& SingleModel2::read_vertex_colors() const
{
	return _vertex_colors;
}

const std::vector<glm::ivec4>& SingleModel2::read_bone_indicies() const
{
	return _bone_indicies;
}

const std::vector<glm::vec4>& SingleModel2::read_bone_weights() const
{
	return _bone_weights;
}

const std::vector<uint32_t>& SingleModel2::read_indicies() const
{
	return _indicies;
}

PrimitiveType SingleModel2::get_primitive() const
{
	return _primitive;
}

std::unique_ptr<Buffer> SingleModel2::create_vertex_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::vec3 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->set_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, _verticies);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel2::create_vertex_buffer(size_t vertex_offset_count) const
{
	return std::unique_ptr<Buffer>();
}

std::unique_ptr<Buffer> SingleModel2::create_normal_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::vec3 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->set_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, _vertex_normals);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel2::create_normal_buffer(size_t vertex_offset_count) const
{
	return std::unique_ptr<Buffer>();
}

std::unique_ptr<Buffer> SingleModel2::create_tangent_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::vec3 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->set_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, _vertex_tangents);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel2::create_tangent_buffer(size_t vertex_offset_count) const
{
	return std::unique_ptr<Buffer>();
}

std::unique_ptr<Buffer> SingleModel2::create_uv0_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	return std::unique_ptr<Buffer>();
}

std::unique_ptr<Buffer> SingleModel2::create_uv0_buffer(size_t vertex_offset_count) const
{
	return std::unique_ptr<Buffer>();
}

std::unique_ptr<Buffer> SingleModel2::create_uv1_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	return std::unique_ptr<Buffer>();
}

std::unique_ptr<Buffer> SingleModel2::create_uv1_buffer(size_t vertex_offset_count) const
{
	return std::unique_ptr<Buffer>();
}

std::unique_ptr<Buffer> SingleModel2::create_uv_merged_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	return std::unique_ptr<Buffer>();
}

std::unique_ptr<Buffer> SingleModel2::create_uv_merged_buffer(size_t vertex_offset_count) const
{
	return std::unique_ptr<Buffer>();
}

std::unique_ptr<Buffer> SingleModel2::create_vertex_color_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	return std::unique_ptr<Buffer>();
}

std::unique_ptr<Buffer> SingleModel2::create_vertex_color_buffer(size_t vertex_offset_count) const
{
	return std::unique_ptr<Buffer>();
}

std::unique_ptr<Buffer> SingleModel2::create_bone_indicies_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	return std::unique_ptr<Buffer>();
}

std::unique_ptr<Buffer> SingleModel2::create_bone_indicies_buffer(size_t vertex_offset_count) const
{
	return std::unique_ptr<Buffer>();
}

std::unique_ptr<Buffer> SingleModel2::create_bone_weights_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	return std::unique_ptr<Buffer>();
}

std::unique_ptr<Buffer> SingleModel2::create_bone_weights_buffer(size_t vertex_offset_count) const
{
	return std::unique_ptr<Buffer>();
}

std::unique_ptr<Buffer> SingleModel2::create_index_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	return std::unique_ptr<Buffer>();
}

std::unique_ptr<Buffer> SingleModel2::create_index_buffer(size_t vertex_offset_count) const
{
	return std::unique_ptr<Buffer>();
}
