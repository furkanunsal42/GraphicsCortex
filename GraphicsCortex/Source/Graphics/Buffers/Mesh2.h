#pragma once

#include "gl/glew.h"
#include <memory>
#include <unordered_map>

#include "StandardBuffer.h"

class Model2;

class Mesh2 {
public:

	enum AttributeType {
		f16_fixed,
		f16,
		f32,
		f64,
		i8,
		ui8,
		i16,
		ui16,
		i32,
		ui32,
		i_2_10_10_10,
		ui_2_10_10_10,
		i8_normalized,
		ui8_normalized,
		i16_normalized,
		ui16_normalized,
		i32_normalized,
		ui32_normalized,
		i_2_10_10_10_normalized,
		ui_2_10_10_10_normalized,
		ui_10f_11f_11f,
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

	enum IndexType {
		ui8,
		ui16,
		ui32,
	};

	Mesh2();
	Mesh2(const Mesh2& other) = delete;
	~Mesh2();

	void load_model(const Model2& model);
	std::shared_ptr<Model2> get_model();

	void release();
	void clear();

	void bind();
	void unbind();

	void set_index_buffer(std::shared_ptr<Buffer> index_buffer, IndexType index_type);
	std::shared_ptr<Buffer> get_index_buffer();
	IndexType get_index_buffer_type();

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
		AttributeType _attribute_type = AttributeType::i8;
		int32_t _element_per_vertex = 0;
		size_t _stride = 0;
		size_t _offset = 0;

		bool _slot_enabled = false;
	};

	std::vector<_buffer_with_structure_info> _vertex_buffers;
	std::shared_ptr<Buffer> _index_buffer;

	struct _singlemesh_description {
		size_t _offset;
		size_t _size;

		Mesh2::PrimitiveType _primitive;
	};

	struct _submesh_description {
		
		_submesh_description* _children_submesh;
		size_t _children_submesh_count;

		_singlemesh_description* _children_singlemesh;
		size_t _children_singlemesh_count;
	};

	_submesh_description* _submesh_tree;
};