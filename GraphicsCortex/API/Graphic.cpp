#include "Graphic.h"

#include "Default_Assets.h"
#include "PhysicsGeometry.h"
#include "Default_Programs.h"

#include <iostream>

Graphic::Graphic() :
	model_matrix(glm::mat4(1.0f)) {}

Graphic::Graphic(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat, std::shared_ptr<Program> program) : 
	model_matrix(glm::mat4(1.0f)), material(mat), renderer(program), mesh(mesh), use_unordered_material(false) {}

Graphic::Graphic(std::shared_ptr<Mesh> mesh, std::shared_ptr<UnorderedMaterial> mat, std::shared_ptr<Program> program) :
	model_matrix(glm::mat4(1.0f)), unordered_material(mat), renderer(program), mesh(mesh), use_unordered_material(true) {}

Graphic::Graphic(std::shared_ptr<Material> material, std::shared_ptr<Program> renderer):
	renderer(renderer), material(material), model_matrix(glm::mat4(1.0f)), use_unordered_material(false) {}

Graphic::Graphic(std::shared_ptr<UnorderedMaterial> material, std::shared_ptr<Program> renderer) :
	renderer(renderer), unordered_material(material), model_matrix(glm::mat4(1.0f)), use_unordered_material(true) {}

void Graphic::draw(bool show_warnings) {
	if (!_is_material_loaded) {
		if (show_warnings)
			std::cout << "[Opengl Warning] material is not specified for Graphic::draw()" << std::endl;
	}
	if (!_is_program_loaded) {
		if (show_warnings)
			std::cout << "[Opengl Warning] renderer is not specified for Graphic::draw()" << std::endl;
	}
	if (!_is_mesh_loaded) {
		std::cout << "[Opengl Error] Mesh is not specified for Graphic::draw()" << std::endl;
	}

	if (_is_program_loaded)
		renderer->bind();
	
	if (_is_material_loaded) {
		if (use_unordered_material) {
			unordered_material->bind();
			unordered_material->update_default_uniforms(*renderer);
		}
		else
			material->bind();
	}

	if (_is_mesh_loaded) {
		for (int i = 0; i < mesh->submeshes.size(); i++) {
			mesh->bind(i);
			GLCall(glDrawElements(mode, mesh->submeshes[i].index_buffer->data_count, GL_UNSIGNED_INT, nullptr));
		}
	}
}

void Graphic::load_model(std::shared_ptr<Mesh> mesh) {
	this->mesh = mesh;
	_is_mesh_loaded = true;
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
	mesh = nullptr;
	_is_mesh_loaded = false;
}

void Graphic::load_material(std::shared_ptr<UnorderedMaterial> material) {
	this->unordered_material = material;
	this->material = nullptr;
	use_unordered_material = true;
	_is_material_loaded = true;
}

void Graphic::load_material(std::shared_ptr<Material> material) {
	this->material = material;
	this->unordered_material = nullptr;
	use_unordered_material = false;
	_is_material_loaded = true;
}

void Graphic::load_program(std::shared_ptr<Program> program) {
	this->renderer = program;
	_is_program_loaded = true;
	_uniform_update_queue.link_program(renderer);
	_uniform_update_queue.update_uniform_ids();
}


bool Graphic::is_uniform_queue_loaded() { return _is_uniforms_loaded; }
bool Graphic::is_program_loaded() { return _is_program_loaded; }
bool Graphic::is_material_loaded() { return _is_material_loaded; }
bool Graphic::is_mesh_loaded() { return _is_mesh_loaded; }

const glm::vec3& Graphic::get_position() {
	return position;
}
const glm::quat& Graphic::get_rotation() {
	return rotation;
}

std::weak_ptr<Mesh> Graphic::get_mesh() {
	return mesh;
}

std::weak_ptr<Program> Graphic::get_program() {
	return renderer;
}

std::weak_ptr<UnorderedMaterial> Graphic::get_unordered_material() {
	return unordered_material;
}

std::weak_ptr<Material> Graphic::get_material(){
	return material;
}

bool Graphic::_is_material_ordered() {
	return use_unordered_material;
}

const glm::mat4& Graphic::get_model_matrix() {
	return model_matrix;
}

void Graphic::set_position(glm::vec3 position) {
	this->position = position;
}

void Graphic::set_rotation(glm::quat rotation) {
	this->rotation = rotation;
}

void Graphic::set_reflection_strength(float reflection){
	cubemap_reflections_strength = reflection;
}

float Graphic::get_reflection_strength(){
	return cubemap_reflections_strength;
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

void Graphic::update_default_uniforms(Program& program) {
	program.update_uniform("model", model_matrix);

	program.update_uniform("cube_map", 13);
	program.update_uniform("use_cube_map_reflection", 0);
	program.update_uniform("cube_map_reflection_strength", 0.85f);

	program.update_uniform("color", 0.8f, 0.7f, 0.6f, 1.0f);
}
