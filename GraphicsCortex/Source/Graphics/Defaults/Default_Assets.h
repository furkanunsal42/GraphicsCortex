#pragma once
#include <memory>

#include "Config.h"
#include "Font.h"
#include "Graphic.h"

namespace default_geometry{
	Graphic rectangle(glm::vec2 scale = glm::vec2(1.0f));

	Graphic rectangle(BindlessMaterial material, glm::vec2 scale = glm::vec2(1.0f));

	Graphic rectangle(BindlessMaterial material, glm::ivec2 texture_atlas_dim, const std::vector<unsigned int>& face_texture_locations, glm::vec2 scale = glm::vec2(1.0f));

	Graphic cube(glm::vec3 scale = glm::vec3(1.0f));
	
	Graphic cube(BindlessMaterial material, glm::vec3 scale = glm::vec3(1.0f));

	Graphic cube(BindlessMaterial material, glm::ivec2 texture_atlas_dim, const std::vector<unsigned int>& face_texture_locations, glm::vec3 scale = glm::vec3(1.0f));
}
