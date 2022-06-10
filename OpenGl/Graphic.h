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
	Graphic(std::vector<float> verticies, int data_dim = 2) {
		std::vector<unsigned int> triangles;
		for (int i = 1; i < verticies.size()/data_dim - 1; i++) {
			
			triangles.push_back(0);
			triangles.push_back(i);
			triangles.push_back(i+1);
		}
		for (auto data : verticies)
			std::cout << data << " ";
		std::cout << "\n";
		for (auto data : triangles)
			std::cout << data << " ";

		ArrayBuffer array_buffer(verticies, data_dim);
		IndexBuffer index_buffer(triangles, 3);

		this->vertex_buffer = array_buffer;
		this->index_buffer = index_buffer;
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