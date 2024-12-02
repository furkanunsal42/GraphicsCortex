#pragma once
#include "GL/glew.h"
#include <memory>
#include <vector>
#include <span>
#include <iostream>
#include "Image.h"

#define BufferElementType(type_name, size, allignment_std430, allignment_std140)	\
	struct type_name {																\
		type_name(uint32_t count = 1) :												\
			_count(count) {}														\
		const uint32_t _size = size;												\
		const uint32_t _allignment_std430 = allignment_std430;						\
		const uint32_t _allignment_std140 = allignment_std140;						\
		const uint32_t _count;														\
	};																			\

class VertexAttributeBuffer;
class Mesh2;

class Buffer {

	friend VertexAttributeBuffer;
	friend Mesh2;

public:

	BufferElementType(boolean,		1,		1,		1				); 
	BufferElementType(float32,		4,		4,		4				);
	BufferElementType(float64,		8,		8,		8				);
	BufferElementType(int8,			1,		1,		1				);
	BufferElementType(int16,		2,		2,		2				);
	BufferElementType(int32,		4,		4,		4				);
	BufferElementType(int64,		8,		8,		8				);
	BufferElementType(vec2,			4*2,	2*4,	2*4				);
	BufferElementType(vec3,			4*3,	3*4,	4*4				);
	BufferElementType(vec4,			4*4,	4*4,	4*4				);
	BufferElementType(mat2x2,		4*2*2,	2*2*4,	2*2*4			);
	BufferElementType(mat3x3,		4*3*3,	3*3*4,	3*4*4/*?*/		);
	BufferElementType(mat4x4,		4*4*4,	4*4*4,	4*4*4			);

	struct structure {
		structure(uint32_t count, int types...) :
			_count(count), _allignment_std430(types), _allignment_std140(types) {}			
		
		const uint32_t _count;
		const uint32_t _allignment_std430;
		const uint32_t _allignment_std140;

		uint32_t compute_allignment_430(int types...) {

		}
		uint32_t compute_allignment_140();



	};										


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
	void bind_as_storage_buffer(size_t offset, unsigned int texture_slot);
	void bind_as_transform_feedback(size_t offset, unsigned int texture_slot);
	void bind_as_vertex_buffer();
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

	void set_data(size_t managed_buffer_offset_in_bytes, size_t uploading_data_offset_in_bytes, size_t size_in_bytes, void* const data);
	void set_data(size_t size_in_bytes, void* const data);
	template<typename T>
	void set_data(size_t managed_buffer_offset_by_count, size_t uploading_data_offset_by_count, size_t count, const std::vector<T>& data);
	template<typename T>
	void set_data(size_t managed_buffer_offset_by_count, size_t uploading_data_offset_by_count, const std::vector<T>& data);
	template<typename T>
	void set_data(const std::vector<T>& data);

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

	void push_variable(size_t element_size_in_bytes, size_t array_count = 1);
	template<typename T>
	void push_variable(size_t array_count = 1);

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
	
	const _bind_target _target_download				= GL_PIXEL_PACK_BUFFER;
	const _bind_target _target_upload				= GL_PIXEL_UNPACK_BUFFER;
	const _bind_target _target_index_buffer			= GL_ELEMENT_ARRAY_BUFFER;
	const _bind_target _target_vertex_buffer		= GL_ARRAY_BUFFER;
	const _bind_target _target_storage_buffer		= GL_SHADER_STORAGE_BUFFER;
	const _bind_target _target_transform_feedback	= GL_TRANSFORM_FEEDBACK_BUFFER;

	MemoryType _buffer_memory_type = MemoryType::GPU_BUFFER;
	size_t _buffer_size = 0;

	char* _buffer_data = nullptr;
	size_t _mapped_pointer_size = 0;
	MapInfo _map_description;

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