#pragma once

#include <memory>
#include "StandardBuffer.h"
#include "IndexBufferEnums.h"

class SingleModel2;

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
		AttributeType _attribute_type = AttributeType::a_f32;
		int32_t _element_per_vertex = 0;
		size_t _stride = 0;
		size_t _offset = 0;

		bool _slot_enabled = false;
	};

	std::vector<_buffer_with_structure_info> _vertex_buffers;

};