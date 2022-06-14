#include "Graphic.h"
#include <iostream>

Graphic::Graphic() :
	model_matrix(glm::mat4(1.0f)), vertex_buffer(ArrayBuffer()), index_buffer(IndexBuffer()), texture(Texture()) {}

Graphic::Graphic(ArrayBuffer buffer, IndexBuffer indicies, Texture tex) : 
	model_matrix(glm::mat4(1.0f)), vertex_buffer(buffer), index_buffer(indicies), texture(tex) {}

Graphic::Graphic(std::vector<float> verticies, int data_dim = 2) {
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
	vertex_buffer.bind();
	index_buffer.bind();
	texture.bind();
	GLCall(glDrawElements(mode, index_buffer.data_count, GL_UNSIGNED_INT, nullptr));
}

void Graphic::draw(Program program) {
	program.bind();
	vertex_buffer.bind();
	index_buffer.bind();
	texture.bind();
	GLCall(glDrawElements(mode, index_buffer.data_count, GL_UNSIGNED_INT, nullptr));
}

void Graphic::update_matrix() {
	model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model_matrix = glm::translate(model_matrix, (glm::vec3)(glm::vec4(position.x, position.y, position.z, 0) * model_matrix));
}