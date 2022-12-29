#include "Graphic.h"

#include "Default_Assets.h"
#include "PhysicsGeometry.h"

#include <iostream>

Graphic::Graphic() :
	model_matrix(glm::mat4(1.0f)) {}

Graphic::Graphic(Mesh& mesh, Material& mat, Program& program) : 
	model_matrix(glm::mat4(1.0f)), material(&mat), renderer(&program), mesh(&mesh), use_unordered_material(false) {}

Graphic::Graphic(Mesh& mesh, UnorderedMaterial& mat, Program& program) :
	model_matrix(glm::mat4(1.0f)), unordered_material(&mat), renderer(&program), mesh(&mesh), use_unordered_material(true) {}

Graphic::Graphic(Mesh&& mesh, Material& mat, Program& program) :
	model_matrix(glm::mat4(1.0f)), material(&mat), renderer(&program), mesh(&mesh), use_unordered_material(false) {}

Graphic::Graphic(Mesh&& mesh, UnorderedMaterial& mat, Program& program) :
	model_matrix(glm::mat4(1.0f)), unordered_material(&mat), renderer(&program), mesh(&mesh), use_unordered_material(true) {}

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

	this->mesh->array_buffer = array_buffer;
	this->mesh->index_buffer = index_buffer;
}

Graphic::Graphic(Material& material, Program& renderer):
	renderer(&renderer), material(&material), model_matrix(glm::mat4(1.0f)), use_unordered_material(false) {}

Graphic::Graphic(UnorderedMaterial& material, Program& renderer) :
	renderer(&renderer), unordered_material(&material), model_matrix(glm::mat4(1.0f)), use_unordered_material(true) {}

void Graphic::draw(bool show_warnings) {
	bool material_exist = true;
	bool renderer_exist = true;
	if (material == nullptr && unordered_material == nullptr) {
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
	mesh->bind();
	
	if (material_exist) {
		if (use_unordered_material)
			unordered_material->bind();
		else
			material->bind();
	}

	GLCall(glDrawElements(mode, mesh->index_buffer.data_count, GL_UNSIGNED_INT, nullptr));
}

void Graphic::load_model(Mesh& mesh) {
	this->mesh = &mesh;
}

void Graphic::load_model(Mesh&& mesh) {
	this->mesh = &mesh;
}

void Graphic::update_matrix() {
	if (_last_updated_position == position && _last_updated_rotation == rotation)
		return;
	model_matrix = glm::mat4_cast(rotation);
	model_matrix = glm::translate(model_matrix, (glm::vec3)(glm::vec4(position.x, position.y, position.z, 0) * model_matrix));
	_last_updated_position = position;
	_last_updated_rotation = rotation;
}

void Graphic::clear_mesh() {
	if (mesh != nullptr)
		delete mesh;
	mesh = new Mesh(ArrayBuffer(), IndexBuffer());
	//mesh->array_buffer = ArrayBuffer();
	//mesh->index_buffer = IndexBuffer();
}

void Graphic::load_material(UnorderedMaterial& material) {
	this->unordered_material = &material;
	this->material = nullptr;
	use_unordered_material = true;
}

void Graphic::load_material(Material& material) {
	this->material = &material;
	this->unordered_material = nullptr;
	use_unordered_material = false;
}

void Graphic::load_program(Program& program) {
	this->renderer = &program;
}

glm::vec3 Graphic::get_position() {
	return position;
}
glm::quat Graphic::get_rotation() {
	return rotation;
}

void Graphic::set_position(glm::vec3 position) {
	this->position = position;
}

void Graphic::set_rotation(glm::quat rotation) {
	this->rotation = rotation;
}

void Graphic::update_uniform_queue() {
	_uniform_update_queue.update_uniforms();
}

void Graphic::set_uniform(const std::string& uniform_name, std::function<void()> update_function) {
	add_uniform_update_queue(update_function);
}

void Graphic::remove_uniform(const std::string& uniform_name) {
	remove_uniform_update_queue(uniform_name);
}

void Graphic::remove_uniform(unsigned int uniform_id) {
	remove_uniform_update_queue(uniform_id);
}

void Graphic::set_uniform_all(uniform_update_queue& new_update_queue) {
	set_uniform_update_queue(new_update_queue);
}

void Graphic::set_uniform_all(uniform_update_queue&& new_update_queue) {
	set_uniform_update_queue(new_update_queue);
}

void Graphic::update_uniforms() {
	update_uniform_queue();
}