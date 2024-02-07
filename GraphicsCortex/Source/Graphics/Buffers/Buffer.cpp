#include <vector>
#include <string>

#include <iostream>

#include "Buffer.h";

ArrayBuffer::ArrayBuffer()
	: id(0), vao(0), data_count(0) {};

ArrayBuffer::ArrayBuffer(const ArrayBuffer& a){
	vao = a.vao;
	id = a.id;
	data_count = a.data_count;
	vertex_attribute_structure = a.vertex_attribute_structure;
};

ArrayBuffer::ArrayBuffer(float verticies[], int data_count)
	: data_count(data_count)
{
	load_buffer(verticies, data_count);
}

ArrayBuffer::ArrayBuffer(std::vector<float> verticies)
	: data_count(verticies.size())
{
	load_buffer(verticies);
}

ArrayBuffer::~ArrayBuffer() {
	_deallocate();
}

void ArrayBuffer::_deallocate() {
	if (!initialized)
		return;
	GLCall(glDeleteVertexArrays(1, &vao));
	GLCall(glDeleteBuffers(1, &id));
	initialized = false;
	vertex_attribute_structure.clear();
}


void ArrayBuffer::_initialize_buffer(const std::vector<float>& verticies) {
	data_count = verticies.size();
	_initialize_buffer((float*)&verticies[0], data_count);
}

void ArrayBuffer::_initialize_buffer(float verticies[], int data_count) {
	if (initialized) {
		_deallocate();
	}
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	GLCall(glGenBuffers(1, &id));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
	GLCall(glBufferData(GL_ARRAY_BUFFER, data_count * sizeof(float), verticies, GL_STATIC_DRAW));
	initialized = true;
}

void ArrayBuffer::load_buffer(const std::vector<float>& verticies){
	data_count = verticies.size();
	load_buffer((float*)&verticies[0], data_count);
}

void ArrayBuffer::load_buffer(float verticies[], int data_count){
	if (!initialized)
		_initialize_buffer(verticies, data_count);
	else {
		vertex_attribute_structure.clear();
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
		GLCall(glBufferData(GL_ARRAY_BUFFER, data_count * sizeof(float), verticies, GL_STATIC_DRAW));
	}
}

void ArrayBuffer::push_attribute(unsigned int count) {
	if( count != 0)
		this->vertex_attribute_structure.push_back(count);
}

void ArrayBuffer::bind() {
	glBindVertexArray(vao);
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
	load_buffer(verticies, vertex_dim, data_count);
};

IndexBuffer::IndexBuffer(std::vector<unsigned int> verticies, int vertex_dim)
	: vertex_dim(vertex_dim), data_count(verticies.size())
{
	load_buffer(verticies, vertex_dim);
}

IndexBuffer::~IndexBuffer() {
	_deallocate();
}

void IndexBuffer::_deallocate() {
	if (!initialized)
		return;
	GLCall(glDeleteBuffers(1, &id));
	initialized = false;
}

void IndexBuffer::_initialize_buffer(const std::vector<unsigned int>& verticies, int vertex_dim) {
	data_count = verticies.size();
	_initialize_buffer((unsigned int*)&verticies[0], vertex_dim, data_count);
}

void IndexBuffer::_initialize_buffer(const unsigned int verticies[], int vertex_dim, int data_count) {
	if (initialized) {
		_deallocate();
	}
	GLCall(glGenBuffers(1, &id));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_count * sizeof(unsigned int), verticies, GL_STATIC_DRAW));
	initialized = true;
}

void IndexBuffer::load_buffer(const std::vector<unsigned int>& verticies, int vertex_dim){
	data_count = verticies.size();
	load_buffer((unsigned int*)&verticies[0], vertex_dim, data_count);
}
void IndexBuffer::load_buffer(const unsigned int verticies[], int vertex_dim, int data_count){
	if (!initialized)
		_initialize_buffer(verticies, vertex_dim, data_count);
	else {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_count * sizeof(unsigned int), verticies, GL_STATIC_DRAW));
		initialized = true;
	}
}

void IndexBuffer::bind() {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}

void IndexBuffer::unbind() {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

