#pragma once
#include <memory>

#include "Config.h"
#include "Font.h"
#include "Graphic.h"

namespace default_geometry{
	Graphic rectangle(glm::vec2 scale = glm::vec2(1.0f));

	Graphic rectangle(std::shared_ptr<Material> material, std::shared_ptr<Program> renderer, glm::vec2 scale = glm::vec2(1.0f));

	Graphic rectangle(std::shared_ptr<Material> material, glm::ivec2 texture_atlas_dim, const std::vector<unsigned int>& face_texture_locations, std::shared_ptr<Program> renderer, glm::vec2 scale = glm::vec2(1.0f));

	Graphic cube(glm::vec3 scale = glm::vec3(1.0f));
	
	Graphic cube(std::shared_ptr<Material> material, std::shared_ptr<Program>  renderer, glm::vec3 scale = glm::vec3(1.0f));

	Graphic cube(std::shared_ptr<Material> material, glm::ivec2 texture_atlas_dim, const std::vector<unsigned int>& face_texture_locations, std::shared_ptr<Program> renderer, glm::vec3 scale = glm::vec3(1.0f));
}
