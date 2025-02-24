#pragma once

#include <vector>
#include <string>
#include <stdint.h>
#include <memory>
#include <span>
#include <unordered_map>
#include <stdint.h>
#include <filesystem>
#include <functional>

#include "mat4x4.hpp"

#include "IndexBufferEnums.h"
#include "SingleModel.h"

class Buffer;
class Mesh;
class VertexAttributeBuffer;

typedef uint32_t model_t;
typedef uint32_t node_t;

class Model {
	friend Mesh;
public:

	static const node_t null_node_name = -1;
	static const node_t root_node_name = 0;
	
	class Node {
	public:
		Node() = default;	// ?

		node_t get_name();
		node_t get_parent();
		void set_parent(node_t parent);

		std::span<node_t> get_children();
		void add_child(node_t child);
		node_t create_child();

		std::span<model_t> get_submodels();
		void add_submodel(model_t model);

		glm::mat4 get_transform();
		void set_transform(glm::mat4 transform);

		Model* get_model();

		void traverse(const std::function<void(Node&, glm::mat4&)>& lambda);

		//std::unique_ptr<Buffer> create_vertex_buffer() const;
		//std::unique_ptr<Buffer> create_normal_buffer() const;
		//std::unique_ptr<Buffer> create_tangent_buffer() const;
		//std::unique_ptr<Buffer> create_uv0_buffer() const;
		//std::unique_ptr<Buffer> create_uv1_buffer() const;
		////std::unique_ptr<Buffer> create_uv_merged_buffer() const;
		//std::unique_ptr<Buffer> create_vertex_color_buffer() const;
		//std::unique_ptr<Buffer> create_bone_indicies_buffer() const;
		//std::unique_ptr<Buffer> create_bone_weights_buffer() const;
		//std::unique_ptr<Buffer> create_index_buffer() const;
		//
		//std::unique_ptr<VertexAttributeBuffer> create_vertex_attribute_buffer() const;

	private:
		friend Model;
		Node(Model* owner, node_t node_name, node_t parent, glm::mat4 transform = glm::mat4(1));

		Model* owner = nullptr;
		node_t name = null_node_name;
		node_t parent = null_node_name;
		std::vector<node_t> children;
		std::vector<model_t> submodels;
		glm::mat4 transform = glm::mat4(1.0f);
	};

	Model() = default;
	Model(const Model& other) = default;
	Model(Model&& other);
	Model& operator=(const Model& other) = default;
	Model& operator=(Model&& other);
	~Model() = default;
	
	void clear();
	//void load_model(const std::filesystem::path& filepath);
	
	bool does_model_exist(model_t model_name);
	size_t get_model_count();
	model_t add_model(const SingleModel& single_model);
	model_t add_model(SingleModel&& single_model);
	SingleModel* get_model(model_t submodel_name);
	std::span<SingleModel> get_models();
	void clear_models();

	void set_index_type(IndexType type);
	IndexType get_index_type();

	// nodes
	size_t get_node_count();
	bool does_node_exist(node_t node_name);
	node_t add_node(node_t parent);
	Node* get_node(node_t node_name);
	Node& operator[](node_t node_name); // error if given node name not found
	void clear_nodes();

	void traverse(const std::function<void(Node&, glm::mat4&)>& lambda, node_t start_node = root_node_name);

	//void save_to_disk(const std::filesystem::path& output_filepath);

	size_t get_models_min_vertex_count_nonzero() const;
	size_t get_models_min_vertex_count() const;
	size_t get_models_max_vertex_count() const;
	size_t get_models_index_count() const;
	size_t get_models_primitive_count() const;

	std::unique_ptr<Buffer> create_vertex_buffer() const;
	std::unique_ptr<Buffer> create_normal_buffer() const;
	std::unique_ptr<Buffer> create_tangent_buffer() const;
	std::unique_ptr<Buffer> create_uv0_buffer() const;
	std::unique_ptr<Buffer> create_uv1_buffer() const;
	//std::unique_ptr<Buffer> create_uv_merged_buffer() const;
	std::unique_ptr<Buffer> create_vertex_color_buffer() const;
	std::unique_ptr<Buffer> create_bone_indicies_buffer() const;
	std::unique_ptr<Buffer> create_bone_weights_buffer() const;
	std::unique_ptr<Buffer> create_index_buffer() const;

	std::unique_ptr<VertexAttributeBuffer> create_vertex_attribute_buffer() const;

private:

	node_t _next_node_name = root_node_name;
	node_t _generate_node_name();
	std::unordered_map<node_t, Node> _name_to_node;

	std::vector<SingleModel> single_models;
	IndexType index_buffer_type = IndexType::i_ui32;
};

