#pragma once

#include "StandardBuffer.h"

template<typename T>
inline std::vector<T> Buffer::get_mapped_vector()
{
	std::vector<T> vector((T*)_buffer_data, (T*)((char*)_buffer_data + _mapped_pointer_size));
	return vector;
}
