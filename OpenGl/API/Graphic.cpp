#include "Graphic.h"
#include <iostream>

Graphic::Graphic() :
	model_matrix(glm::mat4(1.0f)), vertex_buffer(ArrayBuffer()), index_buffer(IndexBuffer())  {}

Graphic::Graphic(const ArrayBuffer& buffer, const IndexBuffer& indicies, Material& mat, Program& renderer) : 
	model_matrix(glm::mat4(1.0f)), vertex_buffer(buffer), index_buffer(indicies), material(&mat), renderer(&renderer) {}

Graphic::Graphic(const std::vector<float>& verticies, int data_dim = 2) {
	std::vector<unsigned int> triangles;
	for (unsigned int i = 1; i < verticies.size() / data_dim - 1; i++) {

		triangles.push_back(0);
		triangles.push_back(i);
		triangles.push_back(i + 1);
	}
	for (auto data : verticies)
		std::cout << data << " ";
	std::cout << "\n";
	for (auto data : triangles)
		std::cout << data << " ";

	// data dim is no longer passed in constructer
	ArrayBuffer array_buffer(verticies/*, data dim*/);
	IndexBuffer index_buffer(triangles, 3);

	this->vertex_buffer = array_buffer;
	this->index_buffer = index_buffer;
}
 
void Graphic::draw() {
	bool success = true;
	if (material == nullptr) {
		std::cout << "[Opengl Error] material is not specified for Graphic.draw" << std::endl;
		success = false;
	}
	if (renderer == nullptr) {
		std::cout << "[Opengl Error] renderer is not specified for Graphic.draw" << std::endl;
		success = false;
	}
	if (!success)
		return;


	renderer->bind();
	vertex_buffer.bind();
	index_buffer.bind();
	//material.bind();
	// temp
	renderer->update_uniform("color_map_slot", material->color_map_slot);
	renderer->update_uniform("specular_map_slot", material->specular_map_slot);
	renderer->update_uniform("normal_map_slot", material->normal_map_slot);
	GLCall(glDrawElements(mode, index_buffer.data_count, GL_UNSIGNED_INT, nullptr));
}

void Graphic::update_matrix() {
	model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model_matrix = glm::translate(model_matrix, (glm::vec3)(glm::vec4(position.x, position.y, position.z, 0) * model_matrix));
}