#pragma once

#include "Buffer.h"

class ResizableBuffer {
public:

	void		require_and_shrink(size_t minimum_new_size);
	void		require(size_t minimum_new_size);
	void		force_resize(size_t new_size);
	size_t		size();

	Buffer&		get_buffer();

private:
	
	static constexpr size_t min_buffer_size = 1024 * 1024 * 4;
	
	void allocate_buffer(size_t allocation_size = min_buffer_size);

	std::shared_ptr<Buffer> buffer = nullptr;

};