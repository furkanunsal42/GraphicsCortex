#include <algorithm>

#include "SingleModel.h"
#include "Debuger.h"
#include "StandardBuffer.h"

/*
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
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, _verticies);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel2::create_vertex_buffer(size_t vertex_offset_count) const
{
	return create_vertex_buffer(vertex_offset_count, 0, _verticies.size());
}

std::unique_ptr<Buffer> SingleModel2::create_normal_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::vec3 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, _vertex_normals);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel2::create_normal_buffer(size_t vertex_offset_count) const
{
	return create_normal_buffer(vertex_offset_count, 0, _vertex_normals.size());
}

std::unique_ptr<Buffer> SingleModel2::create_tangent_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::vec3 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, _vertex_tangents);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel2::create_tangent_buffer(size_t vertex_offset_count) const
{
	return create_tangent_buffer(vertex_offset_count, 0, _vertex_tangents.size());
}

std::unique_ptr<Buffer> SingleModel2::create_uv_merged_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::vec4 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, _texture_coordinates);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel2::create_uv_merged_buffer(size_t vertex_offset_count) const
{
	return create_uv_merged_buffer(vertex_offset_count, 0, _texture_coordinates.size());
}

std::unique_ptr<Buffer> SingleModel2::create_vertex_color_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::vec4 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, _vertex_colors);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel2::create_vertex_color_buffer(size_t vertex_offset_count) const
{
	return create_vertex_color_buffer(vertex_offset_count, 0, _vertex_colors.size());
}

std::unique_ptr<Buffer> SingleModel2::create_bone_indicies_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::ivec4 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, _bone_indicies);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel2::create_bone_indicies_buffer(size_t vertex_offset_count) const
{
	return create_bone_indicies_buffer(vertex_offset_count, 0, _bone_indicies.size());
}

std::unique_ptr<Buffer> SingleModel2::create_bone_weights_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef glm::vec4 attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, _bone_weights);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel2::create_bone_weights_buffer(size_t vertex_offset_count) const
{
	return create_bone_weights_buffer(vertex_offset_count, 0, _bone_weights.size());
}

std::unique_ptr<Buffer> SingleModel2::create_index_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const
{
	typedef uint32_t attribute_type;
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(vertex_count * sizeof(attribute_type), Buffer::GPU_BUFFER);
	buffer->load_data(buffer_offset_in_bytes / sizeof(attribute_type), vertex_offset_count, vertex_count, _indicies);
	return buffer;
}

std::unique_ptr<Buffer> SingleModel2::create_index_buffer(size_t vertex_offset_count) const
{
	return create_index_buffer(vertex_offset_count, 0, _indicies.size());
}

size_t SingleModel2::get_verticies_revision_count()
{
	return _revision_counter_verticies;
}

size_t SingleModel2::get_vertex_normals_revision_count()
{
	return _revision_counter_vertex_normals;
}

size_t SingleModel2::get_vertex_tangents_revision_count()
{
	return _revision_counter_vertex_tangents;
}

size_t SingleModel2::get_texture_coordinates_revision_count()
{
	return _revision_counter_texture_coordinates;
}

size_t SingleModel2::get_vertex_colors_revision_count()
{
	return _revision_counter_vertex_colors;
}

size_t SingleModel2::get_bone_indicies_revision_count()
{
	return _revision_counter_bone_indicies;
}

size_t SingleModel2::get_bone_weights_revision_count()
{
	return _revision_counter_bone_weights;
}

size_t SingleModel2::get_indicies_revision_count()
{
	return _revision_counter_indicies;
}

size_t SingleModel2::get_primitive_revision_count()
{
	return _revision_counter_primitive;
}
*/