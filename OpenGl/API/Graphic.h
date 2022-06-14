#pragma once

#include <vector>
#include "Buffer.h"
#include "ShaderCompiler.h"

class Graphic {
public:
	ArrayBuffer vertex_buffer;
	IndexBuffer index_buffer;
	unsigned int mode = GL_TRIANGLES;
	Graphic();
	Graphic(ArrayBuffer buffer, IndexBuffer indicies);
	Graphic(std::vector<float> verticies, int data_dim);
	void draw();
	void draw(Program program);
};