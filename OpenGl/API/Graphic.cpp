#include "Graphic.h"
#include "Default_Assets.h"
#include <iostream>

Graphic::Graphic() :
	model_matrix(glm::mat4(1.0f)), vertex_buffer(ArrayBuffer()), index_buffer(IndexBuffer()) {}

Graphic::Graphic(const ArrayBuffer& buffer, const IndexBuffer& indicies, Material& mat, Program& program) : 
	model_matrix(glm::mat4(1.0f)), vertex_buffer(buffer), index_buffer(indicies), material(&mat), renderer(&program) {}

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
 
void Graphic::draw(bool show_warnings, bool _ignore_default_uniforms) {
	bool material_exist = true;
	bool renderer_exist = true;
	if (material == nullptr) {
		if (show_warnings)
			std::cout << "[Opengl Warning] material is not specified for Graphic.draw" << std::endl;
		material_exist = false;
	}
	if (renderer == nullptr) {
		if (show_warnings)
			std::cout << "[Opengl Warning] renderer is not specified for Graphic.draw" << std::endl;
		renderer_exist = false;
	}

	if (renderer_exist)
		renderer->bind();
	vertex_buffer.bind();
	index_buffer.bind();
	
	if (material_exist)
		material->bind();
	
	if (!_ignore_default_uniforms){
		// reflection temp code
		renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::CUBE_MAP, 13);
		renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::USE_CUBE_MAP_REFLECTION, 0);
		// temp

		if (renderer_exist && material_exist){
			renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::COLOR_MAP_SLOT, material->color_map_slot);
			renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::SPECULAR_MAP_SLOT, material->specular_map_slot);
			renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::NORMAL_MAP_SLOT, material->normal_map_slot);
		}
	}
	GLCall(glDrawElements(mode, index_buffer.data_count, GL_UNSIGNED_INT, nullptr));
}



void Graphic::update_matrix() {
	model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model_matrix = glm::translate(model_matrix, (glm::vec3)(glm::vec4(position.x, position.y, position.z, 0) * model_matrix));
}