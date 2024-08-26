#pragma once
#include "GL/glew.h"

class AsyncReadBuffer {
public:
	unsigned int id;

	AsyncReadBuffer(const AsyncReadBuffer& other) = delete;
	
	AsyncReadBuffer();
	~AsyncReadBuffer();
	void release();

	bool is_read_finished();
	char* get_buffer();
	size_t get_buffer_size();
	
	void bind();
	void unbind();
	void map(size_t buffer_size_in_bytes);
	void unmap();

private:
	unsigned int target = GL_PIXEL_PACK_BUFFER;
	char* _buffer_data = nullptr;
	size_t _buffer_size = 0;
	GLsync _gl_sync_object = nullptr;

	void _generate_buffer();
	void _allocate_buffer(size_t buffer_size);

	bool _buffer_generated = false;
	bool _buffer_allocated = false;
};