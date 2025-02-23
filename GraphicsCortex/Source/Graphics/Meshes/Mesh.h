#pragma once

#include "gl/glew.h"
#include <memory>
#include <unordered_map>

#include "Buffer.h"
#include "NTree.h"
#include "Model.h"
#include "IndexBufferEnums.h"

typedef uint32_t mesh_t;

class Mesh {
public:

	static const uint32_t null_node_name = -1;
	static const uint32_t root_node_name = 0;

	class SingleMesh {
	public:
		SingleMesh() = default;	// ?

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
		SingleMesh(Mesh* owner);
		Mesh* owner = nullptr;
	};

	class Node {
	public:
		uint32_t node_name;
		std::vector<uint32_t> submeshes;
		std::vector<uint32_t> childnodes;

		bool load_model(const Model& model, uint32_t submeshes_begin);
		bool load_model(const Model::Node& submodel, uint32_t submeshes_begin);
		bool load_model(const SingleModel& single_model, uint32_t submeshes_begin);

		bool add_submesh(uint32_t submesh_name);
		bool add_submeshes(uint32_t submesh_begin, size_t submesh_count);
		bool remove_submesh(uint32_t submesh_name);
		bool remove_submeshes(uint32_t submesh_begin, size_t submesh_count);

		bool add_childnode(uint32_t node_name);
		bool remove_childnode(uint32_t node_name);

		void clear();

	private:
		friend Mesh;

		Mesh* owner;
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
	mesh_t add_mesh(const SingleModel& single_model);
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
	Node& operator[](uint32_t node_name);
	void clear_nodes();

	void merge_all_nodes();

private:

	std::shared_ptr<VertexAttributeBuffer> vab = nullptr;
	std::shared_ptr<Buffer> index_buffer = nullptr;
	IndexType index_buffer_type;

	size_t vertex_size = 0;
	size_t index_size = 0;

	node_t next_node_name = root_node_name;
	node_t generate_node_name();

	std::vector<SingleMesh> single_meshes;
	std::unordered_map<node_t, Node> name_to_nodes;
};