#include "ResizableBuffer.h"

Buffer& ResizableBuffer::get_buffer(){
	allocate_buffer();
	return *buffer;
}

void ResizableBuffer::require_and_shrink(size_t maximum_new_size)
{
	require(maximum_new_size);

	if ((size() / 4) > maximum_new_size) {

		size_t new_size = std::max(size() / 2, min_buffer_size);
		force_resize(new_size);
	}
}

void ResizableBuffer::require(size_t minimum_new_size)
{
	if (size() < minimum_new_size) {

		size_t new_size_candidate = 2 * size();
		size_t new_size = std::max(new_size_candidate, minimum_new_size);

		force_resize(new_size);
	}
	
}

void ResizableBuffer::force_resize(size_t new_size){
	
	if (buffer == nullptr) {
		allocate_buffer(new_size);
		return;
	}

	size_t min_size = std::min(new_size, buffer->get_buffer_size_in_bytes());
	std::shared_ptr<Buffer> new_buffer = std::make_shared<Buffer>(new_size);
	buffer->copy_to(0, 0, min_size, *new_buffer);

	buffer = new_buffer;
}

size_t ResizableBuffer::size(){
	return buffer != nullptr ? buffer->get_buffer_size_in_bytes() : 0ull;
}

void ResizableBuffer::allocate_buffer(size_t allocation_size){
	if (buffer != nullptr && buffer->get_buffer_size_in_bytes() >= allocation_size)
		return;

	buffer = std::make_shared<Buffer>(allocation_size);
}

