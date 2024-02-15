#include "UniformBuffer.h"

UniformBuffer::UniformBuffer()
{
	GLCall(glGenBuffers(1, &id));
	_buffer_generated = true;
}

UniformBuffer::~UniformBuffer()
{
	release();
}

void UniformBuffer::release()
{
	if (!_buffer_generated)
		return;

	GLCall(glDeleteBuffers(1, &id));
	_buffer_generated = false;
}

void UniformBuffer::bind(int bind_target)
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] UniformBuffer tried to bind() but resource was released" << std::endl;
		ASSERT(false);
	}
	upload_data();
	GLCall(glBindBufferBase(_target, bind_target, id));
	bound_slot = bind_target;
	ever_bound = true;
}

void UniformBuffer::bind(int bind_target, int offset, int size)
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] UniformBuffer tried to bind() but resource was released" << std::endl;
		ASSERT(false);
	}
	if (offset + size > _buffer_size) {
		std::cout << "[OpenGL Error] UniformBuffer tried to bind() but offset and size was out of bound" << std::endl;
		ASSERT(false)
	}
	if (offset < 0 || size < 0) {
		std::cout << "[OpenGL Error] UniformBuffer tried to bind() but offset and size was out of bound" << std::endl;
		ASSERT(false)
	}
	
	upload_data();
	GLCall(glBindBufferRange(_target, bind_target, id, offset, offset + size));
	bound_slot = bind_target;
	ever_bound = true;
}

void UniformBuffer::bind()
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] UniformBuffer tried to bind() but resource was released" << std::endl;
		ASSERT(false);
	}
	GLCall(glBindBuffer(_target, id));
}

void UniformBuffer::unbind()
{
	GLCall(glBindBuffer(_target, 0));
}

void UniformBuffer::push_variable_array(int array_size)
{
	const int N = 4;
	_push_varible_size(array_size * 4 * N);
}

void UniformBuffer::set_data(int managed_buffer_offset_in_bytes, int uploading_data_offset_in_bytes, int size_in_bytes, void* data)
{
	if (size_in_bytes == 0) return;
	
	if (managed_buffer_offset_in_bytes + size_in_bytes > _buffer_size) {
		std::cout << "[OpenGL Error] UniformBuffer tried to set_data() to an out of bound location" << std::endl;
		ASSERT(false);
	}
	if (managed_buffer_offset_in_bytes < 0 || uploading_data_offset_in_bytes < 0 || size_in_bytes < 0) {
		std::cout << "[OpenGL Error] UniformBuffer tried to set_data() but offset and size was out of bound" << std::endl;
		ASSERT(false)
	}
	
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] UniformBuffer tried to set_data() but resource was released" << std::endl;
		ASSERT(false);
	}
	_allocate_buffer();

	_updated_ranges.push_back(_range(managed_buffer_offset_in_bytes, managed_buffer_offset_in_bytes + size_in_bytes));
	
	memcpy(managed_buffer_offset_in_bytes + (char*)cpu_data, (void*)((char*)data + uploading_data_offset_in_bytes), size_in_bytes);
}

void UniformBuffer::set_data(int varible_index, void* const data)
{
	set_data(_pushed_variable_offsets[varible_index], 0, _pushed_variable_sizes[varible_index], data);
}

void UniformBuffer::clear()
{
	for (int i = 0; i < _buffer_size; i++)
		((char*)cpu_data)[i] = 0;
	_updated_ranges.push_back(_range(0, _buffer_size));
}

void UniformBuffer::upload_data()
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] UniformBuffer tried to upload_data() but resource was released" << std::endl;
		ASSERT(false);
	}
	_allocate_buffer();

	_range range = _compute_bounding_range();
	if (range.begin == range.end) return;

	bind();
	GLCall(glBufferSubData(_target, range.begin, range.end - range.begin, (char*)cpu_data + range.begin));
	_updated_ranges.clear();

	_user_data_uploaded = true;
}

void UniformBuffer::_push_varible_size(int size) {
	if (_buffer_allocated) {
		std::cout << "[OpenGL Error] Uniform Buffer tried to _push_varible_size() but resource was already allocated" << std::endl;
		ASSERT(false);
	}

	_pushed_variable_offsets.push_back(_buffer_size);
	_pushed_variable_sizes.push_back(size);
	_buffer_size += size;

}

UniformBuffer::_range UniformBuffer::_compute_bounding_range() {
	if (_updated_ranges.size() == 0) return _range(0, 0);
	
	_range result = _updated_ranges[0];
	for (int i = 1; i < _updated_ranges.size(); i++) {
		result.begin = std::min(result.begin, _updated_ranges[i].begin);
		result.end	 = std::max(result.end, _updated_ranges[i].end);
	}

	return result;
}


void UniformBuffer::_allocate_buffer() {
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] UniformBuffer tried to _allocate_buffer() but resource was released" << std::endl;
		ASSERT(false);
	}
	if (_buffer_allocated) return;
	
	bind();
	//GLCall(glBufferStorage(_target, _buffer_size, NULL, GL_DYNAMIC_STORAGE_BIT));
	glBufferData(_target, _buffer_size, NULL, GL_STATIC_DRAW);
	cpu_data = new char[_buffer_size];
	
	_buffer_allocated = true;
}

