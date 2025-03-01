#pragma once

#include <vector>
#include <memory>

#include <vec4.hpp>

#include "ModelMaterial.h"

class Texture2D;
class Program;

class MeshMaterial {
public:

	class SingleMaterial {
	public:
		std::shared_ptr<Texture2D> albedo_texture = nullptr;
		std::shared_ptr<Texture2D> normal_texture = nullptr;
		std::shared_ptr<Texture2D> roughness_texture = nullptr;
		std::shared_ptr<Texture2D> metallic_texture = nullptr;
		std::shared_ptr<Texture2D> specular_texture = nullptr;
		std::shared_ptr<Texture2D> height_texture = nullptr;
		std::shared_ptr<Texture2D> opacity_texture = nullptr;
		std::shared_ptr<Texture2D> ambient_occlusion_texture = nullptr;
		std::shared_ptr<Texture2D> refrection_texture = nullptr;
		std::shared_ptr<Texture2D> emissive_texture = nullptr;
	};

	void load_material(ModelMaterial& model_material);
	void load_material(ModelMaterial&& model_material);
	void load_material(ModelMaterial::SingleMaterial& single_model_material);
	void load_material(ModelMaterial::SingleMaterial&& single_model_material);

	bool does_material_exist(material_t material);
	size_t get_material_count();
	material_t add_material(MeshMaterial::SingleMaterial& single_material);
	material_t add_material(MeshMaterial::SingleMaterial&& single_material);
	MeshMaterial::SingleMaterial* get_material(material_t material);
	std::span<MeshMaterial::SingleMaterial> get_materials();
	void clear();

	void update_uniforms(Program& program, material_t material_index);


	static const std::string albedo_texture_uniform_name;
	static const std::string normal_texture_uniform_name;
	static const std::string roughness_texture_uniform_name;
	static const std::string metallic_texture_uniform_name;
	static const std::string ambient_occlusion_texture_uniform_name;

private:
	std::vector<SingleMaterial> materials;
};