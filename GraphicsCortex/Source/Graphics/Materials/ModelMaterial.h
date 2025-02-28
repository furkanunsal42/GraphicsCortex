#pragma once

#include <span>
#include <vector>
#include <memory>
#include <optional>

#include "vec4.hpp"
#include "vec3.hpp"

#include "Image.h"

typedef uint32_t material_t;

class ModelMaterial {
public:

	class SingleMaterial {
	public:

		std::shared_ptr<Image> albedo_image = nullptr;
		std::shared_ptr<Image> normal_image = nullptr;
		std::shared_ptr<Image> roughness_image = nullptr;
		std::shared_ptr<Image> metallic_image = nullptr;
		std::shared_ptr<Image> height_image = nullptr;
		std::shared_ptr<Image> ambient_occlusion_image = nullptr;
		std::shared_ptr<Image> emissive_image = nullptr;

		glm::vec4 albedo = glm::vec4(1);
		float roughness = 1;
		float metalness = 0;
		glm::vec3 emissive = glm::vec3(0, 0, 0);
	};

	bool does_material_exist(material_t material);
	size_t get_material_count();
	material_t add_material(SingleMaterial& single_material);
	material_t add_material(SingleMaterial&& single_material);
	SingleMaterial* get_material(material_t material);
	std::span<SingleMaterial> get_materials();
	void clear();

private:
	std::vector<SingleMaterial> materials;
};
