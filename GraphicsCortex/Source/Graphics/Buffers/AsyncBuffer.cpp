#include "AsyncBuffer.h"
#include "Debuger.h"
#include <iostream>

AsyncBuffer::AsyncBuffer(size_t buffer_size) : 
	_buffer_size(buffer_size) 
{
	_generate_buffer();
}

AsyncBuffer::~AsyncBuffer()
{
	release();
}

void AsyncBuffer::release()
{
	if (_buffer_generated) {
		GLCall(glDeleteBuffers(1, &id));
	}
	_buffer_generated = false;
	_buffer_allocated = false;
}

bool AsyncBuffer::wait_to_sycronize(int64_t timeout_ms)
{
	if (_gl_sync_object == nullptr)
		return true;

	GLenum wait_return = GL_UNSIGNALED;
 	GLCall(glClientWaitSync(_gl_sync_object, GL_SYNC_FLUSH_COMMANDS_BIT, timeout_ms));

	if (wait_return == GL_ALREADY_SIGNALED || wait_return == GL_CONDITION_SATISFIED) return true;
	else return false;
}

bool AsyncBuffer::wait_to_sycronize()
{
	return wait_to_sycronize(100000000);
}

bool AsyncBuffer::is_syncronized()
{
	if (_gl_sync_object == nullptr)
		return true;

	int result;
	GLCall(glGetSynciv(_gl_sync_object, GL_SYNC_STATUS, sizeof(result), NULL, &result));
	bool success = result == GL_SIGNALED;

	if (success)
		_gl_sync_object = nullptr;

	return success;
}

void AsyncBuffer::set_fence()
{
	if (_gl_sync_object != nullptr)
		glDeleteSync(_gl_sync_object);

	_gl_sync_object = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

	if (_gl_sync_object == 0) {
		std::cout << "[OpenGl Error] AsyncBuffer::set_fence() is called but fence creation failed" << std::endl;
		ASSERT(false);
	}
}

char* AsyncBuffer::get_pointer()
{
	return _buffer_data;
}

std::shared_ptr<Image> AsyncBuffer::get_image()
{
	if (image_parameters == nullptr)
	{
		std::cout << "[OpenGL Error] AsyncBuffer::get_image() is called but AsyncBuffer::image_parameters is nullptr" << std::endl;
		ASSERT(false);
	}

	size_t image_size = image_parameters->width * image_parameters->height * image_parameters->channel_count * image_parameters->bytes_per_channel;
	unsigned char* image_copy = new unsigned char[image_parameters->width * image_parameters->height * image_parameters->depth * image_parameters->channel_count * image_parameters->bytes_per_channel];
	
	wait_to_sycronize();

	std::memcpy(image_copy, _buffer_data, image_size);

	std::shared_ptr<Image> image = std::make_shared<Image>(image_copy, image_parameters->width, image_parameters->height, image_parameters->depth, image_parameters->channel_count, image_parameters->bytes_per_channel, image_parameters->vertical_flip);
	
	return image;
}

size_t AsyncBuffer::get_buffer_size()
{
	return _buffer_size;
}

void AsyncBuffer::bind_download()
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released AsyncBuffer tried to bind_download()" << std::endl;
		ASSERT(false);
	}

	_allocate_buffer(_buffer_size);

	GLCall(glBindBuffer(_target_download, id));
}

void AsyncBuffer::bind_upload()
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released AsyncBuffer tried to bind_upload()" << std::endl;
		ASSERT(false);
	}

	_allocate_buffer(_buffer_size);

	GLCall(glBindBuffer(_target_upload, id));
}

void AsyncBuffer::unbind()
{
	GLCall(glBindBuffer(_target_download, 0));
	GLCall(glBindBuffer(_target_upload, 0));
}

void AsyncBuffer::map()
{
	if (_buffer_data != nullptr) return;
	
	_allocate_buffer(_buffer_size);

	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released AsyncBuffer tried to map()" << std::endl;
		ASSERT(false);
	}
	GLCall(_buffer_data = (char*)glMapNamedBufferRange(id, 0, get_buffer_size(), GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT))
}

void AsyncBuffer::unmap()
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released AsyncBuffer tried to unmap()" << std::endl;
		ASSERT(false);
	}

	GLCall(ASSERT(glUnmapNamedBuffer(id) == GL_TRUE));
	_buffer_data = nullptr;
}

void AsyncBuffer::force_allocation() {
	_allocate_buffer(_buffer_size);
}

void AsyncBuffer::set_image_parameters(Image::ImageParameters parameters)
{
	image_parameters = std::make_unique<Image::ImageParameters>(parameters);
}

void AsyncBuffer::clear_image_parameters()
{
	image_parameters = nullptr;
}

void AsyncBuffer::_generate_buffer()
{
	if (_buffer_generated) return;
	GLCall(glCreateBuffers(1, &id));
	_buffer_generated = true;
}

void AsyncBuffer::_allocate_buffer(size_t buffer_size)
{
	if (!_buffer_generated) return;
	if (_buffer_allocated) return;

	GLCall(glNamedBufferStorage(id, buffer_size, nullptr, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT));
	_buffer_size = buffer_size;
	_buffer_allocated = true;

	map();
}
