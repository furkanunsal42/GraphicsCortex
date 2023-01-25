#include "Mesh.h"

Mesh::Mesh() : 
	array_buffer(ArrayBuffer()), index_buffer(IndexBuffer()) { }

Mesh::Mesh(ArrayBuffer& array_buffer, IndexBuffer& index_buffer) :
	array_buffer(array_buffer), index_buffer(index_buffer)
{

}

Mesh::Mesh(ArrayBuffer&& array_buffer, IndexBuffer&& index_buffer) :
	array_buffer(array_buffer), index_buffer(index_buffer)
{

}

Mesh::Mesh(const Model& model) {
	load_model(model);
}

void Mesh::load_model(const Model& model) {
	array_buffer.initialize_buffer(model.vertex_data);

	array_buffer.vertex_attribute_structure.clear();
	for (unsigned int attribute : model.vertex_attribute_structure)
		array_buffer.push_attribute(attribute);

	index_buffer.initialize_buffer(model.index_data, 3);
}

void Mesh::bind() {
	array_buffer.bind();
	index_buffer.bind();
}
void Mesh::unbind() {
	array_buffer.unbind();
	index_buffer.unbind();
}
