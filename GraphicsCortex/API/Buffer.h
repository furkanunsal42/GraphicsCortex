#pragma once
#include "Config.h"

#include <vector>

#include "GL/glew.h"
#include "Debuger.h"

class ArrayBuffer {
public:
	unsigned int id;
	int data_count;
	std::vector<unsigned int> vertex_attribute_structure;
	ArrayBuffer();
	ArrayBuffer(const ArrayBuffer& a);
	ArrayBuffer(float verticies[], int data_count);
	ArrayBuffer(std::vector<float> verticies);
	~ArrayBuffer();
	void push_attribute(unsigned int count);
	void initialize_buffer(const std::vector<float>& verticies);
	void initialize_buffer(float verticies[], int data_count);
	void bind();
	void unbind();

private:
	bool initialized = false;
};

class IndexBuffer {
public:
	unsigned int id;
	int vertex_dim;
	int data_count;
	IndexBuffer();
	IndexBuffer(const IndexBuffer& i);
	IndexBuffer(unsigned int verticies[], int vertex_dim, int data_count);
	IndexBuffer(std::vector<unsigned int> verticies, int vertex_dim);
	~IndexBuffer();
	void initialize_buffer(const std::vector<unsigned int>& verticies, int vertex_dim);
	void initialize_buffer(const unsigned int verticies[], int vertex_dim, int data_count);
	void bind();
	void unbind();

private:
	bool initialized = false;
};
