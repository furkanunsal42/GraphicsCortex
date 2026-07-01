#pragma once

#include <vector>
#include <list>
#include <memory>

#include "Buffer.h"

class DynamicBuffer {
public:

    void*           gpu_malloc(size_t size_in_bytes);
    void            gpu_free(void* virtual_adress);
    
    size_t          get_offset(void* virtual_adress);
    size_t          get_size(void* virtual_adress);
    Buffer&         get_buffer();

    size_t          size();
    size_t          size_used();
    size_t          size_free();

    size_t          get_epoch();

    static constexpr void* invalid_address = (void*)~0ull;

private:

    static constexpr size_t min_buffer_size = 1024 * 1024 * 4;

    struct BufferHandle {
        bool operator==(const BufferHandle& other) const = default;
        uint64_t index      : 50;
        uint64_t generation : 14;
    };

    static constexpr BufferHandle invalid_handle = BufferHandle {
        .index      = ~0ull,
        .generation = ~0ull
    };

    void            allocate_buffer(size_t allocation_size = min_buffer_size);

    BufferHandle    allocate_handle(size_t offset);
    void            free_handle(BufferHandle handle);
    
    void            defragment(size_t target_size);
    void            grow(size_t required_size);
    void            shrink_if_needed();

    struct Block {
        size_t          offset  = 0;
        size_t          size    = 0;
        BufferHandle    handle  = invalid_handle;
        bool            is_free = false;
    };

    struct ResolvedOffset {
        size_t      offset_in_bytes     = 0;
        uint32_t    generation          = 0;
        std::list<Block>::iterator block_it;
    };

    size_t total_used_size  = 0;
    size_t total_capacity   = 0;
    size_t epoch            = 0; // defragmentation counter

    std::list<Block>                blocks;
    std::vector<ResolvedOffset>     resolved_offsets;
    std::vector<BufferHandle>       free_handles;

    std::shared_ptr<Buffer> buffer;
};