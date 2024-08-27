#include "AsyncReadBuffer.h"
#include "Debuger.h"
#include <iostream>

AsyncReadBuffer::AsyncReadBuffer()
{
	_generate_buffer();
}

AsyncReadBuffer::AsyncReadBuffer(size_t buffer_size) : 
	_buffer_size(buffer_size) { }

AsyncReadBuffer::~AsyncReadBuffer()
{
	release();
}

void AsyncReadBuffer::release()
{
	if (_buffer_generated) {
		GLCall(glDeleteBuffers(1, &id));
	}
	_buffer_generated = false;
	_buffer_allocated = false;
}

bool AsyncReadBuffer::is_read_finished()
{
	if (_gl_sync_object == nullptr)
		return true;

	int result;
	GLCall(glGetSynciv(_gl_sync_object, GL_SYNC_STATUS, sizeof(result), NULL, &result));
	return result == GL_SIGNALED;
}

char* AsyncReadBuffer::get_buffer()
{
	return _buffer_data;
}

size_t AsyncReadBuffer::get_buffer_size()
{
	return _buffer_size;
}

void AsyncReadBuffer::bind()
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released AsyncReadBuffer tried to bind()" << std::endl;
		ASSERT(false);
	}
	GLCall(glBindBuffer(target, id));
}

void AsyncReadBuffer::unbind()
{
	GLCall(glBindBuffer(target, 0));
}

void AsyncReadBuffer::map()
{
	_allocate_buffer(_buffer_size);

	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released AsyncReadBuffer tried to map()" << std::endl;
		ASSERT(false);
	}
	GLCall(_buffer_data = (char*)glMapNamedBuffer(id, GL_READ_ONLY))
}

void AsyncReadBuffer::unmap()
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released AsyncReadBuffer tried to unmap()" << std::endl;
		ASSERT(false);
	}

	GLCall(ASSERT(glUnmapNamedBuffer(id) == GL_TRUE));
	_buffer_data = nullptr;
}

void AsyncReadBuffer::_generate_buffer()
{
	if (_buffer_generated) return;
	GLCall(glCreateBuffers(1, &id));
	_buffer_generated = true;
}

void AsyncReadBuffer::_allocate_buffer(size_t buffer_size)
{
	if (!_buffer_generated) return;
	if (_buffer_allocated) return;

	GLCall(glBufferStorage(id, buffer_size, nullptr, GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT));
	_buffer_size = buffer_size;
	_buffer_allocated = true;
}
