#pragma once

#include "StandardBuffer.h"
#include "Debuger.h"

#include "glm.hpp"
#include <iostream>
#include <StandardBuffer.h>

template<typename T>
inline std::span<T> Buffer::get_mapped_span()
{
	std::span<T> span((T*)_buffer_data, (T*)((char*)_buffer_data + _mapped_pointer_size));
	return span;
}

template<typename T>
void map(size_t offset_by_count, size_t size_by_count, Buffer::MapInfo map_description) {
	map(offset_by_count * sizeof(T), size_by_count * sizeof(T), map_description);
}

template<typename T>
inline size_t Buffer::get_buffer_count()
{
	return _buffer_size / sizeof(T);
}

template<typename T>
inline size_t Buffer::get_mapped_buffer_count()
{
	return _mapped_pointer_size / sizeof(T);
}

template<typename T>
inline void Buffer::set_data(size_t managed_buffer_offset_by_count, size_t uploading_data_offset_by_count, size_t count, const std::vector<T>& data)
{
	if (data.size() == 0) return;

	size_t managed_buffer_offset_in_bytes = managed_buffer_offset_by_count * sizeof(T);
	size_t uploading_data_offset_in_bytes = uploading_data_offset_by_count * sizeof(T);
	size_t size_in_bytes = count * sizeof(T);
	
	set_data(managed_buffer_offset_in_bytes, uploading_data_offset_in_bytes, std::min(data.size() * sizeof(T) - uploading_data_offset_in_bytes, size_in_bytes), (void*) & data[0]);
}

template<typename T>
inline void Buffer::set_data(size_t managed_buffer_offset_by_count, size_t uploading_data_offset_by_count, const std::vector<T>& data)
{
	if (data.size() == 0) return;

	size_t managed_buffer_offset_in_bytes = managed_buffer_offset_by_count * sizeof(T);
	size_t uploading_data_offset_in_bytes = uploading_data_offset_by_count * sizeof(T);

	set_data(managed_buffer_offset_in_bytes, uploading_data_offset_in_bytes, data.size() * sizeof(T), (void*) &data[0]);
}

template<typename T>
inline void Buffer::set_data(const std::vector<T>& data)
{
	set_data(0, 0, data);
}

template<typename T>
inline std::vector<T> Buffer::get_data(size_t managed_buffer_offset_by_count, size_t count){
	
	size_t managed_buffer_offset_in_bytes = managed_buffer_offset_by_count * sizeof(T);
	size_t size_in_bytes = count * sizeof(T);

	char* buffer = get_data(managed_buffer_offset_in_bytes, size_in_bytes);

	return std::vector<T>((T*)buffer, (T*)(buffer + size_in_bytes));
}

template<typename T>
inline std::vector<T> Buffer::get_data(size_t managed_buffer_offset_by_count){

	size_t managed_buffer_offset_in_bytes = managed_buffer_offset_by_count * sizeof(T);

	char* buffer = get_data(managed_buffer_offset_in_bytes);

	return std::vector<T>((T*)buffer, (T*)(buffer + _buffer_size - managed_buffer_offset_in_bytes));
}

template<typename T>
inline std::vector<T> Buffer::get_data(){

	char* buffer = (char*)get_data();

	return std::vector<T>((T*)buffer, (T*)(buffer + _buffer_size));
}

namespace {
	template<typename T>
	unsigned int get_GL_buffer_internal_format();
	template<typename T>
	unsigned int get_GL_buffer_format();
	template<typename T>
	unsigned int get_GL_buffer_type();

	
	template<>
	unsigned int get_GL_buffer_internal_format<int8_t>() {
		return GL_R8I;
	}
	template<>
	unsigned int get_GL_buffer_internal_format<int16_t>() {
		return GL_R16I;
	}
	template<>
	unsigned int get_GL_buffer_internal_format<int32_t>() {
		return GL_R32I;
	}
	template<>
	unsigned int get_GL_buffer_internal_format<uint8_t>() {
		return GL_R8UI;
	}
	template<>
	unsigned int get_GL_buffer_internal_format<uint16_t>() {
		return GL_R16UI;
	}
	template<>
	unsigned int get_GL_buffer_internal_format<uint32_t>() {
		return GL_R32UI;
	}
	template<>
	unsigned int get_GL_buffer_internal_format<float>() {
		return GL_R32F;
	}
	template<>
	unsigned int get_GL_buffer_internal_format<glm::vec2>() {
		return GL_RG32F;
	}
	template<>
	unsigned int get_GL_buffer_internal_format<glm::ivec2>() {
		return GL_RG32I;
	}
	template<>
	unsigned int get_GL_buffer_internal_format<glm::uvec2>() {
		return GL_RG32UI;
	}
	template<>
	unsigned int get_GL_buffer_internal_format<glm::vec3>() {
		return GL_RGB32F;
	}
	template<>
	unsigned int get_GL_buffer_internal_format<glm::ivec3>() {
		return GL_RGB32I;
	}
	template<>
	unsigned int get_GL_buffer_internal_format<glm::uvec3>() {
		return GL_RGB32UI;
	}
	template<>
	unsigned int get_GL_buffer_internal_format<glm::vec4>() {
		return GL_RGBA32F;
	}
	template<>
	unsigned int get_GL_buffer_internal_format<glm::ivec4>() {
		return GL_RGBA32I;
	}
	template<>
	unsigned int get_GL_buffer_internal_format<glm::uvec4>() {
		return GL_RGBA32UI;
	}

	// ----------------------
	
	template<>
	unsigned int get_GL_buffer_format<int8_t>() {
		return GL_RED;
	}
	template<>
	unsigned int get_GL_buffer_format<int16_t>() {
		return GL_RED;
	}
	template<>
	unsigned int get_GL_buffer_format<int32_t>() {
		return GL_RED;
	}
	template<>
	unsigned int get_GL_buffer_format<uint8_t>() {
		return GL_RED;
	}
	template<>
	unsigned int get_GL_buffer_format<uint16_t>() {
		return GL_RED;
	}
	template<>
	unsigned int get_GL_buffer_format<uint32_t>() {
		return GL_RED;
	}
	template<>
	unsigned int get_GL_buffer_format<float>() {
		return GL_RED;
	}
	template<>
	unsigned int get_GL_buffer_format<glm::vec2>() {
		return GL_RG;
	}
	template<>
	unsigned int get_GL_buffer_format<glm::ivec2>() {
		return GL_RG;
	}
	template<>
	unsigned int get_GL_buffer_format<glm::uvec2>() {
		return GL_RG;
	}
	template<>
	unsigned int get_GL_buffer_format<glm::vec3>() {
		return GL_RGB;
	}
	template<>
	unsigned int get_GL_buffer_format<glm::ivec3>() {
		return GL_RGB;
	}
	template<>
	unsigned int get_GL_buffer_format<glm::uvec3>() {
		return GL_RGB;
	}
	template<>
	unsigned int get_GL_buffer_format<glm::vec4>() {
		return GL_RGBA;
	}
	template<>
	unsigned int get_GL_buffer_format<glm::ivec4>() {
		return GL_RGBA;
	}
	template<>
	unsigned int get_GL_buffer_format<glm::uvec4>() {
		return GL_RGBA;
	}

	// ----------------------
	
	template<>
	unsigned int get_GL_buffer_type<int8_t>() {
		return GL_BYTE;
	}
	template<>
	unsigned int get_GL_buffer_type<int16_t>() {
		return GL_SHORT;
	}
	template<>
	unsigned int get_GL_buffer_type<int32_t>() {
		return GL_INT;
	}
	template<>
	unsigned int get_GL_buffer_type<uint8_t>() {
		return GL_UNSIGNED_BYTE;
	}
	template<>
	unsigned int get_GL_buffer_type<uint16_t>() {
		return GL_UNSIGNED_SHORT;
	}
	template<>
	unsigned int get_GL_buffer_type<uint32_t>() {
		return GL_UNSIGNED_INT;
	}
	template<>
	unsigned int get_GL_buffer_type<float>() {
		return GL_FLOAT;
	}
	template<>
	unsigned int get_GL_buffer_type<glm::vec2>() {
		return GL_FLOAT;
	}
	template<>
	unsigned int get_GL_buffer_type<glm::ivec2>() {
		return GL_INT;
	}
	template<>
	unsigned int get_GL_buffer_type<glm::uvec2>() {
		return GL_UNSIGNED_INT;
	}
	template<>
	unsigned int get_GL_buffer_type<glm::vec3>() {
		return GL_FLOAT;
	}
	template<>
	unsigned int get_GL_buffer_type<glm::ivec3>() {
		return GL_INT;
	}
	template<>
	unsigned int get_GL_buffer_type<glm::uvec3>() {
		return GL_UNSIGNED_INT;
	}
	template<>
	unsigned int get_GL_buffer_type<glm::vec4>() {
		return GL_FLOAT;
	}
	template<>
	unsigned int get_GL_buffer_type<glm::ivec4>() {
		return GL_INT;
	}
	template<>
	unsigned int get_GL_buffer_type<glm::uvec4>() {
		return GL_UNSIGNED_INT;
	}
}

template<typename value_type>
inline void Buffer::clear(size_t offset_in_bytes, size_t size_in_bytes, value_type value)
{
	static_assert(
		std::is_same_v<value_type, int8_t> ||
		std::is_same_v<value_type, int16_t> ||
		std::is_same_v<value_type, int32_t> ||
		std::is_same_v<value_type, uint8_t> ||
		std::is_same_v<value_type, uint16_t> ||
		std::is_same_v<value_type, uint32_t> ||
		std::is_same_v<value_type, float> ||
		std::is_same_v<value_type, glm::vec2> ||
		std::is_same_v<value_type, glm::ivec2> ||
		std::is_same_v<value_type, glm::uvec2> ||
		std::is_same_v<value_type, glm::vec3> ||
		std::is_same_v<value_type, glm::ivec3> ||
		std::is_same_v<value_type, glm::uvec3> ||
		std::is_same_v<value_type, glm::vec4> ||
		std::is_same_v<value_type, glm::ivec4> ||
		std::is_same_v<value_type, glm::uvec4>
		);

	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released Buffer tried to clear()" << std::endl;
		ASSERT(false);
	}

	_allocate_buffer(_buffer_size);

	GLCall(glClearNamedBufferSubData(id, get_GL_buffer_internal_format<value_type>(), offset_in_bytes, size_in_bytes, get_GL_buffer_format<value_type>(), get_GL_buffer_type<value_type>(), &value));
}

template<typename value_type>
inline void Buffer::clear(size_t size_in_bytes, value_type value)
{
	clear<value_type>(0, size_in_bytes, value);
}

template<typename value_type>
inline void Buffer::clear(value_type value)
{
	clear<value_type>(0, _buffer_size, value);
}

template<typename T>
void Buffer::copy_to(size_t managed_buffer_offset_by_count, size_t target_buffer_offset_by_count, size_t copy_count, Buffer& target_buffer) {
	
	size_t managed_buffer_offset_in_bytes = managed_buffer_offset_by_count * sizeof(T);
	size_t target_buffer_offset_in_bytes = target_buffer_offset_by_count * sizeof(T);
	size_t copy_size_in_bytes = copy_count * sizeof(T);

	copy_to(managed_buffer_offset_in_bytes, target_buffer_offset_in_bytes, copy_size_in_bytes, target_buffer);
}

template<typename... element_types>
Buffer::layout<element_types...>::layout(element_types... types)
{
	static_assert(((
		std::is_same_v<element_types, boolean> ||
		std::is_same_v<element_types, float32> ||
		std::is_same_v<element_types, float64> ||
		std::is_same_v<element_types, int32> ||
		std::is_same_v<element_types, int64> ||
		std::is_same_v<element_types, vec2> ||
		std::is_same_v<element_types, vec3> ||
		std::is_same_v<element_types, vec4> ||
		std::is_same_v<element_types, mat2x2> ||
		std::is_same_v<element_types, mat4x4>
		/*std::is_same_v<element_types, structure<element_types>> ||
		std::is_same_v<element_types, structure_array<element_types>>*/) && ...),
		"Buffer::layout is defined with unsupperted types, use Buffer::float32, Buffer::int32, Buffer::vec4 etc.."
		);

	_compute_cpu_layout(types...);
	_compute_std140_layout(types...);
	_compute_std430_layout(types...);
}

template<typename... element_types>
constexpr void Buffer::layout<element_types...>::_compute_cpu_layout(element_types... types) {

	int i = 0;
	size_t offset = 0;

	([&] {

		layout_cpu[i].begin_offset = types._offset_map;
		layout_cpu[i].element_stride = types._stride_map;
		layout_cpu[i].count = types._count;
		i++;

	}(), ...);

}

template<typename... element_types>
constexpr void Buffer::layout<element_types...>::_compute_std140_layout(element_types... types) {

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

template<typename... element_types>
constexpr void Buffer::layout<element_types...>::_compute_std430_layout(element_types... types) {
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

template<typename... element_types>
void Buffer::set_memory_structure(layout<element_types...> memory_layout) {

	int32_t element_count = memory_layout.layout_cpu.size();

	if (element_count == 0) {
		std::cout << "[OpenGL Error] Buffer::set_memory_structure() is called with an empty layout" << std::endl;
		ASSERT(false);
	}

	bool is_std_140_compatible = true;
	bool is_std_430_compatible = true;

	for (int i = 1; i < memory_layout.layout_cpu.size(); i++) {
		if (!is_std_430_compatible && !is_std_140_compatible)
			break;
		
		if (memory_layout.layout_cpu[i].begin_offset != memory_layout.layout_std430[i].begin_offset)
			is_std_430_compatible = false;
	
		if (memory_layout.layout_cpu[i].begin_offset != memory_layout.layout_std140[i].begin_offset)
			is_std_140_compatible = false;
	}

	if (!is_std_430_compatible && !is_std_140_compatible) {
		std::cout << "[OpenGL Warning] Buffer::set_memory_structure() is called with a layout that isn't compatible with std140 or std430, uncompatible layouts will cause performance penalty" << std::endl;
	}
		


}