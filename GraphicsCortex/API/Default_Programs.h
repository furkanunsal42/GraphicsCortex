#pragma once

#include "Scene.h"
#include "CubeMap.h"
#include "UnifromQueue.h"

#include <glm.hpp>
#include <vector>

namespace default_program {
	uniform_update_queue solid_default_uniform_queue(Scene& scene, Graphic_s graphic);
	uniform_update_queue flat_default_uniform_queue(Scene& scene, Graphic_s graphic);
	// uniform_update_queue gui_default_uniform_queue();
	uniform_update_queue basic_uniform_queue(Scene& scene, Graphic_s graphic);
	uniform_update_queue cubemap_default_uniform_queue(CubeMapTexture& cubemap);

	uniform_update_queue ambiant_light_default_uniform_queue(AmbiantLight& ambiant_light, int light_ingdex);
	uniform_update_queue directional_light_default_uniform_queue(DirectionalLight& directional_light, int light_ingdex);
	uniform_update_queue point_light_default_uniform_queue(PointLight& point_light, int light_ingdex);
	uniform_update_queue spot_light_default_uniform_queue(SpotLight& spot_light, int light_ingdex);

	Program solid_program();
	Program flatcolor_program();
	Program framebuffer_program();
	Program cubemap_program();
	Program gui_program();
	Program_s gui_program_s();
}