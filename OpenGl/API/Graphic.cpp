#include "Graphic.h"

#include "Default_Assets.h"
#include "PhysicsGeometry.h"

#include <iostream>

Graphic::Graphic() :
	model_matrix(glm::mat4(1.0f)) {}

Graphic::Graphic(Model& model, Material& mat, Program& program) : 
	model_matrix(glm::mat4(1.0f)), material(&mat), renderer(&program), model(model) {}

Graphic::Graphic(Model&& model, Material& mat, Program& program) :
	model_matrix(glm::mat4(1.0f)), material(&mat), renderer(&program), model(model) {}

Graphic::Graphic(const std::vector<float>& verticies, int data_dim = 2)
{	
	// legacy
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

	this->model.vertex_buffer = array_buffer;
	this->model.index_buffer = index_buffer;
}

Graphic::Graphic(Material& material, Program& renderer):
	renderer(&renderer), material(&material), model_matrix(glm::mat4(1.0f)) {}

void Graphic::draw(bool show_warnings, bool _ignore_default_uniforms) {
	bool material_exist = true;
	bool renderer_exist = true;
	if (material == nullptr) {
		if (show_warnings)
			std::cout << "[Opengl Warning] material is not specified for Graphic.draw()" << std::endl;
		material_exist = false;
	}
	if (renderer == nullptr) {
		if (show_warnings)
			std::cout << "[Opengl Warning] renderer is not specified for Graphic.draw()" << std::endl;
		renderer_exist = false;
	}

	if (renderer_exist)
		renderer->bind();
	model.vertex_buffer.bind();
	model.index_buffer.bind();
	
	if (material_exist)
 		material->bind();

	
	GLCall(glDrawElements(mode, model.index_buffer.data_count, GL_UNSIGNED_INT, nullptr));
}

void Graphic::load_model(const std::string& filename) {
	model.load_model(filename);
}

void Graphic::update_matrix() {
	if (_last_updated_position == position && _last_updated_rotation == rotation)
		return;
	model_matrix = glm::mat4_cast(rotation_quat);
	model_matrix = glm::translate(model_matrix, (glm::vec3)(glm::vec4(position.x, position.y, position.z, 0) * model_matrix));
	_last_updated_position = position;
	_last_updated_rotation = rotation;
}

void Graphic::clear_mesh() {
	model.vertex_buffer = ArrayBuffer();
	model.index_buffer = IndexBuffer();
}

glm::vec3 Graphic::get_position() {
	return position;
}
glm::vec3 Graphic::get_rotation() {
	return rotation;
}

void Graphic::set_position(glm::vec3 position) {
	this->position = position;
}

void Graphic::set_rotation(glm::vec3 rotation) {
	this->rotation = rotation;
}

void Graphic::update_uniform_queue(bool init) {
	_uniform_update_queue.update_uniforms();
}