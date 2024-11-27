#pragma once

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