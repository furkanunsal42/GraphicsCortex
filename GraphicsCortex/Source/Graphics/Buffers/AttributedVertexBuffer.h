#pragma once

#include "gl/glew.h"
#include <memory>
#include <unordered_map>

#include "StandardBuffer.h"

class AttributedVertexBuffer {
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
		ui_10f_11f_11f
	};

	AttributedVertexBuffer();
	AttributedVertexBuffer(const AttributedVertexBuffer& other) = delete;
	~AttributedVertexBuffer();

	void release();

	void bind();
	void unbind();

	void attach_vertex_buffer(int slot, std::shared_ptr<Buffer> vertex_buffer, AttributeType attribute_type, int8_t element_per_vertex, size_t stride);
	void deattach_vertex_buffer(int slot);
	std::shared_ptr<Buffer> get_vertex_buffer(int slot);

	void attach_index_buffer(std::shared_ptr<Buffer> index_buffer);
	void deattach_index_buffer();
	std::shared_ptr<Buffer> get_index_buffer();

	void enable_attribute(int slot);
	void disable_attribute(int slot);
	bool is_attribute_enabled(int slot);
	void enabled_all_attached_attributes();
	void disable_all_attributes();

	int get_max_attribute_count();
private:

	struct _buffer_with_slotinfo {
	public:
		_buffer_with_slotinfo() = default;
		_buffer_with_slotinfo(std::shared_ptr<Buffer> buffer, AttributeType attribute_type, int8_t element_per_vertex, size_t stride, bool is_enabled);
		std::shared_ptr<Buffer> _buffer = nullptr;
		AttributeType _attribute_type = AttributeType::i8;
		int8_t _element_per_vertex = 0;
		size_t _stride = 0;
		
		bool _enabled = false;
	};

	unsigned int id = 0;
	const int _max_attribute_count;

	std::shared_ptr<Buffer> _index_buffer = nullptr;
	std::unordered_map<int, _buffer_with_slotinfo> _vertex_buffers;

	void _generate_buffer();

	bool _buffer_generated = false;

};