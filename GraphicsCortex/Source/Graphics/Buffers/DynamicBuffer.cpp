#include "DynamicBuffer.h"

void DynamicBuffer::allocate_buffer(size_t allocation_size)
{
    if (buffer != nullptr) return;

    allocation_size = std::max(allocation_size, min_buffer_size);

    buffer = std::make_shared<Buffer>(allocation_size);

    blocks.clear();
    blocks.push_back(Block{
        .offset = 0,
        .size = allocation_size,
        .handle = invalid_handle,
        .is_free = true,
        });
}

DynamicBuffer::BufferHandle DynamicBuffer::allocate_handle(size_t offset)
{
	if (free_handles.size() != 0) {
		
		BufferHandle handle = free_handles.back();
		free_handles.pop_back();
		
		resolved_offsets[handle.index].offset_in_bytes = offset;
		handle.generation = resolved_offsets[handle.index].generation;

		return handle;
	}
	else {

		BufferHandle handle {
			.index = resolved_offsets.size(),
			.generation = 0
		};

		resolved_offsets.push_back(ResolvedOffset{
			.offset_in_bytes = offset, 
			.generation = 0,
			});

		return handle;
	}
}

void DynamicBuffer::free_handle(BufferHandle handle)
{
	if (handle == invalid_handle) return;

	if (resolved_offsets[handle.index].generation != handle.generation) {
		std::cout << "[OpenGL Error] DynamicBuffer::free_handle() is called but given handle is expired" << std::endl;
		ASSERT(false);
	}

	resolved_offsets[handle.index].offset_in_bytes = 0;
	resolved_offsets[handle.index].generation++;

	free_handles.push_back(handle);
}

void* DynamicBuffer::gpu_malloc(size_t size_in_bytes)
{
    allocate_buffer(size_in_bytes);

    auto attempt_allocation = [&]() -> BufferHandle {
        for (auto it = blocks.begin(); it != blocks.end(); ++it) {
            if (it->is_free && it->size >= size_in_bytes) {

                size_t leftover_size = it->size - size_in_bytes;

                if (leftover_size > 0) {
                    Block new_free_block{
                        .offset = it->offset + size_in_bytes,
                        .size = leftover_size,
                        .handle = invalid_handle,
                        .is_free = true
                    };
                    blocks.insert(std::next(it), new_free_block);
                }

                it->size = size_in_bytes;
                it->is_free = false;

                BufferHandle handle = allocate_handle(it->offset);
                it->handle = handle;
                resolved_offsets[handle.index].block_it = it;

                total_used_size += size_in_bytes;

                return handle;
            }
        }
        return invalid_handle;
        };

    
    size_t total_free_space = total_capacity - total_used_size;

    if (total_free_space < size_in_bytes) {
        grow(size_in_bytes);
        return std::bit_cast<void*>(attempt_allocation());
    }

    BufferHandle handle = attempt_allocation();
    if (handle != invalid_handle) return std::bit_cast<void*>(handle);

    defragment(total_capacity);
    handle = attempt_allocation();
    
    if (handle == invalid_handle) {
        ASSERT(false && "Out of GPU Memory even after defragmentation/growing!");
    }

    return std::bit_cast<void*>(handle);
}

void DynamicBuffer::gpu_free(void* virtual_adress)
{
    BufferHandle handle = std::bit_cast<BufferHandle>(virtual_adress);

    if (handle == invalid_handle) return;
    
    if (handle.index >= resolved_offsets.size()) {
        std::cout << "[OpenGL Error] DynamicBuffer::gpu_free() is called with a handle out of bounds" << std::endl;
        ASSERT(false);
        return;
    }

    if (resolved_offsets[handle.index].generation != handle.generation) {
        std::cout << "[OpenGL Error] DynamicBuffer::gpu_free() has attempted to free an expired handle." << std::endl;
        ASSERT(false);
        return;
    }

    auto it = resolved_offsets[handle.index].block_it;

    if (!it->is_free) {
        total_used_size -= it->size;
    }

    it->is_free = true;
    it->handle = invalid_handle;

    auto next_it = std::next(it);
    if (next_it != blocks.end() && next_it->is_free) {
        it->size += next_it->size;
        blocks.erase(next_it);
    }

    if (it != blocks.begin()) {
        auto prev_it = std::prev(it);
        if (prev_it->is_free) {
            prev_it->size += it->size;
            blocks.erase(it);
            it = prev_it;
        }
    }

    free_handle(handle);

    shrink_if_needed();
}

size_t DynamicBuffer::get_offset(void* virtual_adress)
{
    BufferHandle handle = std::bit_cast<BufferHandle>(virtual_adress);

    if (handle == invalid_handle ||
        handle.index >= resolved_offsets.size() ||
        handle.generation != resolved_offsets[handle.index].generation)
    {
        std::cout << "[OpenGL Error] DynamicBuffer::get_offset() is called with an invalid handle" << std::endl;
        ASSERT(false);
    }

    return resolved_offsets[handle.index].offset_in_bytes;
}

size_t DynamicBuffer::get_size(void* virtual_adress)
{
    BufferHandle handle = std::bit_cast<BufferHandle>(virtual_adress);

    if (handle == invalid_handle ||
        handle.index >= resolved_offsets.size() ||
        handle.generation != resolved_offsets[handle.index].generation)
    {
        std::cout << "[OpenGL Error] DynamicBuffer::get_size() is called with an invalid handle" << std::endl;
        ASSERT(false);
    }

    return resolved_offsets[handle.index].block_it->size;
}

Buffer& DynamicBuffer::get_buffer()
{
    return *buffer;
}

size_t DynamicBuffer::size()
{
    return total_capacity;
}

size_t DynamicBuffer::size_used()
{
    return total_used_size;;
}

size_t DynamicBuffer::size_free()
{
    return total_capacity - total_used_size;
}

size_t DynamicBuffer::get_epoch()
{
    return epoch;
}

void DynamicBuffer::defragment(size_t target_capacity)
{
    if (blocks.empty() && total_used_size == 0) return;

    size_t new_capacity     = std::max(target_capacity, total_used_size);
    auto new_buffer         = std::make_shared<Buffer>(new_capacity);

    size_t current_offset = 0;
    auto it = blocks.begin();

    while (it != blocks.end()) {
        if (it->is_free) {
            it = blocks.erase(it);
        }
        else {
            buffer->copy_to(it->offset, current_offset, it->size, *new_buffer);

            it->offset = current_offset;

            resolved_offsets[it->handle.index].offset_in_bytes = current_offset;

            current_offset += it->size;
            ++it;
        }
    }

    if (new_capacity > current_offset) {
        blocks.push_back(Block{
            .offset = current_offset,
            .size = new_capacity - current_offset,
            .handle = invalid_handle,
            .is_free = true
            });
    }

    this->buffer = new_buffer;
    this->total_capacity = new_capacity;
    this->epoch++;
}

void DynamicBuffer::grow(size_t required_additional_size)
{
    size_t new_capacity = std::max(size() * 2, size() + required_additional_size);

    defragment(new_capacity);
}

void DynamicBuffer::shrink_if_needed()
{
    if (size() > min_buffer_size && size_used() < (size() / 4)) {

        size_t new_capacity = size() / 2;

        defragment(std::max(new_capacity, min_buffer_size));
    }
}