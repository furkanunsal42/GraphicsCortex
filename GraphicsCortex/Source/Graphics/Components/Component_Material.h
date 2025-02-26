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
	MaterialComponent(std::shared_ptr<Program> program, const MeshMaterial::SingleMaterial& material);

	void set_texture(const std::string& name, std::shared_ptr<Texture2D> texture);
	std::shared_ptr<Texture2D> get_texture2d(const std::string& name);
	void remove_texture(const std::string& name);
	void clear_uniforms();

	std::shared_ptr<Program> get_program();
	void set_program(std::shared_ptr<Program> program);

	void load_mesh_material(const MeshMaterial& mesh_material);
	void load_mesh_material(const MeshMaterial::SingleMaterial& mesh_material);

	void on_pre_render();

	static const std::string albedo_texture_uniform_name;
	static const std::string normal_texture_uniform_name;
	static const std::string roughness_texture_uniform_name;
	static const std::string metalness_texture_uniform_name;

private:

	std::unordered_map<std::string, std::shared_ptr<Texture2D>> textures;
	std::shared_ptr<Program> program = nullptr;
};

const std::string MaterialComponent::albedo_texture_uniform_name = "albedo_texture";
const std::string MaterialComponent::normal_texture_uniform_name = "normal_texture";
const std::string MaterialComponent::roughness_texture_uniform_name = "roughness_texture";
const std::string MaterialComponent::metalness_texture_uniform_name = "metalness_texture";
