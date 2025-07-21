#include "AsyncBuffer.h"
#include "Debuger.h"
#include <iostream>

AsyncBuffer::AsyncBuffer(size_t buffer_size, AsyncBuffer::Direction transfer_direction) : 
	_buffer_size(buffer_size), _transfer_direction(transfer_direction)
{
	_generate_buffer();
}

AsyncBuffer::~AsyncBuffer()
{
	release();
}

void AsyncBuffer::release()
{
	unmap();
	
	if (_buffer_generated) {
		GLCall(glDeleteBuffers(1, &id));
	}
	
	_buffer_generated = false;
	_buffer_allocated = false;
}

bool AsyncBuffer::wait_to_sycronize_download(int64_t timeout_ms)
{
	if (_gl_sync_object_download == nullptr)
		return true;

	GLenum wait_return = GL_UNSIGNALED;
	GLCall(wait_return = glClientWaitSync(_gl_sync_object_download, GL_SYNC_FLUSH_COMMANDS_BIT, timeout_ms));

	if (wait_return == GL_ALREADY_SIGNALED || wait_return == GL_CONDITION_SATISFIED) {
		_gl_sync_object_download = nullptr;
		return true;
	}
	else if (wait_return == GL_WAIT_FAILED) {
		ASSERT(false);
	}
	else return false;
}

bool AsyncBuffer::wait_to_sycronize_download()
{
	return wait_to_sycronize_download(1000000000);
}

bool AsyncBuffer::is_syncronized_download()
{
	if (_gl_sync_object_download == nullptr)
		return true;

	int result;
	GLCall(glGetSynciv(_gl_sync_object_download, GL_SYNC_STATUS, sizeof(result), NULL, &result));
	bool success = result == GL_SIGNALED;

	if (success)
		_gl_sync_object_download = nullptr;

	return success;
}

void AsyncBuffer::set_fence_download()
{
	if (_gl_sync_object_download != nullptr)
		glDeleteSync(_gl_sync_object_download);

	_gl_sync_object_download = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

	if (_gl_sync_object_download == 0) {
		std::cout << "[OpenGl Error] AsyncBuffer::set_fence_download() is called but fence creation failed" << std::endl;
		ASSERT(false);
	}
}

bool AsyncBuffer::wait_to_sycronize_upload(int64_t timeout_ms)
{
	if (_gl_sync_object_upload == nullptr)
		return true;

	GLenum wait_return = GL_UNSIGNALED;
	GLCall(wait_return = glClientWaitSync(_gl_sync_object_upload, GL_SYNC_FLUSH_COMMANDS_BIT, timeout_ms));

	if (wait_return == GL_ALREADY_SIGNALED || wait_return == GL_CONDITION_SATISFIED) {
		_gl_sync_object_upload = nullptr;
		return true;
	}
	else if (wait_return == GL_WAIT_FAILED) {
		ASSERT(false);
	}
	else return false;
}

bool AsyncBuffer::wait_to_sycronize_upload()
{
	return wait_to_sycronize_upload(1000000000);
}

bool AsyncBuffer::is_syncronized_upload()
{
	if (_gl_sync_object_upload == nullptr)
		return true;

	int result;
	GLCall(glGetSynciv(_gl_sync_object_upload, GL_SYNC_STATUS, sizeof(result), NULL, &result));
	bool success = result == GL_SIGNALED;

	if (success)
		_gl_sync_object_upload = nullptr;

	return success;
}

void AsyncBuffer::set_fence_upload()
{
	if (_gl_sync_object_upload != nullptr)
		glDeleteSync(_gl_sync_object_upload);

	_gl_sync_object_upload = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

	if (_gl_sync_object_upload == 0) {
		std::cout << "[OpenGl Error] AsyncBuffer::set_fence_upload() is called but fence creation failed" << std::endl;
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
	
	wait_to_sycronize_download();

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

	unsigned int flag = GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT;
	if (_transfer_direction == Bothways || _transfer_direction == Upload) flag |= GL_MAP_WRITE_BIT;
	if (_transfer_direction == Bothways || _transfer_direction == Download) flag |= GL_MAP_READ_BIT;

	GLCall(_buffer_data = (char*)glMapNamedBufferRange(id, 0, get_buffer_size(), flag))
}

void AsyncBuffer::unmap()
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released AsyncBuffer tried to unmap()" << std::endl;
		ASSERT(false);
	}

	GLCall(bool success = glUnmapNamedBuffer(id) == GL_TRUE);
	if (!success) {
		std::cout << "[OpenGL Error] AsyncBuffer unmap() content corrupted" << std::endl;
	}

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
	
	unsigned int flag = GL_MAP_PERSISTENT_BIT;
	if (_transfer_direction == Bothways || _transfer_direction == Upload) flag |= GL_MAP_WRITE_BIT;
	if (_transfer_direction == Bothways || _transfer_direction == Download) flag |= GL_MAP_READ_BIT;

	GLCall(glNamedBufferStorage(id, buffer_size, nullptr, flag));
	_buffer_size = buffer_size;
	_buffer_allocated = true;

	map();
}
