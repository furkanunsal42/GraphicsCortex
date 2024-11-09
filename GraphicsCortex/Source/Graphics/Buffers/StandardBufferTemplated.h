#pragma once

#include "StandardBuffer.h"
#include "Debuger.h"

#include "glm.hpp"
#include <iostream>

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
	
	set_data(managed_buffer_offset_in_bytes, uploading_data_offset_in_bytes, std::min(data.size() * sizeof(T) - uploading_data_offset_in_bytes, size_in_bytes), &data[0]);
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

template<typename T>
inline void Buffer::clear(size_t offset_in_bytes, size_t size_in_bytes, T value)
{
	static_assert(
		std::is_same_v<T, int8_t> ||
		std::is_same_v<T, int16_t> ||
		std::is_same_v<T, int32_t> ||
		std::is_same_v<T, uint8_t> ||
		std::is_same_v<T, uint16_t> ||
		std::is_same_v<T, uint32_t> ||
		std::is_same_v<T, float> ||
		std::is_same_v<T, glm::vec2> ||
		std::is_same_v<T, glm::ivec2> ||
		std::is_same_v<T, glm::uvec2> ||
		std::is_same_v<T, glm::vec3> ||
		std::is_same_v<T, glm::ivec3> ||
		std::is_same_v<T, glm::uvec3> ||
		std::is_same_v<T, glm::vec4> ||
		std::is_same_v<T, glm::ivec4> ||
		std::is_same_v<T, glm::uvec4>
		);

	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] released Buffer tried to clear()" << std::endl;
		ASSERT(false);
	}

	_allocate_buffer(_buffer_size);

	GLCall(glClearNamedBufferSubData(id, get_GL_buffer_internal_format<T>(), offset_in_bytes, size_in_bytes, get_GL_buffer_format<T>(), get_GL_buffer_type<T>(), &value));
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
void copy_to(size_t managed_buffer_offset_by_count, size_t target_buffer_offset_by_count, size_t copy_count, Buffer& target_buffer) {
	
	size_t managed_buffer_offset_in_bytes = managed_buffer_offset_by_count * sizeof(T);
	size_t target_buffer_offset_in_bytes = target_buffer_offset_by_count * sizeof(T);
	size_t copy_size_in_bytes = copy_count * sizeof(T);

	copy_to(managed_buffer_offset_in_bytes, target_buffer_offset_in_bytes, copy_size_in_bytes, target_buffer);
}
