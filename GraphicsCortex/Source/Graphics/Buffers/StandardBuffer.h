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

	Buffer(size_t buffer_size, Buffer::TransferDirection transfer_direction = Bothways, bool map_after_initialization = false);
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
	template<typename T>
	std::vector<T> get_mapped_vector();
	size_t get_buffer_size_in_bytes();

	void bind_to_async_download(size_t offset = 0);
	void bind_to_async_upload(size_t offset = 0);
	void bind_to_index_buffer(size_t offset = 0);
	void bind_to_vertex_buffer(size_t offset = 0);
	void bind_to_storage_buffer(size_t offset, unsigned int texture_slot);
	void bind_to_transform_feedback(size_t offset, unsigned int texture_slot);
	void unbind();

	void map();
	void map(size_t offset, size_t size_in_bytes);
	void unmap();

	void set_data(int managed_buffer_offset_in_bytes, int uploading_data_offset_in_bytes, int size_in_bytes, void* const data);
	void set_data(int size_in_bytes, void* const data);
	template<typename T>
	void set_data(int managed_buffer_offset_in_bytes, int uploading_data_offset_in_bytes, int size_in_bytes, const std::vector<T>& data);
	template<typename T>
	void set_data(const std::vector<T>& data);

	void* get_data(int managed_buffer_offset_in_bytes, int uploading_data_offset_in_bytes, int size_in_bytes);
	void* get_data();
	template<typename T>
	std::vector<T> get_data(int managed_buffer_offset_in_bytes, int uploading_data_offset_in_bytes, int size_in_bytes);
	template<typename T>
	std::vector<T> get_data();

	void clear();
	void copy_buffers();

	std::shared_ptr<Image> get_image();
	void set_image_parameters(Image::ImageParameters parameters);
	void clear_image_parameters();

	template<typename T>
	void push_variable(size_t array_size = 1);

	void force_allocation();

private:
	typedef uint32_t _bind_target;
	
	_bind_target _target_download			= GL_PIXEL_PACK_BUFFER;
	_bind_target _target_upload				= GL_PIXEL_UNPACK_BUFFER;
	_bind_target _target_index_buffer		= GL_ELEMENT_ARRAY_BUFFER;
	_bind_target _target_vertex_buffer		= GL_ARRAY_BUFFER;
	_bind_target _target_storage_buffer		= GL_SHADER_STORAGE_BUFFER;
	_bind_target _target_transform_feedback = GL_TRANSFORM_FEEDBACK_BUFFER;

	TransferDirection _transfer_direction = TransferDirection::Bothways;

	char* _buffer_data = nullptr;
	size_t _buffer_size = 0;
	size_t _mapped_pointer_size = 0;
	bool _map_after_initialization = false;
	std::unique_ptr<Image::ImageParameters> image_parameters = nullptr;

	GLsync _gl_sync_object_download = nullptr;
	GLsync _gl_sync_object_upload = nullptr;

	void _generate_buffer();
	void _allocate_buffer(size_t buffer_size);

	bool _buffer_generated = false;
	bool _buffer_allocated = false;
};

#include "StandardBufferTemplated.h"