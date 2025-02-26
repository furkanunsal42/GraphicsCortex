#pragma once

#include <span>
#include <vector>
#include <memory>
#include <optional>

#include "vec4.hpp"

class Image;

typedef uint32_t material_t;

class ModelMaterial {
public:

	class SingleMaterial {
	public:

		std::optional<Image> albedo_image = std::nullopt;
		std::optional<Image> normal_image = std::nullopt;
		std::optional<Image> roughness_image = std::nullopt;
		std::optional<Image> metalness_image = std::nullopt;
		std::optional<Image> specular_image = std::nullopt;
		std::optional<Image> height_image = std::nullopt;
		std::optional<Image> opacity_image = std::nullopt;
		std::optional<Image> ambiant_occlusion_image = std::nullopt;
		std::optional<Image> refrection_image = std::nullopt;
		std::optional<Image> emissive_image = std::nullopt;

		glm::vec4 albedo = glm::vec4(1);
		float roughness = 1;
		float metalness = 0;
		glm::vec4 specular;
		glm::vec4 opacity;
		glm::vec4 refrection;
		float emissive = 0;
	};

	bool does_material_exist(material_t material);
	size_t get_material_count();
	material_t add_material(SingleMaterial single_material);
	material_t add_material(SingleMaterial&& single_material);
	SingleMaterial* get_material(material_t material);
	std::span<SingleMaterial> get_materials();
	void clear();

private:
	std::vector<SingleMaterial> materials;
};
