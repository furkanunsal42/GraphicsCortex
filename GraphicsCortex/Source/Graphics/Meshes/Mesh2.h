#pragma once

#include "gl/glew.h"
#include <memory>
#include <unordered_map>

#include "StandardBuffer.h"
#include "NTree.h"
#include "StandardModel.h"

enum IndexType {
	i_ui8,
	i_ui16,
	i_ui32,
};

class VertexAttributeBuffer {
public:

	enum AttributeType {
		a_f16_fixed,
		a_f16,
		a_f32,
		a_f64,
		a_i8,
		a_ui8,
		a_i16,
		a_ui16,
		a_i32,
		a_ui32,
		a_i_2_10_10_10,
		a_ui_2_10_10_10,
		a_i8_normalized,
		a_ui8_normalized,
		a_i16_normalized,
		a_ui16_normalized,
		a_i32_normalized,
		a_ui32_normalized,
		a_i_2_10_10_10_normalized,
		a_ui_2_10_10_10_normalized,
		a_ui_10f_11f_11f,
	};

	enum PrimitiveType {
		point,
		line,
		line_strip,
		line_loop,
		triangle,
		triangle_strip,
		triangle_fan,

		line_strip_adjacency,
		line_adjacency,
		triangle_strip_adjacency,
		triangle_adjacency,
		patches,
	};

	VertexAttributeBuffer();
	VertexAttributeBuffer(const VertexAttributeBuffer& other) = delete;
	~VertexAttributeBuffer();
	void release();
	void clear();

	void load_model(const SingleModel2& model);

	void bind();
	void unbind();

	void attach_vertex_buffer(int32_t slot, std::shared_ptr<Buffer> vertex_buffer, size_t stride, size_t offset);
	void attach_vertex_buffer(int32_t slot, std::shared_ptr<Buffer> vertex_buffer, AttributeType attribute_type, int32_t element_per_vertex, size_t stride, size_t offset, bool enabled = false);
	void detach_vertex_buffer(int32_t slot);
	
	void set_attribute_format(int32_t slot, AttributeType attribute_type, int32_t element_per_vertex, size_t offset);
	std::shared_ptr<Buffer> get_vertex_buffer(int32_t slot);
	AttributeType get_attribute_type(int32_t slot);
	int32_t get_attribute_element_per_vertex(int32_t slot);
	size_t get_attribute_stride(int32_t slot);
	size_t get_attribute_offset(int32_t slot);

	void enable_attribute(int32_t slot);
	void disable_attribute(int32_t slot);
	bool is_attribute_enabled(int32_t slot);
	void enabled_all_attached_attributes();
	void disable_all_attributes();

	int32_t get_max_attribute_count();
	int32_t get_largest_active_buffer_slot();

private:

	unsigned int id = 0;
	const int32_t _max_attribute_count;

	void _generate_buffer();
	bool _buffer_generated = false;

	struct _buffer_with_structure_info {
	public:
		_buffer_with_structure_info() = default;
		_buffer_with_structure_info(std::shared_ptr<Buffer> buffer, AttributeType attribute_type, int32_t element_per_vertex, size_t stride, size_t offset, bool is_enabled);
		std::shared_ptr<Buffer> _buffer = nullptr;
		AttributeType _attribute_type = AttributeType::a_i8;
		int32_t _element_per_vertex = 0;
		size_t _stride = 0;
		size_t _offset = 0;

		bool _slot_enabled = false;
	};

	std::vector<_buffer_with_structure_info> _vertex_buffers;

};

class Mesh2 {
public:

	struct SubmeshInfo {
		size_t offset;
		size_t size;

		VertexAttributeBuffer::PrimitiveType primitive;
	};

	struct Node {
		uint32_t node_name;
		std::vector<uint32_t> submeshes;
		std::vector<uint32_t> childnodes;
	};

	struct _ProxyNode {
		friend Mesh2;
	public:

		bool load_model(const Model2& model, uint32_t submeshes_begin);
		bool load_model(const Model2::_ProxyNode& submodel, uint32_t submeshes_begin);
		bool load_model(const SingleModel2& single_model, uint32_t submeshes_begin);

		bool add_submodel(uint32_t submodel_name);
		bool remove_submodel(uint32_t submodel_name);

		bool add_childnode(uint32_t node_name);
		bool remove_childnode(uint32_t node_name);

		std::vector<uint32_t>& childnodes;
		std::vector<uint32_t>& submeshes;

		void clear();

	private:
		_ProxyNode();

		Mesh2& _owner_mesh;
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
	uint32_t insert_submeshes(const Model2& model);
	uint32_t insert_submeshes(const Model2::_ProxyNode& submodel);
	uint32_t insert_submesh(const SingleModel2& single_model);
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

	std::unordered_map<uint32_t, SubmeshInfo> _name_to_singlemesh;
	std::unordered_map<uint32_t, Node> _nodes;

};