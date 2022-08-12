#pragma once
#include "Config.h"

#include "Graphic.h"
#include "Scene.h"
#include "UnifromQueue.h"

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
	uniform_update_queue solid_default_uniform_queue(Scene& scene, Graphic& mesh);
	uniform_update_queue flat_default_uniform_queue(Scene& scene, Graphic& mesh);

	uniform_update_queue ambiant_light_default_uniform_queue(AmbiantLight& ambiant_light, int light_ingdex);
	uniform_update_queue directional_light_default_uniform_queue(DirectionalLight& directional_light, int light_ingdex);
	uniform_update_queue point_light_default_uniform_queue(PointLight& point_light, int light_ingdex);
	uniform_update_queue spot_light_default_uniform_queue(SpotLight& spot_light, int light_ingdex);

	Program solid_program();
	Program flatcolor_program();
	Program framebuffer_program();
	Program cubemap_program();
}