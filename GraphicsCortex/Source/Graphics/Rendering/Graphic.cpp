#include "Graphic.h"

#include "Default_Assets.h"
#include "PhysicsGeometry.h"
#include "Default_Programs.h"

#include <iostream>

Graphic::Graphic() {}

bool Graphic::operator==(const Graphic& other)
{
	return mesh == other.mesh && material == material;
}

Graphic::Graphic(std::shared_ptr<Mesh> mesh) : mesh(mesh) {}

Graphic::Graphic(std::shared_ptr<BindlessMaterial> material) : material(material) { 
	_is_material_loaded = true;  
}

Graphic::Graphic(std::shared_ptr<Mesh> mesh, std::shared_ptr<BindlessMaterial> material) : material(material), mesh(mesh) {
	_is_material_loaded = true;
	_is_mesh_loaded = true;
}

void Graphic::draw(bool show_warnings) {
	update_matrix();

	if (!_is_material_loaded) {
		if (show_warnings)
			std::cout << "[Opengl Warning] Material is not specified before Graphic::draw()" << std::endl;
	}
	if (material->program == nullptr) {
		if (show_warnings)
			std::cout << "[Opengl Warning] Material's program is not specified before Graphic::draw()" << std::endl;
	}
	if (!_is_mesh_loaded) {
		std::cout << "[Opengl Error] Mesh is not specified before Graphic::draw()" << std::endl;
	}

	if (_is_material_loaded) {
		material->program->bind();

		material->update_uniforms();
		material->program->update_uniform_buffer_slots();
	}

	if (_is_mesh_loaded) {
		draw_without_bind();
	}
}

void Graphic::draw_without_bind()
{
	update_matrix();

	if (!_is_mesh_loaded) {
		std::cout << "[Opengl Error] Mesh is not specified before Graphic::draw_without_bind()" << std::endl;
		ASSERT(false);
	}

	for (int i = 0; i < mesh->submeshes.size(); i++) {
		mesh->bind(i);
		GLCall(glDrawElements(mode, mesh->submeshes[i].index_buffer->data_count, GL_UNSIGNED_INT, nullptr));
	}
}

void Graphic::set_mesh(std::shared_ptr<Mesh> mesh) {
	this->mesh = mesh;
	_is_mesh_loaded = true;
}

void Graphic::set_material(std::shared_ptr<BindlessMaterial> material) {
	this->material = material;
	_is_material_loaded = true;
}

void Graphic::update_matrix() {
	if (_last_updated_position == position && _last_updated_rotation == rotation)
		return;
	model_matrix = glm::mat4_cast(rotation);
	model_matrix = glm::translate(model_matrix, (glm::vec3)(glm::vec4(position.x, position.y, position.z, 0) * model_matrix));
	_last_updated_position = position;
	_last_updated_rotation = rotation;
}

bool Graphic::is_material_loaded() { return _is_material_loaded; }
bool Graphic::is_mesh_loaded() { return _is_mesh_loaded; }

glm::mat4 Graphic::get_object_to_world_matrix() {
	return model_matrix;
}

glm::mat4 Graphic::get_world_to_object_matrix() {
	return glm::inverse(model_matrix);
}

void Graphic::update_default_uniforms(Program& program) {
	program.update_uniform("model", model_matrix);

	//program.update_uniform("cube_map", 13);
	//program.update_uniform("use_cube_map_reflection", 0);
	//program.update_uniform("cube_map_reflection_strength", 0.85f);

	//program.update_uniform("color", 0.8f, 0.7f, 0.6f, 1.0f);
}
