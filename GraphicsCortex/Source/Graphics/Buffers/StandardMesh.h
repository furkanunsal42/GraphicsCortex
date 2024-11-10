#pragma once

#include "Buffer.h"
#include "AttributedVertexBuffer.h"

class Mesh2 {
public:

	
private:

	std::shared_ptr<AttributedVertexBuffer> _attributed_vertex_buffer;
	std::shared_ptr<Buffer> _index_buffer;

};