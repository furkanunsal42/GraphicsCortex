#pragma once

#include "Buffer.h"
#include "AttributedVertexBuffer.h"



class Mesh2 {
public:

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

	enum IndexType {
		ui8,
		ui16,
		ui32,
	};

	Mesh2(const Mesh2& other) = delete;
	Mesh2();
	~Mesh2();

	

private:
	
	struct _single_mesh_description {
	public:

		size_t _offset;
		size_t _size;

		Mesh2::PrimitiveType _primitive;
	};

	std::vector<_single_mesh_description> _single_mesh_descriptions;

	std::shared_ptr<AttributedVertexBuffer> _attributed_vertex_buffer;
	std::shared_ptr<Buffer> _index_buffer;


};