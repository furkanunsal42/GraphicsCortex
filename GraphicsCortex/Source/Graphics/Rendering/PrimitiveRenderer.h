#pragma once

#include "RenderParameters.h"

class Framebuffer;
class Program;
class Buffer;
class AttributedVertexBuffer;
class Mesh2;

namespace primitive_renderer {

	void render(Program& program, AttributedVertexBuffer& verticies);
	void render(Program& program, AttributedVertexBuffer& verticies, Buffer& index_buffer);
	void render(Program& program, Mesh2& mesh);

}