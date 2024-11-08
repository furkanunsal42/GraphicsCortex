#include "StandardBuffer.h"

#include <iostream>

#include "Debuger.h"

Buffer::Buffer(size_t buffer_size, Buffer::TransferDirection transfer_direction, bool map_after_initialization) :
	_buffer_size(buffer_size), _transfer_direction(transfer_direction), _map_after_initialization(map_after_initialization)
{
	_generate_buffer();
}

Buffer::~Buffer()
{
	release();
}

void Buffer::release()
{
	unmap();

	if (_buffer_generated) {
		GLCall(glDeleteBuffers(1, &id));
	}

	_buffer_generated = false;
	_buffer_allocated = false;
}

bool Buffer::wait_to_sycronize_download(int64_t timeout_ms)
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

bool Buffer::wait_to_sycronize_download()
{
	return wait_to_sycronize_download(1000000000);
}

bool Buffer::is_syncronized_download()
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

void Buffer::set_fence_download()
{
	if (_gl_sync_object_download != nullptr)
		glDeleteSync(_gl_sync_object_download);

	_gl_sync_object_download = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

	if (_gl_sync_object_download == 0) {
		std::cout << "[OpenGl Error] Buffer::set_fence_download() is called but fence creation failed" << std::endl;
		ASSERT(false);
	}
}

bool Buffer::wait_to_sycronize_upload(int64_t timeout_ms)
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

bool Buffer::wait_to_sycronize_upload()
{
	return wait_to_sycronize_upload(1000000000);
}

bool Buffer::is_syncronized_upload()
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

void Buffer::set_fence_upload()
{
	if (_gl_sync_object_upload != nullptr)
		glDeleteSync(_gl_sync_object_upload);

	_gl_sync_object_upload = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

	if (_gl_sync_object_upload == 0) {
		std::cout << "[OpenGl Error] AsyncBuffer::set_fence_upload() is called but fence creation failed" << std::endl;
		ASSERT(false);
	}
}

std::shared_ptr<Image> Buffer::get_image()
{
	if (image_parameters == nullptr)
	{
		std::cout << "[OpenGL Error] Buffer::get_image() is called but Buffer::image_parameters is nullptr" << std::endl;
		ASSERT(false);
	}

	size_t image_size = image_parameters->width * image_parameters->height * image_parameters->channel_count * image_parameters->bytes_per_channel;
	unsigned char* image_copy = new unsigned char[image_parameters->width * image_parameters->height * image_parameters->depth * image_parameters->channel_count * image_parameters->bytes_per_channel];

	wait_to_sycronize_download();

	std::memcpy(image_copy, _buffer_data, image_size);

	std::shared_ptr<Image> image = std::make_shared<Image>(image_copy, image_parameters->width, image_parameters->height, image_parameters->depth, image_parameters->channel_count, image_parameters->bytes_per_channel, image_parameters->vertical_flip);

	return image;
}

void Buffer::set_image_parameters(Image::ImageParameters parameters)
{
	image_parameters = std::make_unique<Image::ImageParameters>(parameters);
}

void Buffer::clear_image_parameters()
{
	image_parameters = nullptr;
}

void Buffer::force_allocation()
{
	_allocate_buffer(_buffer_size);
}

void Buffer::map()
{
	map(0, get_buffer_size_in_bytes());
}

void Buffer::map(size_t offset, size_t size_in_bytes)
{
	if (_buffer_data != nullptr) return;

	_allocate_buffer(_buffer_size);

	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released Buffer tried to map()" << std::endl;
		ASSERT(false);
	}

	unsigned int flag = GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT;
	if (_transfer_direction == Bothways || _transfer_direction == Upload) flag |= GL_MAP_WRITE_BIT;
	if (_transfer_direction == Bothways || _transfer_direction == Download) flag |= GL_MAP_READ_BIT;

	size_t map_size = std::min(_buffer_size - offset, size_in_bytes);
	GLCall(_buffer_data = (char*)glMapNamedBufferRange(id, offset, map_size, flag));
	_mapped_pointer_size = map_size;
}

void Buffer::unmap()
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released Buffer tried to unmap()" << std::endl;
		ASSERT(false);
	}

	if (_buffer_data == nullptr)
		return;

	GLCall(ASSERT(glUnmapNamedBuffer(id) == GL_TRUE));
	_buffer_data = nullptr;
	_mapped_pointer_size = 0;
}

void Buffer::set_data(int managed_buffer_offset_in_bytes, int uploading_data_offset_in_bytes, int size_in_bytes, void* const data)
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released Buffer tried to set_data()" << std::endl;
		ASSERT(false);
	}
	
	_allocate_buffer(_buffer_size);
	
	std::cout << managed_buffer_offset_in_bytes << " " << std::min<int>(_buffer_size, size_in_bytes) << std::endl;
	GLCall(glNamedBufferSubData(id, managed_buffer_offset_in_bytes, std::min<int>(_buffer_size, size_in_bytes), (char*)data + uploading_data_offset_in_bytes));
}

char* Buffer::get_mapped_pointer()
{
	return _buffer_data;
}

size_t Buffer::get_buffer_size_in_bytes()
{
	return _buffer_size;
}

void Buffer::_generate_buffer()
{
	if (_buffer_generated) return;
	GLCall(glCreateBuffers(1, &id));
	_buffer_generated = true;
}

void Buffer::_allocate_buffer(size_t buffer_size)
{
	if (!_buffer_generated) return;
	if (_buffer_allocated) return;

	unsigned int flag = GL_MAP_PERSISTENT_BIT | GL_DYNAMIC_STORAGE_BIT;
	if (_transfer_direction == Bothways || _transfer_direction == Upload) flag |= GL_MAP_WRITE_BIT;
	if (_transfer_direction == Bothways || _transfer_direction == Download) flag |= GL_MAP_READ_BIT;

	GLCall(glNamedBufferStorage(id, buffer_size, nullptr, flag));
	_buffer_size = buffer_size;
	_buffer_allocated = true;

	if (_map_after_initialization)
		map();
}


