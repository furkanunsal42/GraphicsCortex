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

	bool operator==(const SingleModel2& other) const;
	void operator=(const SingleModel2& other);
	SingleModel2(const SingleModel2& other);

	std::vector<glm::vec3>& unlock_verticies();
	std::vector<glm::vec3>& unlock_vertex_normals();
	std::vector<glm::vec2>& unlock_texture_coordinates();
	std::vector<glm::vec4>& unlock_vertex_colors();
	std::vector<uint32_t>& unlock_indicies();
	void set_primitive(PrimitiveType primitive);
	
	const std::vector<glm::vec3>& read_verticies() const;
	const std::vector<glm::vec3>& read_vertex_normals() const;
	const std::vector<glm::vec2>& read_texture_coordinates() const;
	const std::vector<glm::vec4>& read_vertex_colors() const;
	const std::vector<uint32_t>& read_indicies() const;
	PrimitiveType get_primitive() const;

	std::unique_ptr<Buffer> create_vertex_buffer(size_t vertex_offset, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_vertex_buffer(size_t vertex_offset = 0) const;
	std::unique_ptr<Buffer> create_normal_buffer(size_t vertex_offset, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_normal_buffer(size_t vertex_offset = 0) const;
	std::unique_ptr<Buffer> create_uv_buffer(size_t vertex_offset, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_uv_buffer(size_t vertex_offset = 0) const;
	std::unique_ptr<Buffer> create_vertex_color_buffer(size_t vertex_offset, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_vertex_color_buffer(size_t vertex_offset = 0) const;
	std::unique_ptr<Buffer> create_index_buffer(size_t vertex_offset, size_t vertex_count) const;
	std::unique_ptr<Buffer> create_index_buffer(size_t vertex_offset = 0) const;

	void load_model(const std::string& path, uint32_t submodel_index);
	void load_model(const std::string& path, uint32_t submodels_begin_index, uint32_t submodel_count);

private:
	std::vector<glm::vec3> _verticies;
	std::vector<glm::vec3> _vertex_normals;
	std::vector<glm::vec3> _vertex_tangents;
	std::vector<glm::vec2> _texture_coordinates;
	std::vector<glm::vec4> _vertex_colors;
	std::vector<glm::ivec4> _bone_indicies;
	std::vector<glm::vec4> _bone_weights;
	std::vector<uint32_t> _indicies;
	PrimitiveType _primitive = PrimitiveType::triangle;

	size_t _revision_counter_verticies = 0;
	size_t _revision_counter_vertex_normals = 0;
	size_t _revision_counter_texture_coordinates = 0;
	size_t _revision_counter_vertex_colors = 0;
	size_t _revision_counter_indicies = 0;
	size_t _revision_counter_primitive = 0;
};

class Model2 {
public:

	struct Node {
	public:
		bool operator==(const Node& other) const;

		uint32_t name;
		std::vector<size_t> submodels;
		std::vector<size_t> childnodes;
		glm::mat4 transform = glm::identity<glm::mat4>();

		size_t _childnodes_hash;
	};


	struct _ProxyNode {
		friend Model2;
	public:
		
		void operator=(const _ProxyNode& other);
		bool operator==(const Node& other) const;
		bool operator==(const _ProxyNode& other) const;

		bool add_submodel(std::shared_ptr<SingleModel2> submodel);
		bool add_submodel(uint32_t submodel_name);
		bool remove_submodel(std::weak_ptr<SingleModel2> submodel);
		bool remove_submodel(uint32_t submodel_name);

		bool add_childnode(uint32_t node_name);
		bool remove_childnode(uint32_t node_name);

		const std::vector<uint32_t>& childnode;
		const std::vector<uint32_t>& submodels;
		glm::mat4& transform;

		std::unique_ptr<Buffer> create_vertex_buffer();
		std::unique_ptr<Buffer> create_normal_buffer();
		std::unique_ptr<Buffer> create_uv_buffer();
		std::unique_ptr<Buffer> create_vertex_color_buffer();
		std::unique_ptr<Buffer> create_index_buffer();

		void clear();

		void load_model(const std::string& path);
		void load_model_async(const std::string& path);
		void save_to_disc(const std::string& output_filepath);

	private:
		_ProxyNode(Model2& owner_model, uint32_t name, size_t id, std::vector<uint32_t>& childnodes, std::vector<uint32_t>& submodels);

		Model2& _owner_model;
		uint32_t _node_name;
	};

	Model2(const std::string& filepath);
	Model2(std::shared_ptr<SingleModel2> single_model);
	Model2() = default;
	~Model2() = default;
	void clear();

	void load_model(const std::string& filepath);
	void load_model_async(const std::string& filepath);

	// submodels
	size_t get_submodel_count() const;
	bool does_submodel_exist(std::weak_ptr<SingleModel2> submodel) const;
	uint32_t insert_submodel(std::shared_ptr<SingleModel2> submodel);
	uint32_t insert_submodel(SingleModel2&& submodel);
	void erase_submodel(std::weak_ptr<SingleModel2> submodel);
	void erase_submodel(uint32_t submodel_name);
	bool set_submodel(uint32_t submodel_name, std::shared_ptr<SingleModel2> new_submodel);
	bool set_submodel(uint32_t submodel_name, SingleModel2&& new_submodel);
	uint32_t get_submodel_name(std::weak_ptr<SingleModel2> submodel) const;
	std::shared_ptr<SingleModel2> get_submodel(uint32_t submodel_name);

	// nodes
	size_t get_node_count();
	bool does_node_exist(uint32_t node_name);
	_ProxyNode operator[](uint32_t node_name);
	void create_node(uint32_t node_name);
	void delete_node(uint32_t node_name);

	void save_to_disc(const std::string& output_filepath);

	std::unique_ptr<Buffer> create_vertex_buffer();
	std::unique_ptr<Buffer> create_normal_buffer();
	std::unique_ptr<Buffer> create_uv_buffer();
	std::unique_ptr<Buffer> create_vertex_color_buffer();
	std::unique_ptr<Buffer> create_index_buffer();

private:

	uint32_t _next_submodel_name = 0;
	uint32_t _generate_submodel_name();

	std::unordered_map<std::shared_ptr<SingleModel2>, uint32_t> _submodel_to_name;
	std::unordered_map<uint32_t, std::shared_ptr<SingleModel2>> _name_to_submodel;

	std::unordered_map<uint32_t, Node> _name_to_node;
	std::unordered_map<size_t, uint32_t> _id_to_node;
};