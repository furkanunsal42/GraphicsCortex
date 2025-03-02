#include "Buffer.h"

#include <iostream>

#include "Debuger.h"

Buffer::Buffer(size_t buffer_size, MapInfo map_upon_initialization, MemoryType buffer_memory_type) :
	_buffer_size(buffer_size), _map_after_initialization(true), _buffer_memory_type(buffer_memory_type)
{
	_map_description = map_upon_initialization;
	_generate_buffer();
}

Buffer::Buffer(size_t buffer_size, MemoryType buffer_memory_type) :
	_buffer_size(buffer_size), _map_after_initialization(false), _buffer_memory_type(buffer_memory_type)
{
	_generate_buffer();
}

Buffer::~Buffer()
{
	release();
}

void Buffer::release()
{
	bool was_mapped = is_mapped();
	
	if (_buffer_generated) {
		GLCall(glDeleteBuffers(1, &id));
	}

	if (was_mapped) {
		_buffer_data = nullptr;
		_mapped_pointer_size = 0;
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
		std::cout << "[OpenGl Error] Buffer::set_fence_upload() is called but fence creation failed" << std::endl;
		ASSERT(false);
	}
}

void Buffer::copy_to(size_t managed_buffer_offset_in_bytes, size_t target_buffer_offset_in_bytes, size_t copy_size_in_bytes, Buffer& target_buffer)
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released Buffer tried to be source for copy_to()" << std::endl;
		ASSERT(false);
	}

	if (!target_buffer._buffer_generated) {
		std::cout << "[OpenGL Error] released Buffer tried to be target for copy_to()" << std::endl;
		ASSERT(false);
	}

	_allocate_buffer(_buffer_size);
	target_buffer._allocate_buffer(target_buffer._buffer_size);

	size_t operation_size = std::min<size_t>(target_buffer._buffer_size - target_buffer_offset_in_bytes, (std::min<size_t>(_buffer_size - managed_buffer_offset_in_bytes, copy_size_in_bytes)));
	GLCall(glCopyNamedBufferSubData(id, target_buffer.id, managed_buffer_offset_in_bytes, target_buffer_offset_in_bytes, operation_size));
}

void Buffer::copy_to(Buffer& target_buffer)
{
	copy_to(0, 0, std::min<size_t>(_buffer_size, target_buffer._buffer_size), target_buffer);
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

void Buffer::bind_as_vertex_buffer()
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released Buffer tried to bind_as_vertex_buffer()" << std::endl;
		ASSERT(false);
	}

	_allocate_buffer(_buffer_size);

	GLCall(glBindBuffer(_target_vertex_buffer, id));
}

void Buffer::bind_as_index_buffer()
{
	GLCall(glBindBuffer(_target_index_buffer, id));
}

void Buffer::unbind()
{
	GLCall(glBindBuffer(_target_download, 0));
	GLCall(glBindBuffer(_target_upload, 0));

	
	// others
}

void Buffer::map(MapInfo map_description)
{
	map(0, get_buffer_size_in_bytes(), map_description);
}

void Buffer::map(size_t offset_in_bytes, size_t size_in_bytes, MapInfo map_description)
{
	if (_buffer_data != nullptr) return;

	_allocate_buffer(_buffer_size);

	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released Buffer tried to map()" << std::endl;
		ASSERT(false);
	}

	unsigned int flag = GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT;
	if (_map_description.direction == MapInfo::Bothways || _map_description.direction == MapInfo::Upload) flag |= GL_MAP_WRITE_BIT;
	if (_map_description.direction == MapInfo::Bothways || _map_description.direction == MapInfo::Download) flag |= GL_MAP_READ_BIT;

	size_t map_size = std::min(_buffer_size - offset_in_bytes, size_in_bytes);
	GLCall(_buffer_data = (char*)glMapNamedBufferRange(id, offset_in_bytes, map_size, flag));
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

	GLCall(glUnmapNamedBuffer(id));
	_buffer_data = nullptr;
	_mapped_pointer_size = 0;
}

bool Buffer::is_mapped()
{
	return _mapped_pointer_size != 0;
}

size_t Buffer::get_buffer_size_in_bytes()
{
	return _buffer_size;
}

size_t Buffer::get_mapped_buffer_size()
{
	return _mapped_pointer_size;
}

bool Buffer::is_mapping_persistant()
{
	return is_mapped() && _map_description.lifetime == MapInfo::Persistant;
}

Buffer::MemoryType Buffer::get_memory_type()
{
	return _buffer_memory_type;
}

void Buffer::load_data(size_t managed_buffer_offset_in_bytes, size_t uploading_data_offset_in_bytes, size_t size_in_bytes, void* const data)
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released Buffer tried to load_data()" << std::endl;
		ASSERT(false);
	}
	
	_allocate_buffer(_buffer_size);
	
	GLCall(glNamedBufferSubData(id, managed_buffer_offset_in_bytes, std::min<size_t>(_buffer_size, size_in_bytes), (char*)data + uploading_data_offset_in_bytes));
}

void Buffer::load_data(size_t size_in_bytes, void* const data)
{
	load_data(0, 0, size_in_bytes, data);
}

void Buffer::get_data(size_t managed_buffer_offset_in_bytes, size_t downloading_data_offset_in_bytes, size_t size_in_bytes, void* data_out)
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released Buffer tried to get_data()" << std::endl;
		ASSERT(false);
	}

	_allocate_buffer(_buffer_size);

	GLCall(glGetNamedBufferSubData(id, managed_buffer_offset_in_bytes, size_in_bytes, (char*)data_out + downloading_data_offset_in_bytes));
}

void Buffer::get_data(size_t managed_buffer_offset_in_bytes, size_t downloading_data_offset_in_bytes, void* data_out)
{
	get_data(managed_buffer_offset_in_bytes, downloading_data_offset_in_bytes, _buffer_size, data_out);
}

void* Buffer::get_data(size_t managed_buffer_offset_in_bytes, size_t size_in_bytes)
{
	char* buffer = new char[size_in_bytes];

	get_data(managed_buffer_offset_in_bytes, 0, size_in_bytes, buffer);

	return buffer;
}

void* Buffer::get_data(size_t managed_buffer_offset_in_bytes)
{
	return get_data(managed_buffer_offset_in_bytes, _buffer_size);
}

void* Buffer::get_data()
{
	return get_data(0);
}

void Buffer::clear()
{
	clear<int8_t>(0);
}

char* Buffer::get_mapped_pointer()
{
	return _buffer_data;
}

void Buffer::bind_to_async_download()
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released Buffer tried to bind_download()" << std::endl;
		ASSERT(false);
	}

	_allocate_buffer(_buffer_size);

	GLCall(glBindBuffer(_target_download, id));
}

void Buffer::bind_to_async_upload()
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released Buffer tried to bind_upload()" << std::endl;
		ASSERT(false);
	}

	_allocate_buffer(_buffer_size);

	GLCall(glBindBuffer(_target_upload, id));
}

void Buffer::bind_as_storage_buffer(unsigned int buffer_slot, size_t offset, size_t size)
{
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released Buffer tried to bind_as_storage_buffer()" << std::endl;
		ASSERT(false);
	}

	if (!_buffer_allocated) {
		std::cout << "[OpenGL Warning] Buffer tried to bind_as_storage_buffer() but no user data was loaded yet" << std::endl;
		_allocate_buffer(_buffer_size);
	}
	
	GLCall(glBindBufferRange(_target_storage_buffer, buffer_slot, id, offset, size));
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

	unsigned int flag = GL_DYNAMIC_STORAGE_BIT;
	flag |= GL_MAP_WRITE_BIT;
	flag |= GL_MAP_READ_BIT;
	
	if (_buffer_memory_type == MemoryType::CPU_BUFFER) flag |= GL_CLIENT_STORAGE_BIT;
	if (_map_description.lifetime == MapInfo::Persistant) flag |= GL_MAP_PERSISTENT_BIT;
	
	GLCall(glNamedBufferStorage(id, buffer_size, nullptr, flag));
	_buffer_size = buffer_size;
	_buffer_allocated = true;

	if (_map_after_initialization)
		map();
}

size_t Buffer::query_can_map_write()
{
	return size_t();
}

bool Buffer::query_is_map_read()
{
	return false;
}

bool Buffer::query_is_map_write()
{
	return false;
}

bool Buffer::query_is_map_persistant()
{
	return false;
}

bool Buffer::query_is_map_coherent()
{
	return false;
}

bool Buffer::query_is_mapped()
{
	return false;
}

size_t Buffer::query_map_size()
{
	return size_t();
}

size_t Buffer::query_map_offset()
{
	return size_t();
}

bool Buffer::query_is_immutable()
{
	return false;
}

bool Buffer::query_is_dynamic_storage()
{
	return false;
}

size_t Buffer::query_buffer_size()
{
	return size_t();
}

size_t Buffer::query_can_map_read()
{
	return size_t();
}

Buffer::MapInfo::MapInfo(Direction direction, Lifetime lifetime) :
	direction(direction), lifetime(lifetime) {}
