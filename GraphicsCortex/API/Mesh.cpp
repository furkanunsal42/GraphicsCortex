#include "Mesh.h"

Mesh::Mesh() : 
	array_buffer(std::make_shared<ArrayBuffer>()), index_buffer(std::make_shared<IndexBuffer>()) { }

Mesh::Mesh(std::shared_ptr<ArrayBuffer> array_buffer, std::shared_ptr<IndexBuffer> index_buffer) :
	array_buffer(array_buffer), index_buffer(index_buffer) {}

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
