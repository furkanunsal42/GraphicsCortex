#pragma once
#include "Component/Component.h"

#include <unordered_map>
#include <string>

#include "Texture2D.h"
#include "ShaderCompiler.h"

#include "Materials/MeshMaterial.h"

class MaterialComponent : public Component {
public:

	MaterialComponent(std::shared_ptr<Program> program);
	MaterialComponent(std::shared_ptr<Program> program, const MeshMaterial& material);

	void set_texture(const std::string& name, std::shared_ptr<Texture2D> texture);
	std::shared_ptr<Texture2D> get_texture2d(const std::string& name);
	void remove_texture(const std::string& name);
	void clear_uniforms();

	std::shared_ptr<Program> get_program();
	void set_program(std::shared_ptr<Program> program);

	void load_mesh_material(const MeshMaterial& mesh_material);

	void on_pre_render();

private:

	std::unordered_map<std::string, std::shared_ptr<Texture2D>> textures;
	std::shared_ptr<Program> program = nullptr;
};