#pragma once

#include "gl/glew.h"
#include <memory>
#include <unordered_map>

#include "mat4x4.hpp"

#include "Buffer.h"
#include "NTree.h"
#include "Model.h"
#include "IndexBufferEnums.h"
#include "VertexAttributeBuffer.h"

typedef uint32_t mesh_t;

class Mesh {
public:

	static const node_t null_node_name = -1;
	static const node_t root_node_name = 0;

	class SingleMesh {
	public:
		SingleMesh() = default;

		size_t vertex_buffer_offset = 0;
		size_t normal_buffer_offset = 0;
		size_t tangent_buffer_offset = 0;
		size_t uv0_buffer_offset = 0;
		size_t uv1_buffer_offset = 0;
		size_t vertex_color_buffer_offset = 0;
		size_t bone_indicies_buffer_offset = 0;
		size_t bone_weights_buffer_offset = 0;
		size_t index_buffer_offset = 0;

		size_t vertex_buffer_size = 0;
		size_t normal_buffer_size = 0;
		size_t tangent_buffer_size = 0;
		size_t uv0_buffer_size = 0;
		size_t uv1_buffer_size = 0;
		size_t vertex_color_buffer_size = 0;
		size_t bone_indicies_buffer_size = 0;
		size_t bone_weights_buffer_size = 0;
		size_t index_buffer_size = 0;

		PrimitiveType primitive = PrimitiveType::triangle;

	private:
		friend Mesh;
		Mesh* owner = nullptr;
	};

	class Node {
	public:
		Node() = default;	// ?

		//bool load_model(const Model& model, uint32_t submeshes_begin);
		//bool load_model(const Model::Node& submodel, uint32_t submeshes_begin);
		//bool load_model(const SingleModel& single_model, uint32_t submeshes_begin);

		node_t get_name();
		node_t get_parent();
		void set_parent(node_t parent);

		std::span<node_t> get_children();
		void add_child(node_t child);
		node_t create_child();

		std::span<mesh_t> get_submeshes();
		void add_submeshes(mesh_t submesh);

		glm::mat4 get_transform();
		void set_transform(glm::mat4 transform);

		Mesh* get_mesh();

	private:
		friend Mesh;
		Node(Mesh* owner, node_t node_name, node_t parent, glm::mat4 transform = glm::mat4(1));

		Mesh* owner = nullptr;
		node_t name = null_node_name;
		node_t parent = null_node_name;
		std::vector<mesh_t> submeshes;
		std::vector<node_t> children;
		glm::mat4 transform = glm::mat4(1.0f);
	};

	Mesh() = default;
	Mesh(const Model& model);
	Mesh(const SingleModel single_model);

	void load_model(const Model& model);
	void load_model(const SingleModel& single_model);

	// buffer management
	void set_index_buffer(std::shared_ptr<Buffer> index_buffer);
	void set_vertex_attribute_buffer(std::shared_ptr<VertexAttributeBuffer> vertex_attribute_buffer);
	std::shared_ptr<Buffer> get_index_buffer();
	std::shared_ptr<VertexAttributeBuffer> get_vertex_attribute_buffer();
	void clear(); 

	// submeshes
	bool does_mesh_exist(mesh_t submesh_name);
	size_t get_mesh_count();
	mesh_t add_mesh(SingleMesh mesh_definition);
	mesh_t add_mesh(SingleMesh&& mesh_definition);
	//mesh_t add_mesh(const SingleModel& single_model);
	//mesh_t add_mesh(const Model& model);
	SingleMesh* get_mesh(mesh_t submesh_name);
	std::span<SingleMesh> get_meshes();
	void clear_meshes();
	//mesh_t add_meshes(const Model& model);

	void set_index_type(IndexType type);
	IndexType get_index_type();

	// nodes
	size_t get_node_count();
	bool does_node_exist(node_t node_name);
	node_t add_node(node_t parent);
	Node* get_node(node_t node_name);
	Node& operator[](node_t node_name);
	void clear_nodes();

	// void merge_all_nodes();

	//std::unique_ptr<Model> get_model();

	static const uint32_t vab_vertex_slot = 0;
	static const uint32_t vab_normal_slot = 1;
	static const uint32_t vab_tangent_slot = 2;
	static const uint32_t vab_uv0_slot = 3;
	static const uint32_t vab_uv1_slot = 4;
	static const uint32_t vab_vertex_color_slot = 5;
	static const uint32_t vab_bone_indicies_slot = 6;
	static const uint32_t vab_bone_weights_slot = 7;

private:

	node_t next_node_name = root_node_name;
	node_t generate_node_name();

	std::shared_ptr<VertexAttributeBuffer> vab = nullptr;
	std::shared_ptr<Buffer> index_buffer = nullptr;
	IndexType index_buffer_type;

	//size_t vertex_size = 0;
	//size_t index_size = 0;

	std::vector<SingleMesh> single_meshes;
	std::unordered_map<node_t, Node> name_to_nodes;
};