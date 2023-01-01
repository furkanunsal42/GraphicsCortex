#pragma once
#include "Config.h"

#include "Graphic.h"

namespace default_geometry{
	Graphic rectangle(glm::vec2 scale = glm::vec2(1.0f));

	Graphic rectangle(Material_s material, Program_s renderer, glm::vec2 scale = glm::vec2(1.0f));

	Graphic rectangle(Material_s material, glm::ivec2 texture_atlas_dim, const std::vector<unsigned int>& face_texture_locations, Program_s renderer, glm::vec2 scale = glm::vec2(1.0f));

	Graphic cube(glm::vec3 scale = glm::vec3(1.0f));
	
	Graphic cube(Material_s material, Program_s renderer, glm::vec3 scale = glm::vec3(1.0f));

	Graphic cube(Material_s material, glm::ivec2 texture_atlas_dim, const std::vector<unsigned int>& face_texture_locations, Program_s renderer, glm::vec3 scale = glm::vec3(1.0f));
}
