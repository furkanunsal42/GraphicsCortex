#pragma once

#include "gl/glew.h"
#include <memory>
#include <unordered_map>
#include <functional>

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
		SingleMesh(
			size_t vertex_offset = 0, 
			size_t vertex_count = 0,  
			size_t index_offset = 0, 
			size_t index_count = 0, 
			PrimitiveType primitive = PrimitiveType::triangle, 
			IndexType index_type = IndexType::i_ui32
			);

		size_t vertex_offset = 0;
		size_t vertex_count = 0;
		size_t index_offset = 0;
		size_t index_count = 0;
		PrimitiveType primitive = PrimitiveType::triangle;
		IndexType index_type = IndexType::i_ui32;

		Mesh* get_owner();

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
		void add_submesh(mesh_t submesh);

		glm::mat4 get_transform();
		void set_transform(glm::mat4 transform);

		void traverse(const std::function<void(Node&, glm::mat4&)>& lambda);
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
	Mesh(Model& model);
	Mesh(Model&& model);
	Mesh(const SingleModel single_model, IndexType type = IndexType::i_ui32);

	void load_model(Model& model);
	void load_model(Model&& model);
	void load_model(const SingleModel& single_model, IndexType type = IndexType::i_ui32);

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

	//void set_index_type(IndexType type);
	//IndexType get_index_type();

	// nodes
	size_t get_node_count();
	bool does_node_exist(node_t node_name);
	node_t add_node(node_t parent);
	Node* get_node(node_t node_name);
	Node& operator[](node_t node_name);
	void clear_nodes();

	void traverse(const std::function<void(Node&, glm::mat4&)>& lambda, node_t start_node = root_node_name);

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

	void load_node_structure(Model::Node& start_node);

	node_t next_node_name = root_node_name;
	node_t generate_node_name();

	std::shared_ptr<VertexAttributeBuffer> vab = nullptr;
	std::shared_ptr<Buffer> index_buffer = nullptr;

	std::vector<SingleMesh> single_meshes;
	std::unordered_map<node_t, Node> name_to_nodes;
};