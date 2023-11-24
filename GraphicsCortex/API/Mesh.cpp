#include "Mesh.h"

Mesh::Mesh() {
	submeshes.push_back(SingleMesh());
	submeshes.back().array_buffer = std::make_shared<ArrayBuffer>();
	submeshes.back().index_buffer = std::make_shared<IndexBuffer>();
}

Mesh::Mesh(std::shared_ptr<ArrayBuffer> array_buffer, std::shared_ptr<IndexBuffer> index_buffer) {
	submeshes.push_back(SingleMesh());
	submeshes.back().array_buffer = array_buffer;
	submeshes.back().index_buffer = index_buffer;
}

Mesh::Mesh(const Model& model) :
	Mesh()
{
	load_model(model);
}

void Mesh::load_model(const Model& model)
{
	submeshes.clear();
	for (const Model::SingleModel& single_model : model.submodels) {
		submeshes.push_back(SingleMesh());
		SingleMesh& submesh = submeshes.back();
		submesh.array_buffer = std::make_shared<ArrayBuffer>();
		submesh.index_buffer = std::make_shared<IndexBuffer>();

		submeshes.back().array_buffer->load_buffer(single_model.vertex_data);

		for (std::pair attribute : single_model.vertex_attribute_structure)
			submesh.array_buffer->push_attribute(attribute.second);

		submesh.index_buffer->load_buffer(single_model.index_data, 3);
	}
}

void Mesh::bind(int submesh_index) {
	submeshes[submesh_index].array_buffer->bind();
	submeshes[submesh_index].index_buffer->bind();
}
void Mesh::unbind() {
	submeshes[0].array_buffer->unbind();
	submeshes[0].index_buffer->unbind();
}
