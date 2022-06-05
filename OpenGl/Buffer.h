#pragma once

#include <vector>

#include "GL/glew.h"
#include "Debuger.h"

struct ArrayBuffer {
public:
	unsigned int id;
	int vertex_dim;
	int data_count;
	ArrayBuffer()
		: id(0), vertex_dim(2), data_count(0) {};

	ArrayBuffer(float verticies[], int vertex_dim, int data_count)
		: vertex_dim(vertex_dim), data_count(data_count)
	{
		initialize_buffer(verticies, vertex_dim, data_count);
	}

	ArrayBuffer(std::vector<float> verticies, int vertex_dim)
		: vertex_dim(vertex_dim), data_count(verticies.size())
	{
		initialize_buffer((float*)&verticies[0], vertex_dim, data_count);
	}
	
	void initialize_buffer(float verticies[], int vertex_dim, int data_count) {
		GLCall(glGenBuffers(1, &id));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
		GLCall(glBufferData(GL_ARRAY_BUFFER, data_count*sizeof(float), verticies, GL_STATIC_DRAW));
	}

	void bind() {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, vertex_dim, GL_FLOAT, GL_FALSE, vertex_dim * sizeof(float), 0));
	}

	void unbind() {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
};

struct IndexBuffer {
public:
	unsigned int id;
	int vertex_dim;
	int data_count;
	IndexBuffer() : id(0), vertex_dim(2), data_count(0) {}

	IndexBuffer(unsigned int verticies[], int vertex_dim, int data_count)
		: vertex_dim(vertex_dim), data_count(data_count)
	{
		initialize_buffer(verticies, vertex_dim, data_count);
	};

	IndexBuffer(std::vector<unsigned int> verticies, int vertex_dim)
		: vertex_dim(vertex_dim), data_count(verticies.size())
	{
		initialize_buffer((unsigned int*)&verticies[0], vertex_dim, data_count);
	}

	void initialize_buffer(unsigned int verticies[], int vertex_dim, int data_count) {
		GLCall(glGenBuffers(1, &id));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_count * sizeof(unsigned int), verticies, GL_STATIC_DRAW));
	}

	void bind() {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	}

	void unbind() {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
};