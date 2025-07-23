#pragma once
#include "GL/glew.h"
#include <memory>

#include "Image.h"

class AsyncBuffer {
public:
	enum Direction {
		Upload,
		Download,
		Bothways,
	};

	unsigned int id;

	AsyncBuffer() = delete;;
	AsyncBuffer(const AsyncBuffer& other) = delete;
	
	AsyncBuffer(size_t buffer_size, AsyncBuffer::Direction transfer_direction = Bothways);
	~AsyncBuffer();
	void release();

	bool wait_to_sycronize_download(int64_t timeout_ms);
	bool wait_to_sycronize_download();
	bool is_syncronized_download();
	void set_fence_download();

	bool wait_to_sycronize_upload(int64_t timeout_ms);
	bool wait_to_sycronize_upload();
	bool is_syncronized_upload();
	void set_fence_upload();

	char* get_pointer();
	std::shared_ptr<Image> get_image();
	size_t get_buffer_size();
	
	void get_data(size_t managed_buffer_offset_in_bytes, size_t downloading_data_offset_in_bytes, size_t size_in_bytes, void* data_out);
	void get_data(size_t managed_buffer_offset_in_bytes, size_t downloading_data_offset_in_bytes, void* data_out);
	void* get_data(size_t managed_buffer_offset_in_bytes, size_t size_in_bytes);
	void* get_data(size_t managed_buffer_offset_in_bytes);
	void* get_data();
	
	void bind_download();
	void bind_upload();
	void unbind();
	
	void map();
	void unmap();

	void force_allocation();

	void set_image_parameters(Image::ImageParameters parameters);
	void clear_image_parameters();

private:
	unsigned int _target_download = GL_PIXEL_PACK_BUFFER;
	unsigned int _target_upload = GL_PIXEL_UNPACK_BUFFER;

	Direction _transfer_direction;

	char* _buffer_data = nullptr;
	size_t _buffer_size = 0;

	GLsync _gl_sync_object_download = nullptr;
	GLsync _gl_sync_object_upload = nullptr;

	void _generate_buffer();
	void _allocate_buffer(size_t buffer_size);

	bool _buffer_generated = false;
	bool _buffer_allocated = false;

	std::unique_ptr<Image::ImageParameters> image_parameters = nullptr;
};