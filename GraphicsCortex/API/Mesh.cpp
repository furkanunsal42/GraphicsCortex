#include "Mesh.h"

Mesh::Mesh() : 
	array_buffer(ArrayBuffer_s().obj), index_buffer(IndexBuffer_s().obj) { }

Mesh::Mesh(ArrayBuffer_s array_buffer, IndexBuffer_s index_buffer) :
	array_buffer(array_buffer.obj), index_buffer(index_buffer.obj) {}

Mesh::Mesh(const Model& model) :
	Mesh()
{
	load_model(model);
}

void Mesh::load_model(const Model& model)
{
	array_buffer->load_buffer(model.vertex_data);

	for (unsigned int attribute : model.vertex_attribute_structure)
		array_buffer->push_attribute(attribute);

	index_buffer->load_buffer(model.index_data, 3);
}

void Mesh::bind() {
	array_buffer->bind();
	index_buffer->bind();
}
void Mesh::unbind() {
	array_buffer->unbind();
	index_buffer->unbind();
}
