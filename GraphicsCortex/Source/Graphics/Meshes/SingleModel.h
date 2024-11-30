#pragma once

#include <vector>
#include <string>
#include <stdint.h>
#include <memory>
#include <unordered_map>
#include "glm.hpp"

#include "IndexBufferEnums.h"

class Buffer;

struct SingleModel2 {
	friend Buffer;
public:

	std::vector<glm::vec3>& unlock_verticies();
	std::vector<glm::vec3>& unlock_vertex_normals();
	std::vector<glm::vec3>& unlock_vertex_tangents();
	std::vector<glm::vec4>& unlock_texture_coordinates();
	std::vector<glm::vec4>& unlock_vertex_colors();
	std::vector<glm::ivec4>& unlock_bone_indicies();
	std::vector<glm::vec4>& unlock_bone_weights();
	std::vector<uint32_t>& unlock_indicies();
	void set_primitive(PrimitiveType primitive);

	const std::vector<glm::vec3>& read_verticies() const;
	const std::vector<glm::vec3>& read_vertex_normals() const;
	const std::vector<glm::vec3>& read_vertex_tangents() const;
	const std::vector<glm::vec4>& read_texture_coordinates() const;
	const std::vector<glm::vec4>& read_vertex_colors() const;
	const std::vector<glm::ivec4>& read_bone_indicies() const;
	const std::vector<glm::vec4>& read_bone_weights() const;
	const std::vector<uint32_t>& read_indicies() const;
	PrimitiveType get_primitive() const;

	std::unique_ptr<Buffer> create_vertex_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_vertex_buffer(size_t vertex_offset_count = 0) const;
	std::unique_ptr<Buffer> create_normal_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_normal_buffer(size_t vertex_offset_count = 0) const;
	std::unique_ptr<Buffer> create_tangent_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_tangent_buffer(size_t vertex_offset_count = 0) const;
	//std::unique_ptr<Buffer> create_uv0_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	//std::unique_ptr<Buffer> create_uv0_buffer(size_t vertex_offset_count = 0) const;
	//std::unique_ptr<Buffer> create_uv1_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	//std::unique_ptr<Buffer> create_uv1_buffer(size_t vertex_offset_count = 0) const;
	std::unique_ptr<Buffer> create_uv_merged_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_uv_merged_buffer(size_t vertex_offset_count = 0) const;
	std::unique_ptr<Buffer> create_vertex_color_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_vertex_color_buffer(size_t vertex_offset_count = 0) const;
	std::unique_ptr<Buffer> create_bone_indicies_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_bone_indicies_buffer(size_t vertex_offset_count = 0) const;
	std::unique_ptr<Buffer> create_bone_weights_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_bone_weights_buffer(size_t vertex_offset_count = 0) const;
	std::unique_ptr<Buffer> create_index_buffer(size_t vertex_offset_count, size_t buffer_offset_in_bytes, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_index_buffer(size_t vertex_offset_count = 0) const;

	void load_model(const std::string& path, uint32_t submodel_index);
	void load_model(const std::string& path, uint32_t submodels_begin_index, uint32_t submodel_count);

	size_t get_verticies_revision_count();
	size_t get_vertex_normals_revision_count();
	size_t get_vertex_tangents_revision_count();
	size_t get_texture_coordinates_revision_count();
	size_t get_vertex_colors_revision_count();
	size_t get_bone_indicies_revision_count();
	size_t get_bone_weights_revision_count();
	size_t get_indicies_revision_count();
	size_t get_primitive_revision_count();

private:
	std::vector<glm::vec3> _verticies;
	std::vector<glm::vec3> _vertex_normals;
	std::vector<glm::vec3> _vertex_tangents;
	std::vector<glm::vec4> _texture_coordinates;
	std::vector<glm::vec4> _vertex_colors;
	std::vector<glm::ivec4> _bone_indicies;
	std::vector<glm::vec4> _bone_weights;
	std::vector<uint32_t> _indicies;
	PrimitiveType _primitive = PrimitiveType::triangle;

	size_t _revision_counter_verticies = 0;
	size_t _revision_counter_vertex_normals = 0;
	size_t _revision_counter_vertex_tangents = 0;
	size_t _revision_counter_texture_coordinates = 0;
	size_t _revision_counter_vertex_colors = 0;
	size_t _revision_counter_bone_indicies = 0;
	size_t _revision_counter_bone_weights = 0;
	size_t _revision_counter_indicies = 0;
	size_t _revision_counter_primitive = 0;
};
