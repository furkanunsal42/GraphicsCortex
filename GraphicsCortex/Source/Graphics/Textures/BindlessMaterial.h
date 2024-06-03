#pragma once

#include <unordered_map>
#include "ShaderCompiler.h"
#include "Texture1D.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Texture2DArray.h"
#include "TextureCubeMap.h"

class BindlessMaterial {
public:
	std::shared_ptr<Program> program = nullptr;

	BindlessMaterial();
	BindlessMaterial(const BindlessMaterial& other) = delete;

	BindlessMaterial(const std::shared_ptr<Program>& program);
	bool operator==(const BindlessMaterial& other);

	void set_program(const std::shared_ptr<Program>& program);
	void set_program_without_texture_reset(const std::shared_ptr<Program>& program);

	void add_texture(int index, const std::shared_ptr<Texture2D>& texture);
	void remove_texture(int index);
	
	void set_albedo(glm::vec4 color);
	void set_metalic(float value);
	void set_roughness(float value);
	void set_emmisive(glm::vec3 color);

	void add_albedo_texture(std::shared_ptr<Texture2D> texture);
	void add_normal_texture(std::shared_ptr<Texture2D> texture);
	void add_metalic_texture(std::shared_ptr<Texture2D> texture);
	void add_roughness_texture(std::shared_ptr<Texture2D> texture);
	void add_brdf_texture(std::shared_ptr<Texture2D> texture);
	void add_emmisive_texture(std::shared_ptr<Texture2D> texture);
	void add_ambient_occlusion_texture(std::shared_ptr<Texture2D> texture);

	void update_uniforms();

	// texture clusterint
	const int _cluster_buffer_texture_count = 1024*4;
	std::string _cluster_texture_buffer_name = "textures";
	std::shared_ptr<UniformBuffer> _cluster_texture_uniform_buffer = std::make_shared<UniformBuffer>();
	std::unordered_map<int, std::shared_ptr<Texture2D>> _cluster_textures;

	// pbr material
	std::string _pbr_material_buffer_name = "pbr_material";
	std::shared_ptr<UniformBuffer> _pbr_texture_uniform_buffer = std::make_shared<UniformBuffer>();
	glm::vec4 _pbr_const_albedo = glm::vec4(1, 0, 1, 1);
	glm::vec4 _pbr_const_metalic_roughness = glm::vec4(0, 0, 0, 0);
	glm::vec4 _pbr_const_emmisive = glm::vec4(0, 0, 0, 0);
	std::shared_ptr<Texture2D> _pbr_albedo				= nullptr;
	std::shared_ptr<Texture2D> _pbr_normal				= nullptr;
	std::shared_ptr<Texture2D> _pbr_metalic				= nullptr;
	std::shared_ptr<Texture2D> _pbr_roughness			= nullptr;
	std::shared_ptr<Texture2D> _pbr_brdf				= nullptr;
	std::shared_ptr<Texture2D> _pbr_emmisive			= nullptr;
	std::shared_ptr<Texture2D> _pbr_ambient_occlusion	= nullptr;
	
private:
};