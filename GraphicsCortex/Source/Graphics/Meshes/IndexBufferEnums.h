#pragma once

#include <stdint.h>

enum IndexType {
	i_ui8,
	i_ui16,
	i_ui32,
};

enum PrimitiveType {
	point,
	line,
	line_strip,
	line_loop,
	triangle,
	triangle_strip,
	triangle_fan,

	line_strip_adjacency,
	line_adjacency,
	triangle_strip_adjacency,
	triangle_adjacency,
	patches,
};

uint32_t get_IndexType_bytes_per_index(IndexType type);
uint32_t get_PrimitiveType_index_count(PrimitiveType primitive);
uint32_t IndexType_to_GL(IndexType type);
uint32_t PrimitiveType_to_GL(PrimitiveType primitive);