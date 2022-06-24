#pragma once

#include "Graphic.h"
#include <glm.hpp>
#include <vector>


namespace default_geometry{
	Graphic rectangle(glm::vec2 scale = glm::vec2(1.0f));

	Graphic rectangle(Material& material, Program& renderer, glm::vec2 scale = glm::vec2(1.0f));

	Graphic rectangle(Material& material, glm::ivec2 texture_atlas_dim, const std::vector<unsigned int>& face_texture_locations, Program& renderer, glm::vec2 scale = glm::vec2(1.0f));

	Graphic cube(glm::vec3 scale = glm::vec3(1.0f));
	
	Graphic cube(Material& material, Program& renderer, glm::vec3 scale = glm::vec3(1.0f));

	Graphic cube(Material& material, glm::ivec2 texture_atlas_dim, const std::vector<unsigned int>& face_texture_locations, Program& renderer, glm::vec3 scale = glm::vec3(1.0f));
}

namespace default_program {
	Program solid_program();
	Program framebuffer_program();
	Program cubemap_program();
}