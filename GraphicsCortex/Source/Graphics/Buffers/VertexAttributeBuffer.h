#pragma once

#include <memory>
#include "Buffer.h"
#include "IndexBufferEnums.h"

class SingleModel;

typedef uint32_t attribute_slot_t;
typedef uint32_t buffer_slot_t;

class VertexAttributeBuffer {
public:

	static const buffer_slot_t null_buffer_slot = -1;
	static const attribute_slot_t null_attribute_slot = -1;

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

	uint32_t get_attribute_type_to_GL_type(VertexAttributeBuffer::AttributeType type);
	uint32_t get_attribute_type_to_GL_normalized_flag(VertexAttributeBuffer::AttributeType type);

	struct BufferSlotInfo {
	public:
		BufferSlotInfo() = default;
		BufferSlotInfo(std::shared_ptr<Buffer> buffer, size_t stride_in_bytes, size_t offset_in_bytes, size_t divisor);

		std::shared_ptr<Buffer> buffer = nullptr;
		size_t stride_in_bytes = 0;
		size_t offset_in_bytes = 0;
		size_t divisor = 0;
	};

	struct AttributeSlotInfo {
		AttributeSlotInfo() = default;
		AttributeSlotInfo(buffer_slot_t buffer_slot, AttributeType type, int32_t element_count_per_vertex, size_t relative_offset_in_bytes, bool slot_enabled);

		buffer_slot_t buffer_slot = null_buffer_slot;
		AttributeType type = AttributeType::a_f32;
		int32_t element_count_per_vertex = 0;
		size_t relative_offset_in_bytes = 0;
		bool slot_enabled = false;
	};

	VertexAttributeBuffer();
	VertexAttributeBuffer(const VertexAttributeBuffer& other) = delete;
	~VertexAttributeBuffer();
	void release();
	void clear();

	void load_model(const SingleModel& model);

	void bind();
	void unbind();

	//void attach_vertex_buffer(int32_t attribute_slot, int32_t buffer_slot, std::shared_ptr<Buffer> vertex_buffer, size_t stride, size_t buffer_offset, size_t divisor, AttributeType attribute_type, int32_t element_per_vertex, size_t relative_offset, bool enabled = true);
	
	void attach_vertex_buffer(
		buffer_slot_t buffer_slot, 
		std::shared_ptr<Buffer> vertex_buffer, 
		size_t stride_in_bytes, 
		size_t buffer_offset_in_bytes = 0, 
		uint32_t divisor_for_instancing = 0
	);
	
	void detach_vertex_buffer(buffer_slot_t buffer_slot);

	void set_attribute_format(
		attribute_slot_t attribute_slot, 
		buffer_slot_t buffer_slot, 
		AttributeType attribute_type, 
		uint32_t element_count_per_vertex, 
		size_t relative_offset_in_bytes, 
		bool enabled = true
	);
	
	BufferSlotInfo& get_buffer_slot(buffer_slot_t buffer_slot);
	AttributeSlotInfo& get_attribute_slot(attribute_slot_t attribute_slot);
	
	void enable_attribute(attribute_slot_t attribute_slot);
	void disable_attribute(attribute_slot_t attribute_slot);
	bool is_attribute_enabled(attribute_slot_t attribute_slot);
	void enabled_all_attached_attributes();
	void disable_all_attributes();
	
	uint32_t get_vertex_buffer_count();
	uint32_t get_active_attribute_count();

	size_t get_vertex_count_of_buffer(buffer_slot_t buffer_slot);
	size_t get_element_count_of_attribute(attribute_slot_t attribute_slot);
	size_t get_vertex_count_of_attribute(attribute_slot_t attribute_slot);

	// by vertex count
	buffer_slot_t get_largest_buffer_slot();
	buffer_slot_t get_smallest_buffer_slot();

	// by vertex count
	attribute_slot_t get_largest_active_attribute_slot();
	attribute_slot_t get_smallest_active_attribute_slot();

	size_t get_max_vertex_count();
	size_t get_min_vertex_count();

	static int32_t get_max_buffer_count();
	static int32_t get_max_attribute_count();

private:

	uint32_t id = 0;

	bool check_for_validity();

	void generate_buffer();
	bool buffer_generated = false;

	bool is_validity_checked = false;

	std::vector<BufferSlotInfo> vertex_buffers;
	std::vector<AttributeSlotInfo> attributes;
};