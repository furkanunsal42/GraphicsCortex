#pragma once

#include <vector>
#include <string>
#include <stdint.h>
#include <memory>
#include <unordered_map>
#include <filesystem>
#include "glm.hpp"

#include "IndexBufferEnums.h"

class Buffer;
class VertexAttributeBuffer;

struct SingleModel {
	friend Buffer;
public:

	SingleModel() = default;
	SingleModel(const SingleModel& other) = default;
	SingleModel(SingleModel&& other);
	SingleModel& operator=(const SingleModel& other) = default;
	SingleModel& operator=(SingleModel&& other);
	~SingleModel() = default;

	std::vector<glm::vec3> verticies;
	std::vector<glm::vec3> vertex_normals;
	std::vector<glm::vec3> vertex_tangents;
	std::vector<glm::vec2> texture_coordinates_0;
	std::vector<glm::vec2> texture_coordinates_1;
	std::vector<glm::vec4> vertex_colors;
	std::vector<glm::ivec4> bone_indicies;
	std::vector<glm::vec4> bone_weights;
	std::vector<uint32_t> indicies;
	PrimitiveType primitive = PrimitiveType::triangle;

	//void load_model(const std::filesystem::path& path, uint32_t submodel_index);
	//void load_model(const std::filesystem::path& path, uint32_t submodels_begin_index, uint32_t submodel_count);
	void clear();

	size_t get_min_vertex_count_nonzero() const;
	size_t get_min_vertex_count() const;
	size_t get_max_vertex_count() const;
	size_t get_index_count() const;
	size_t get_primitive_count() const;

	std::unique_ptr<Buffer> create_vertex_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_vertex_buffer(size_t vertex_offset_count = 0) const;
	std::unique_ptr<Buffer> create_normal_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_normal_buffer(size_t vertex_offset_count = 0) const;
	std::unique_ptr<Buffer> create_tangent_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_tangent_buffer(size_t vertex_offset_count = 0) const;
	std::unique_ptr<Buffer> create_uv0_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_uv0_buffer(size_t vertex_offset_count = 0) const;
	std::unique_ptr<Buffer> create_uv1_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_uv1_buffer(size_t vertex_offset_count = 0) const;
	//std::unique_ptr<Buffer> create_uv_merged_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	//std::unique_ptr<Buffer> create_uv_merged_buffer(size_t vertex_offset_count = 0) const;
	std::unique_ptr<Buffer> create_vertex_color_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_vertex_color_buffer(size_t vertex_offset_count = 0) const;
	std::unique_ptr<Buffer> create_bone_indicies_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_bone_indicies_buffer(size_t vertex_offset_count = 0) const;
	std::unique_ptr<Buffer> create_bone_weights_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_bone_weights_buffer(size_t vertex_offset_count = 0) const;
	std::unique_ptr<Buffer> create_index_buffer(IndexType index_type, size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_index_buffer(IndexType index_type = IndexType::i_ui32, size_t vertex_offset_count = 0) const;

	std::unique_ptr<VertexAttributeBuffer> create_vertex_attribute_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<VertexAttributeBuffer> create_vertex_attribute_buffer(size_t vertex_offset_count = 0) const;
};
