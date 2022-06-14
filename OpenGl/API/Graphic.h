#pragma once

#include <vector>
#include "Buffer.h"
#include "ShaderCompiler.h"
#include "Texture.h"

class Graphic {
public:
	ArrayBuffer vertex_buffer;
	IndexBuffer index_buffer;
	Texture texture;
	unsigned int mode = GL_TRIANGLES;
	Graphic();
	Graphic(ArrayBuffer buffer, IndexBuffer indicies, Texture texture);
	Graphic(std::vector<float> verticies, int data_dim);
	void draw();
	void draw(Program program);
};