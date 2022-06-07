#pragma once

#include <vector>
#include "Buffer.h"
#include "ShaderCompiler.h"

class Graphic {
public:
	ArrayBuffer vertex_buffer;
	IndexBuffer index_buffer;
	unsigned int mode = GL_TRIANGLES;
	Graphic() {
		vertex_buffer = ArrayBuffer();
		index_buffer = IndexBuffer();
	}
	Graphic(ArrayBuffer buffer, IndexBuffer indicies) {
		vertex_buffer = buffer;
		index_buffer = indicies;
	}
	void draw() {
		vertex_buffer.bind();
		index_buffer.bind();
		GLCall(glDrawElements(mode, index_buffer.data_count, GL_UNSIGNED_INT, nullptr));
	}
	void draw(Program program) {
		program.bind();
		vertex_buffer.bind();
		index_buffer.bind();
		GLCall(glDrawElements(mode, index_buffer.data_count, GL_UNSIGNED_INT, nullptr));
	}
};