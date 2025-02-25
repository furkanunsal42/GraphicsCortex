#pragma once
#include "Image.h"
#include "vec4.hpp"

class SingleMaterial {
public:
	std::shared_ptr<Image> albedo_image = nullptr;
	std::shared_ptr<Image> normal_image = nullptr;
	std::shared_ptr<Image> roughness_image = nullptr;
	std::shared_ptr<Image> metalness_image = nullptr;
	std::shared_ptr<Image> specular_image = nullptr;
	std::shared_ptr<Image> height_image = nullptr;
	std::shared_ptr<Image> opacity_image = nullptr;
	std::shared_ptr<Image> ambiant_occlusion_image = nullptr;
	std::shared_ptr<Image> refrection_image = nullptr;
	std::shared_ptr<Image> emissive_image = nullptr;

	glm::vec4 color = glm::vec4(1);
	float roughness = 1;
	float metalness = 0;
	glm::vec4 specular;
	glm::vec4 opacity;
	glm::vec4 refrection;
	float emissive = 0;
};