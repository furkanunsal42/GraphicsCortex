#include <vector>
#include <string>

#include <iostream>

#include "Buffer.h";

ArrayBuffer::ArrayBuffer()
	: id(0), data_count(0) {};

ArrayBuffer::ArrayBuffer(const ArrayBuffer& a){
	id = a.id;
	data_count = a.data_count;
	vertex_attribute_structure = a.vertex_attribute_structure;
};

ArrayBuffer::ArrayBuffer(float verticies[], int data_count)
	: data_count(data_count)
{
	initialize_buffer(verticies, data_count);
}

ArrayBuffer::ArrayBuffer(std::vector<float> verticies)
	: data_count(verticies.size())
{
	initialize_buffer(verticies);
}

void ArrayBuffer::initialize_buffer(const std::vector<float>& verticies) {
	data_count = verticies.size();
	initialize_buffer((float*)&verticies[0], data_count);
}

void ArrayBuffer::initialize_buffer(float verticies[], int data_count) {
	GLCall(glGenBuffers(1, &id));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
	GLCall(glBufferData(GL_ARRAY_BUFFER, data_count * sizeof(float), verticies, GL_STATIC_DRAW));
}

void ArrayBuffer::push_attribute(unsigned int count) {
	this->vertex_attribute_structure.push_back(count);
}

void ArrayBuffer::bind() {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
		int sum = 0;
		for (int attrib : vertex_attribute_structure) {
			sum += attrib;
		}
		int prefix_sum = 0;
		for (int i = 0; i < this->vertex_attribute_structure.size(); i++) {
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, vertex_attribute_structure[i], GL_FLOAT, GL_FALSE, sum*sizeof(float), (void*)(prefix_sum*sizeof(float))));
			prefix_sum += vertex_attribute_structure[i];
		}
}

void ArrayBuffer::unbind() {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

IndexBuffer::IndexBuffer() : id(0), vertex_dim(2), data_count(0) {}

IndexBuffer::IndexBuffer(const IndexBuffer& i) {
	id = i.id;
	data_count = i.data_count;
	vertex_dim = i.vertex_dim;
}

IndexBuffer::IndexBuffer(unsigned int verticies[], int vertex_dim, int data_count)
	: vertex_dim(vertex_dim), data_count(data_count)
{
	initialize_buffer(verticies, vertex_dim, data_count);
};

IndexBuffer::IndexBuffer(std::vector<unsigned int> verticies, int vertex_dim)
	: vertex_dim(vertex_dim), data_count(verticies.size())
{
	initialize_buffer(verticies, vertex_dim);
}

void IndexBuffer::initialize_buffer(std::vector<unsigned int>& verticies, int vertex_dim) {
	data_count = verticies.size();
	initialize_buffer((unsigned int*)&verticies[0], vertex_dim, data_count);
}

void IndexBuffer::initialize_buffer(unsigned int verticies[], int vertex_dim, int data_count) {
	GLCall(glGenBuffers(1, &id));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_count * sizeof(unsigned int), verticies, GL_STATIC_DRAW));
}

void IndexBuffer::bind() {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}

void IndexBuffer::unbind() {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}