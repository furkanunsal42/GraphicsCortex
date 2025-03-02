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

	UniformBuffer();
	UniformBuffer(const UniformBuffer& other) = delete;
	~UniformBuffer();
	void release();

	void bind(int bind_target);
	void bind(int bind_target, int offset, int size);
	void bind();
	void unbind();

	template<typename T>
	void push_variable();
	void push_variable_array(int array_size);

	void set_data(int managed_buffer_offset_in_bytes, int uploading_data_offset_in_bytes, int size_in_bytes, void* const data);
	void set_data(int varible_index, void* const data);

	template<typename T>
	void set_data(int managed_buffer_offset_in_bytes, int uploading_data_offset_in_bytes, int size_in_bytes, const std::vector<T>& data);
	
	void clear();
	size_t get_buffer_size_in_bytes();
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
	int _get_gl_std140_size();

	void _push_varible_size(int size);
	std::vector<int> _pushed_variable_sizes;
	std::vector<int> _pushed_variable_offsets;

	bool _buffer_generated = false;
	bool _buffer_allocated = false;
	bool _user_data_uploaded = false;

	void _allocate_buffer();

	unsigned int _target = GL_UNIFORM_BUFFER;
};

#include "UniformBufferTemplated.h"