#pragma once
#include <vector>

class VertexBuffer {
public:
	unsigned int id;

	VertexBuffer(const VertexBuffer& other) = delete;
	
	VertexBuffer();
	~VertexBuffer();
	void release();

	void bind();
	void unbind();

	void push_attribute(unsigned int count);

	void load_data(const std::vector<float>& verticies);
	void load_data(int managed_buffer_offset_in_bytes, int uploading_buffer_offset_in_bytes, int size_in_bytes, const std::vector<float>& verticies);
	void load_data(int managed_buffer_offset_in_bytes, int uploading_buffer_offset_in_bytes, int size_in_bytes, float* data);

	void clear();
	void clear(int offset_in_bytes, int size_in_bytes);

	void copy_to_buffer(VertexBuffer& target_buffer);
	void copy_to_buffer(VertexBuffer& target_buffer, int managed_buffer_offset_in_bytes, int target_buffer_offset_in_bytes, int size_in_bytes);


private:

	int _vertex_amount;

	std::vector<unsigned int> _attribute_table;

	void _generate_buffer();
	void _allocate_buffer();

	bool _buffer_generated = false;
	bool _buffer_allocated = false;
	bool _user_data_loaded = false;
};