#pragma once
#include "GL/glew.h"
#include <memory>

#include "Image.h"

class AsyncBuffer {
public:
	unsigned int id;

	AsyncBuffer() = delete;;
	AsyncBuffer(const AsyncBuffer& other) = delete;
	
	AsyncBuffer(size_t buffer_size);
	~AsyncBuffer();
	void release();

	bool wait_to_sycronize(int64_t timeout_ms);
	bool wait_to_sycronize();
	bool is_syncronized();
	void set_fence();

	char* get_pointer();
	std::shared_ptr<Image> get_image();
	size_t get_buffer_size();
	
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

	char* _buffer_data = nullptr;
	size_t _buffer_size = 0;
	GLsync _gl_sync_object = nullptr;

	void _generate_buffer();
	void _allocate_buffer(size_t buffer_size);

	bool _buffer_generated = false;
	bool _buffer_allocated = false;

	std::unique_ptr<Image::ImageParameters> image_parameters = nullptr;
};