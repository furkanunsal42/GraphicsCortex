#pragma once
#include "Config.h"

#include <glm.hpp>

#include "ShaderCompiler.h"
#include "Mesh.h"
#include "BindlessMaterial.h"

class Graphic : public UpdatesDefaultUniforms {
public:
	std::shared_ptr<unsigned int> mode = std::make_shared<unsigned int>(GL_TRIANGLES);

	Mesh mesh;
	BindlessMaterial material;

	std::shared_ptr<glm::vec3> position = std::make_shared<glm::vec3>(0.0f, 0.0f, 0.0f);
	std::shared_ptr<glm::quat> rotation = std::make_shared<glm::quat>(1, 0, 0, 0);
	std::shared_ptr<glm::mat4> model_matrix = std::make_shared<glm::mat4>(1.0f);

	Graphic();
	Graphic(const Graphic& other) = default;
	Graphic(Mesh mesh);
	Graphic(BindlessMaterial material);
	Graphic(Mesh mesh, BindlessMaterial material);

	bool operator==(const Graphic& other);

	void set_mesh(Mesh mesh);
	void set_material(BindlessMaterial material);

	void update_matrix();
	void draw(bool show_warnings = true);

	bool is_material_loaded();
	bool is_mesh_loaded();

	Mesh get_mesh();
	BindlessMaterial get_material();							

	glm::mat4 get_object_to_world_matrix();
	glm::mat4 get_world_to_object_matrix();

	// UpdatesDefaultUniforms
	void update_default_uniforms(Program& program);

protected:
	std::shared_ptr<glm::vec3> _last_updated_position = std::make_shared<glm::vec3>(0.0f, 0.0f, 0.0f);
	std::shared_ptr<glm::quat> _last_updated_rotation = std::make_shared<glm::quat>(1.0f, 0.0f, 0.0f, 0.0f);

	std::shared_ptr<bool> _is_mesh_loaded = std::make_shared<bool>(false);
	std::shared_ptr<bool> _is_material_loaded = std::make_shared<bool>(false);
};