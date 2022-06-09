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
		// calculate the mid point
		float x_mid = 0, y_mid = 0, z_mid = 0;
		for (int i = 0; i < verticies.size(); i+=data_dim) {
			x_mid += verticies[i];
			y_mid += verticies[i + 1];
			if (data_dim == 3)
				z_mid += verticies[i + 2];
		}
		x_mid /= verticies.size();
		y_mid /= verticies.size();
		if (data_dim == 3)
			z_mid /= verticies.size();

		// create new buffer array 
		std::vector<float> buffer_verticies = { x_mid, y_mid };
		if (data_dim == 3)
			buffer_verticies.push_back(z_mid);
		for (float vertex : verticies)
			buffer_verticies.push_back(vertex);

		// create new index array
		std::vector<unsigned int> triangles;
		for (int i = 1; i < verticies.size()/data_dim + 1; i++) {
			triangles.push_back(0);
			triangles.push_back(i);
			if(i < verticies.size() / data_dim)
				triangles.push_back(i+1);
			else
				triangles.push_back(1);

		}
		for (auto data : buffer_verticies)
			std::cout << data << " ";
		std::cout << "\n";
		for (auto data : triangles)
			std::cout << data << " ";


		ArrayBuffer array_buffer(buffer_verticies, data_dim);
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