#pragma once

#include "gl/glew.h"
#include <memory>
#include <unordered_map>

#include "Buffer.h"
#include "NTree.h"
#include "Model.h"
#include "IndexBufferEnums.h"

class Mesh {
public:

	static const uint32_t null_submesh_name = -1;
	static const uint32_t null_node_name = -1;
	static const uint32_t root_node_name = 0;

	struct SubmeshInfo {
		SubmeshInfo(size_t vertex_buffers_offset, size_t vertex_buffers_size, size_t index_buffer_offset, size_t index_buffer_size, PrimitiveType primitive);

		size_t vertex_buffers_offset;
		size_t vertex_buffers_size;
		size_t index_buffer_offset;
		size_t index_buffer_size;

		PrimitiveType primitive;
	};

	struct Node {
		uint32_t node_name;
		std::vector<uint32_t> submeshes;
		std::vector<uint32_t> childnodes;
	};

	struct _ProxyNode {
		friend Mesh;
	public:

		bool load_model(const Model& model, uint32_t submeshes_begin);
		bool load_model(const Model::_ProxyNode& submodel, uint32_t submeshes_begin);
		bool load_model(const SingleModel& single_model, uint32_t submeshes_begin);

		bool add_submesh(uint32_t submesh_name);
		bool add_submeshes(uint32_t submesh_begin, size_t submesh_count);
		bool remove_submesh(uint32_t submesh_name);
		bool remove_submeshes(uint32_t submesh_begin, size_t submesh_count);

		bool add_childnode(uint32_t node_name);
		bool remove_childnode(uint32_t node_name);

		std::vector<uint32_t>& childnodes;
		std::vector<uint32_t>& submeshes;

		void clear();

	private:
		_ProxyNode();

		Mesh& _owner_mesh;
		uint32_t _node_name;
	};

	// buffer management
	void set_position_buffer(std::shared_ptr<Buffer> position_buffer);
	void set_normal_buffer(std::shared_ptr<Buffer> normal_buffer);
	void set_texture_coordinate_buffer(std::shared_ptr<Buffer> texture_coordinate_buffer);
	void set_color_buffer(std::shared_ptr<Buffer> color_buffer);
	void set_index_buffer(std::shared_ptr<Buffer> index_buffer);
	void set_vertex_attribute_buffer(std::shared_ptr<VertexAttributeBuffer> vertex_attribute_buffer);
	
	std::shared_ptr<Buffer> get_position_buffer();
	std::shared_ptr<Buffer> get_normal_buffer();
	std::shared_ptr<Buffer> get_texture_coordinate_buffer();
	std::shared_ptr<Buffer> get_color_buffer();
	std::shared_ptr<Buffer> get_index_buffer();
	std::shared_ptr<VertexAttributeBuffer> get_vertex_attribute_buffer();

	// submeshes
	size_t get_submesh_count();
	bool does_submesh_exist(uint32_t submesh_name);
	uint32_t insert_submeshes(const Model& model);
	uint32_t insert_submeshes(const Model::_ProxyNode& submodel);
	uint32_t insert_submesh(const SingleModel& single_model);
	void erase_submesh(uint32_t submesh_name);
	void erase_submesh(uint32_t submesh_name, size_t submesh_count);
	SubmeshInfo& get_submesh(uint32_t submesh_name);
	bool set_submesh(uint32_t submodel_name, SubmeshInfo submesh);

	// nodes
	size_t get_node_count();
	bool does_node_exist(uint32_t node_name);
	_ProxyNode operator[](uint32_t node_name);
	void create_node(uint32_t node_name);
	void delete_node(uint32_t node_name);

private:

	std::shared_ptr<VertexAttributeBuffer> vao;
	std::shared_ptr<Buffer> _index_buffer;
	IndexType _index_buffer_type;

	size_t _vertex_size = 0;
	size_t _index_size = 0;

	std::unordered_map<uint32_t, SubmeshInfo> _name_to_singlemesh;
	std::unordered_map<uint32_t, Node> _nodes;
};