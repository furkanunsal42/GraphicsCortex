#pragma once
#include "GL/glew.h"
#include "glm.hpp"
#include <type_traits>
#include <typeinfo>
#include "Debuger.h"
#include <vector>
#include <iostream>

class Texture2D;

class UniformBuffer {
public:
	unsigned int id;
	unsigned int bound_slot = 0;
	bool ever_bound = false;

	UniformBuffer();
	UniformBuffer(const UniformBuffer& other) = delete;
	~UniformBuffer();
	void release();

	void bind(int bind_target);
	void bind(int bind_target, int offset, int size);
	void bind();
	void unbind();

	template<typename T>
	std::enable_if_t<
		std::is_same_v<T, bool> ||
		std::is_same_v<T, int> ||
		std::is_same_v<T, float> ||
		std::is_same_v<T, glm::vec2> ||
		std::is_same_v<T, glm::vec3> ||
		std::is_same_v<T, glm::vec4> ||
		std::is_same_v<T, glm::mat2> ||
		std::is_same_v<T, glm::mat3> ||
		std::is_same_v<T, glm::mat4> ||
		std::is_same_v<T, Texture2D> , void>
		push_variable()
	{
		_push_varible_size(_get_gl_std140_size<T>());
	}
	void push_variable_array(int array_size);

	void set_data(int managed_buffer_offset_in_bytes, int uploading_data_offset_in_bytes, int size_in_bytes, void* const data);
	void set_data(int varible_index, void* const data);

	template<typename T>
	void set_data(int managed_buffer_offset_in_bytes, int uploading_data_offset_in_bytes, int size_in_bytes, const std::vector<T>& data) {
		if (size_in_bytes == 0) return;
		if (managed_buffer_offset_in_bytes < 0 || uploading_data_offset_in_bytes < 0 || size_in_bytes < 0) {
			std::cout << "[OpenGL Error] UniformBuffer tried to set_data() but offset and size was out of bound" << std::endl;
			ASSERT(false)
		}
		if (uplading_data_offset_in_bytes + size_in_bytes > data.size() * sizeof(T)) {
			std::cout << "[OpenGL Error] UniformBuffer tried to set_data() but given data's size is lower than given offset + size" << std::endl;
			ASSERT(false);
		}
		if (!_buffer_generated) {
			std::cout << "[OpenGL Error] UniformBuffer tried to set_data() but resource was released" << std::endl;
			ASSERT(false);
		}
		_allocate_buffer();

		set_data(managed_buffer_offset_in_bytes, uploading_data_offset_in_bytes, size_in_bytes, (void*)((char*)data.data + custom_data_offset_in_bytes));
	}
	
	void clear();

	void upload_data();

	void* cpu_data = nullptr;

private:

	int _buffer_size = 0;
	
	struct _range {
		_range(int begin, int end) {
			this->begin = begin;
			this->end = end;
		}
		int begin;
		int end;
	};

	std::vector<_range> _updated_ranges;

	_range _compute_bounding_range();

	template<typename T>
	std::enable_if_t<
		std::is_same_v<T, bool> ||
		std::is_same_v<T, int> ||
		std::is_same_v<T, float> ||
		std::is_same_v<T, glm::vec2> ||
		std::is_same_v<T, glm::vec3> ||
		std::is_same_v<T, glm::vec4> ||
		std::is_same_v<T, glm::mat2> ||
		std::is_same_v<T, glm::mat3> ||
		std::is_same_v<T, glm::mat4> || 
		std::is_same_v<T, Texture2D>, int>
		_get_gl_std140_size()
	{
		const int N = 4; // base size
		
		if (typeid(T) == typeid(bool))			return N;
		if (typeid(T) == typeid(int))			return N;
		if (typeid(T) == typeid(float))			return N;
		if (typeid(T) == typeid(glm::vec2))		return 2*N;
		if (typeid(T) == typeid(glm::vec3))		return 4*N;
		if (typeid(T) == typeid(glm::vec4))		return 4*N;
		if (typeid(T) == typeid(glm::mat2))		return 2*4*N;
		if (typeid(T) == typeid(glm::mat3))		return 3*4*N;
		if (typeid(T) == typeid(glm::mat4))		return 4 * 4 * N;
		if (typeid(T) == typeid(Texture2D))		return 2*N;
		ASSERT(false);
	}

	void _push_varible_size(int size);
	std::vector<int> _pushed_variable_sizes;
	std::vector<int> _pushed_variable_offsets;

	bool _buffer_generated = false;
	bool _buffer_allocated = false;
	bool _user_data_uploaded = false;

	void _allocate_buffer();

	unsigned int _target = GL_UNIFORM_BUFFER;
};