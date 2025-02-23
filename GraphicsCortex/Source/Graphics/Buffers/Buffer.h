#pragma once
#include "GL/glew.h"
#include <memory>
#include <vector>
#include <span>
#include <iostream>
#include <type_traits>
#include "Image.h"
#include <iostream>
#include <array>

// draw indirect buffer
// dispatch indirect buffer

class VertexAttributeBuffer;
class Mesh;
class ComputeProgram;

class Buffer {

	friend VertexAttributeBuffer;
	friend Mesh;
	friend ComputeProgram;

public:

	struct MapInfo {
	public:
		enum Direction {
			Upload,
			Download,
			Bothways,
			None,
		};

		enum Lifetime {
			Persistant,
			Temporary,
		};

		MapInfo(Direction direction = Bothways, Lifetime lifetime = Persistant);

		Direction direction;
		Lifetime lifetime;
	};

	enum MemoryType {
		GPU_BUFFER,
		CPU_BUFFER,
	};

	Buffer() = delete;
	Buffer(const Buffer& other) = delete;

	Buffer(size_t buffer_size_in_bytes, MapInfo map_upon_initialization, MemoryType buffer_memory_type);
	Buffer(size_t buffer_size_in_bytes, MemoryType buffer_memory_type = GPU_BUFFER);

	~Buffer();
	
	void release();

	void bind_to_async_download();
	void bind_to_async_upload();
	void bind_as_storage_buffer(unsigned int buffer_slot, size_t offset, size_t size);
	void bind_as_transform_feedback(unsigned int buffer_slot, size_t offset, size_t size);
	void bind_as_vertex_buffer();
	void bind_as_index_buffer();
	void unbind();

	bool wait_to_sycronize_download(int64_t timeout_ms);
	bool wait_to_sycronize_download();
	bool is_syncronized_download();
	void set_fence_download();

	bool wait_to_sycronize_upload(int64_t timeout_ms);
	bool wait_to_sycronize_upload();
	bool is_syncronized_upload();
	void set_fence_upload();

	void map(MapInfo map_description = MapInfo());
	void map(size_t offset_in_bytes, size_t size_in_bytes, MapInfo map_description = MapInfo());
	template<typename T>
	void map(size_t offset_by_count, size_t size_by_count, MapInfo map_description = MapInfo());
	void unmap();

	char* get_mapped_pointer();
	template<typename T>
	std::span<T> get_mapped_span();

	bool is_mapped();
	bool is_mapping_persistant();
	size_t get_mapped_buffer_size();

	size_t get_buffer_size_in_bytes();
	template<typename T>
	size_t get_buffer_count();
	template<typename T>
	size_t get_mapped_buffer_count();

	void load_data(size_t managed_buffer_offset_in_bytes, size_t uploading_data_offset_in_bytes, size_t size_in_bytes, void* const data);
	void load_data(size_t size_in_bytes, void* const data);
	template<typename T>
	void load_data(size_t managed_buffer_offset_by_count, size_t uploading_data_offset_by_count, size_t count, const std::vector<T>& data);
	template<typename T>
	void load_data(size_t managed_buffer_offset_by_count, size_t uploading_data_offset_by_count, const std::vector<T>& data);
	template<typename T>
	void load_data(const std::vector<T>& data);

	void get_data(size_t managed_buffer_offset_in_bytes, size_t downloading_data_offset_in_bytes, size_t size_in_bytes, void* data_out);
	void get_data(size_t managed_buffer_offset_in_bytes, size_t downloading_data_offset_in_bytes, void* data_out);
	void* get_data(size_t managed_buffer_offset_in_bytes, size_t size_in_bytes);
	void* get_data(size_t managed_buffer_offset_in_bytes);
	void* get_data();
	template<typename T>
	std::vector<T> get_data(size_t managed_buffer_offset_by_count, size_t count);
	template<typename T>
	std::vector<T> get_data(size_t managed_buffer_offset_by_count);
	template<typename T>
	std::vector<T> get_data();

	void clear();
	template<typename value_type>
	void clear(size_t offset_in_bytes, size_t size_in_bytes, value_type value);
	template<typename value_type>
	void clear(size_t size_in_bytes, value_type value);
	template<typename value_type>
	void clear(value_type value);

	void copy_to(size_t managed_buffer_offset_in_bytes, size_t target_buffer_offset_in_bytes, size_t copy_size_in_bytes, Buffer& target_buffer);
	template<typename T>
	void copy_to(size_t managed_buffer_offset_by_count, size_t target_buffer_offset_by_count, size_t copy_count, Buffer& target_buffer);
	void copy_to(Buffer& target_buffer);

	std::shared_ptr<Image> get_image();
	void set_image_parameters(Image::ImageParameters parameters);
	void clear_image_parameters();

	//template<typename... element_types>
	//void set_memory_structure(layout<element_types...> memory_layout);
	//void clear_memory_structure();
	//void set_structured_data(size_t structured_data_offset_in_bytes, size_t structured_data_size_in_bytes, void* structured_data);
	//void set_structured_data(void* structured_data);
	//void set_structured_data(size_t dynamic_array_count, void* structured_data);
	//void* get_structured_data(size_t structured_data_offset_in_bytes, size_t structured_data_size_in_bytes);
	//void* get_structured_data(size_t dynamic_array_count);
	//void* get_structured_data();
	
	void force_allocation();

	MemoryType get_memory_type();

	bool query_is_map_read();
	bool query_is_map_write();
	bool query_is_map_persistant();
	bool query_is_map_coherent();
	bool query_is_mapped();
	size_t query_map_size();
	size_t query_map_offset();
	bool query_is_immutable();
	bool query_is_dynamic_storage();
	size_t query_buffer_size();
	size_t query_can_map_read();
	size_t query_can_map_write();

private:

	unsigned int id = 0;

	typedef uint32_t _bind_target;
	
	static const _bind_target _target_download				= GL_PIXEL_PACK_BUFFER;
	static const _bind_target _target_upload				= GL_PIXEL_UNPACK_BUFFER;
	static const _bind_target _target_index_buffer			= GL_ELEMENT_ARRAY_BUFFER;
	static const _bind_target _target_vertex_buffer			= GL_ARRAY_BUFFER;
	static const _bind_target _target_storage_buffer		= GL_SHADER_STORAGE_BUFFER;
	static const _bind_target _target_transform_feedback	= GL_TRANSFORM_FEEDBACK_BUFFER;

	MemoryType _buffer_memory_type = MemoryType::GPU_BUFFER;
	size_t _buffer_size = 0;

	char* _buffer_data = nullptr;
	size_t _mapped_pointer_size = 0;
	MapInfo _map_description;

	bool _map_after_initialization = false;
	std::unique_ptr<Image::ImageParameters> image_parameters = nullptr;

	//std::vector<_layout_info> layout_cpu;
	//std::vector<_layout_info> layout_std430;
	//std::vector<_layout_info> layout_std140;

	GLsync _gl_sync_object_download = nullptr;
	GLsync _gl_sync_object_upload = nullptr;

	void _generate_buffer();
	void _allocate_buffer(size_t buffer_size);

	bool _buffer_generated = false;
	bool _buffer_allocated = false;
};

#include "BufferTemplated.h"