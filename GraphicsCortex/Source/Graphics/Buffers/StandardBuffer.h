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

class VertexAttributeBuffer;
class Mesh2;

class Buffer {

	friend VertexAttributeBuffer;
	friend Mesh2;

public:

	#define BufferElementType(type_name, size_std430, alignment_std430, array_size_std430, array_alignment_std430, size_std140, alignment_std140, array_size_std140, array_alignment_std140)\
		struct type_name {																						\
			type_name():																						\
				_offset_map(-1), _stride_map(-1) {}																\
			type_name(size_t offset_map, size_t stride_map, size_t count) :													\
				_offset_map(offset_map), _stride_map(stride_map) {}												\
			const size_t _offset_map;																			\
			const size_t _stride_map;																			\
			const uint32_t _size_std430 = size_std430;															\
			const uint32_t _alignment_std430 = alignment_std430;												\
			const uint32_t _size_std140 = size_std140;															\
			const uint32_t _alignment_std140 = alignment_std140;												\
			const uint32_t _count = 1;																			\
			const bool _is_array_type = false;																	\
			const bool _is_dynamic_length = false;																\
		};																										\
		struct type_name##_array {																				\
			type_name##_array(size_t offset_map, size_t stride_map, uint32_t count) :							\
				_offset_map(offset_map), _stride_map(stride_map), _count(count), _is_dynamic_length(false) {}	\
			/*type_name##_array(size_t offset_map, size_t stride_map) :											\
				_offset_map(offset_map), _stride_map(stride_map), _count(1), _is_dynamic_length(true) {}		\
			*/type_name##_array(uint32_t count) :																	\
				_offset_map(-1), _stride_map(-1), _count(count), _is_dynamic_length(false) {}					\
			type_name##_array() :																				\
				_offset_map(-1), _stride_map(-1), _count(1), _is_dynamic_length(true) {}						\
			const size_t _offset_map;																			\
			const size_t _stride_map;																			\
			const uint32_t _size_std430 = array_size_std430;													\
			const uint32_t _alignment_std430 = array_alignment_std430;											\
			const uint32_t _size_std140 = array_size_std140;													\
			const uint32_t _alignment_std140 = array_alignment_std140;											\
			const uint32_t _count;																				\
			const bool _is_array_type = true;																	\
			const bool _is_dynamic_length = false;																\
		};																										\

	//					name		size430		align430	size430_arr align430_array	size140		align140	size140_arr	align140_array
	BufferElementType(boolean,		4,			4,			4,			4,				4,			4,			4*4,		4*4		); 
	BufferElementType(float32,		4,			4,			4,			4,				4,			4,			4*4,		4*4		);
	BufferElementType(float64,		8,			8,			8,			8,				8,			8,			4*4,		4*4		);
	BufferElementType(int32,		4,			4,			4,			4,				4,			4,			4*4,		4*4		);
	BufferElementType(int64,		8,			8,			8,			8,				8,			8,			4*4,		4*4		);
	BufferElementType(vec2,			2*4,		2*4,		2*4,		2*4,			2*4,		2*4,		4*4,		4*4		);
	BufferElementType(vec3,			3*4,		4*4,		3*4,		3*4,			3*4,		4*4,		4*4,		4*4		);
	BufferElementType(vec4,			4*4,		4*4,		4*4,		4*4,			4*4,		4*4,		4*4,		4*4		);
	BufferElementType(mat2x2,		2*2*4,		2*2*4,		2*2*4,		2*2*4,			2*4*4,		2*4*4,		2*4*4,		2*4*4	);
	BufferElementType(mat4x4,		4*4*4,		4*4*4,		4*4*4,		4*4*4,			4*4*4,		4*4*4,		4*4*4,		4*4*4	);

	/*
	template<typename... element_types>
	struct structure_array;

	template<typename... element_types>
	struct structure {
		structure(element_types... types) :
			_alignment_std430(compute_alignment_430(types...)), 
			_size_std430(compute_size_430(types...)), 
			_alignment_std140(compute_alignment_140(types...)), 
			_size_std140(compute_size_140(types...)) {}
		
		static_assert(((
			std::is_same_v<element_types, boolean>			||
			std::is_same_v<element_types, float32>			||
			std::is_same_v<element_types, float64>			||
			std::is_same_v<element_types, int32>			||
			std::is_same_v<element_types, int64>			||
			std::is_same_v<element_types, vec2>				||
			std::is_same_v<element_types, vec4>				||
			std::is_same_v<element_types, mat2x2>			||
			std::is_same_v<element_types, mat4x4>			||
			std::is_same_v<element_types, boolean_array>	||
			std::is_same_v<element_types, float32_array>	||
			std::is_same_v<element_types, float64_array>	||
			std::is_same_v<element_types, int32_array>		||
			std::is_same_v<element_types, int64_array>		||
			std::is_same_v<element_types, vec2_array>		||
			std::is_same_v<element_types, vec4_array>		||
			std::is_same_v<element_types, mat2x2_array>		||
			std::is_same_v<element_types, mat4x4_array>		||
			std::is_same_v<element_types, structure<element_types>>		||
			std::is_same_v<element_types, structure_array<element_types>>) && ...),
			"Buffer::structure is defined with unsupperted types, use Buffer::float32, Buffer::int32, Buffer::vec4 etc.."
			);

		const uint32_t _size_std430;
		const uint32_t _alignment_std430;
		const uint32_t _size_std140;
		const uint32_t _alignment_std140;
		const uint32_t _count = 1;
		const bool _is_array_type = false;											
		const bool _is_dynamic_length = false;										
		constexpr uint32_t compute_alignment_430(element_types... types) {
			uint32_t max_alignment = 0;
			([&] { max_alignment = std::max<uint32_t>(max_alignment, types._size_std430); }(), ...);
			return max_alignment;
		}

		constexpr uint32_t compute_size_430(element_types... types) {
			uint32_t total_size = 0;
			([&] { total_size += types._size_std430 * types._count; }(), ...);
			return total_size;
		}

		constexpr uint32_t compute_alignment_140(element_types... types) {
			uint32_t max_alignment = 0;
			([&] { max_alignment = std::max<uint32_t>(max_alignment, types._size_std140); }(), ...);
			return max_alignment;
		}

		constexpr uint32_t compute_size_140(element_types... types) {
			uint32_t total_size = 0;
			([&] { total_size += types._size_std140 * types._count; }(), ...);
			return total_size;
		}
	};										

	template<typename... element_types>
	struct structure_array {
		structure_array(uint32_t count, element_types... types) :
			_count(count),
			_is_dynamic_length(false),
			_alignment_std430(compute_alignment_430(types...)),
			_size_std430(compute_size_430(types...)),
			_alignment_std140(compute_alignment_140(types...)),
			_size_std140(compute_size_140(types...)) {}

		structure_array(element_types... types) :
			_count(1),
			_is_dynamic_length(true),
			_alignment_std430(compute_alignment_430(types...)),
			_size_std430(compute_size_430(types...)),
			_alignment_std140(compute_alignment_140(types...)),
			_size_std140(compute_size_140(types...)) {}

		const uint32_t _size_std430;
		const uint32_t _alignment_std430;
		const uint32_t _size_std140;
		const uint32_t _alignment_std140;
		const uint32_t _count = 1;
		const bool _is_array_type = true;
		const bool _is_dynamic_length = false;

		static_assert(((
			std::is_same_v<element_types, boolean>			||
			std::is_same_v<element_types, float32>			||
			std::is_same_v<element_types, float64>			||
			std::is_same_v<element_types, int32>			||
			std::is_same_v<element_types, int64>			||
			std::is_same_v<element_types, vec2>				||
			std::is_same_v<element_types, vec4>				||
			std::is_same_v<element_types, mat2x2>			||
			std::is_same_v<element_types, mat4x4>			||
			std::is_same_v<element_types, boolean_array>	||
			std::is_same_v<element_types, float32_array>	||
			std::is_same_v<element_types, float64_array>	||
			std::is_same_v<element_types, int32_array>		||
			std::is_same_v<element_types, int64_array>		||
			std::is_same_v<element_types, vec2_array>		||
			std::is_same_v<element_types, vec4_array>		||
			std::is_same_v<element_types, mat2x2_array>		||
			std::is_same_v<element_types, mat4x4_array>		||
			std::is_same_v<element_types, structure<element_types>>		||
			std::is_same_v<element_types, structure_array<element_types>>) && ...),
			"Buffer::structure_array is defined with unsupperted types, use Buffer::float32, Buffer::int32, Buffer::vec4 etc.."
			);

		constexpr uint32_t compute_alignment_430(element_types... types) {
			uint32_t max_alignment = 0;
			([&] { max_alignment = std::max<uint32_t>(max_alignment, types._size_std430); }(), ...);
			return max_alignment;
		}

		constexpr uint32_t compute_size_430(element_types... types) {
			uint32_t total_size = 0;
			([&] { total_size += types._size_std430 * types._count; }(), ...);
			return total_size;
		}

		constexpr uint32_t compute_alignment_140(element_types... types) {
			uint32_t max_alignment = 0;
			([&] { max_alignment = std::max<uint32_t>(max_alignment, types._size_std140); }(), ...);
			return max_alignment;
		}

		constexpr uint32_t compute_size_140(element_types... types) {
			uint32_t total_size = (types._size_std430 + ...);
			return total_size;
		}
	};
	*/

	template<typename... element_types>
	struct layout {
		layout(element_types... types) //:
		{
			_compute_cpu_layout(types...);
			_compute_std140_layout(types...);
			_compute_std430_layout(types...);
		}

		static_assert(((
			std::is_same_v<element_types, boolean>			||
			std::is_same_v<element_types, float32> 			||
			std::is_same_v<element_types, float64> 			||
			std::is_same_v<element_types, int32> 			||
			std::is_same_v<element_types, int64> 			||
			std::is_same_v<element_types, vec2>				||
			std::is_same_v<element_types, vec3> 			||
			std::is_same_v<element_types, vec4> 			||
			std::is_same_v<element_types, mat2x2> 			||
			std::is_same_v<element_types, mat4x4> 			||
			std::is_same_v<element_types, boolean_array>	||
			std::is_same_v<element_types, float32_array>	||
			std::is_same_v<element_types, float64_array>	||
			std::is_same_v<element_types, int32_array> 		||
			std::is_same_v<element_types, int64_array> 		||
			std::is_same_v<element_types, vec2_array> 		||
			std::is_same_v<element_types, vec4_array> 		||
			std::is_same_v<element_types, mat2x2_array>		||
			std::is_same_v<element_types, mat4x4_array>/*		||
			std::is_same_v<element_types, structure<element_types>> ||
			std::is_same_v<element_types, structure_array<element_types>>*/) && ...),
			"Buffer::layout is defined with unsupperted types, use Buffer::float32, Buffer::int32, Buffer::vec4 etc.."
			);

		struct _layout_info {
			_layout_info() = default;
			_layout_info(size_t begin_offset, size_t count, size_t element_stride) :
				begin_offset(begin_offset), count(count), element_stride(element_stride) {}
			
			size_t begin_offset = 0;
			size_t count = 0;
			size_t element_stride = 0;
		};

		std::array<_layout_info, sizeof...(element_types)> layout_cpu;
		std::array<_layout_info, sizeof...(element_types)> layout_std430;
		std::array<_layout_info, sizeof...(element_types)> layout_std140;

		constexpr void _compute_cpu_layout(element_types... types) {
			
			int i = 0;
			size_t offset = 0;

			([&] {

				layout_cpu[i].begin_offset = types._offset_map;
				layout_cpu[i].element_stride = types._stride_map;
				i++;
				
			}(), ...);

		}

		constexpr void _compute_std140_layout(element_types... types) {

			int i = 0;
			size_t offset = 0;

			([&] {

				if (offset % types._alignment_std140 != 0)
					offset = (offset / types._alignment_std140 + 1) * types._alignment_std140;

				layout_std140[i].begin_offset = offset;
				layout_std140[i].count = types._count;
				layout_std140[i].element_stride = types._size_std140;

				offset += types._size_std140 * types._count;

				i++;
			}(), ...);
		}
		
		constexpr void _compute_std430_layout(element_types... types) {
			int i = 0;
			size_t offset = 0;

			([&] {

			if (offset % types._alignment_std430 != 0)
				offset = (offset / types._alignment_std430 + 1) * types._alignment_std430;

			layout_std430[i].begin_offset = offset;
			layout_std430[i].count = types._count;
			layout_std430[i].element_stride = types._size_std430;

			offset += types._size_std430 * types._count;

			i++;
				}(), ...);
		}

	};


#define layout_map_to(structure, member) offsetof(structure, member), sizeof(std::remove_all_extents<decltype(structure##:: member)>::type), sizeof(((structure*) 0)->member) / sizeof(std::remove_all_extents<decltype(cpu_layout_struct::c)>::type) 

	struct x {
		int32_t a;
		double b;
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