#pragma once
#include "GL/glew.h"
#include <memory>
#include <vector>

#include "Image.h"

class Buffer {
public:
	enum TransferDirection {
		Upload,
		Download,
		Bothways,
	};

	unsigned int id;

	Buffer() = delete;
	Buffer(const Buffer& other) = delete;

	Buffer(size_t buffer_size, Buffer::TransferDirection transfer_direction = Bothways);
	~Buffer();
	void release();

	bool wait_to_sycronize_download(int64_t timeout_ms);
	bool wait_to_sycronize_download();
	bool is_syncronized_download();
	void set_fence_download();

	bool wait_to_sycronize_upload(int64_t timeout_ms);
	bool wait_to_sycronize_upload();
	bool is_syncronized_upload();
	void set_fence_upload();

	char* get_mapped_pointer();
	size_t get_buffer_size();

	void bind_to_async_download(size_t offset = 0);
	void bind_to_async_upload(size_t offset = 0);
	void bind_to_index_buffer(size_t offset = 0);
	void bind_to_vertex_buffer(size_t offset = 0);
	void bind_to_storage_buffer(size_t offset = 0);
	void bind_to_transform_feedback(size_t offset = 0);
	void unbind();

	void map(size_t offset = 0);
	void map(size_t offset, size_t size);
	void unmap();

	void set_data(int managed_buffer_offset_in_bytes, int uploading_data_offset_in_bytes, int size_in_bytes, void* const data);
	template<typename T>
	void set_data(int managed_buffer_offset_in_bytes, int uploading_data_offset_in_bytes, int size_in_bytes, const std::vector<T>& data);

	void* get_data(int managed_buffer_offset_in_bytes, int uploading_data_offset_in_bytes, int size_in_bytes);
	template<typename T>
	std::vector<T> get_data(int managed_buffer_offset_in_bytes, int uploading_data_offset_in_bytes, int size_in_bytes);

	void clear();

	std::shared_ptr<Image> get_image();
	void set_image_parameters(Image::ImageParameters parameters);
	void clear_image_parameters();

	void force_allocation();

private:
	unsigned int _target_download = GL_PIXEL_PACK_BUFFER;
	unsigned int _target_upload = GL_PIXEL_UNPACK_BUFFER;

	TransferDirection _transfer_direction;

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