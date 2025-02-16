#pragma once
#include "Config.h"

#include <glm.hpp>

#include "ShaderCompiler.h"
#include "Mesh.h"
#include "BindlessMaterial.h"
#include <unordered_set>

class Graphic : public UpdatesDefaultUniforms {
public:
	unsigned mode = GL_TRIANGLES;

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	std::shared_ptr<BindlessMaterial> material = std::make_shared<BindlessMaterial>();

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	glm::quat rotation = glm::quat(1, 0, 0, 0);
	glm::mat4 model_matrix = glm::mat4(1.0f);

	Graphic();
	Graphic(const Graphic& other) = delete;
	Graphic(std::shared_ptr<Mesh> mesh);
	Graphic(std::shared_ptr<BindlessMaterial> material);
	Graphic(std::shared_ptr<Mesh> mesh, std::shared_ptr<BindlessMaterial> material);

	bool operator==(const Graphic& other);

	void set_mesh(std::shared_ptr<Mesh> mesh);
	void set_material(std::shared_ptr<BindlessMaterial> material);

	void update_matrix();
	void draw(bool show_warnings = true);
	void draw_without_bind();

	bool is_material_loaded();
	bool is_mesh_loaded();

	glm::mat4 get_object_to_world_matrix();
	glm::mat4 get_world_to_object_matrix();

	// UpdatesDefaultUniforms
	void update_default_uniforms(Program& program);

protected:
	glm::vec3 _last_updated_position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat _last_updated_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	bool _is_mesh_loaded = false;
	bool _is_material_loaded = false;
};
