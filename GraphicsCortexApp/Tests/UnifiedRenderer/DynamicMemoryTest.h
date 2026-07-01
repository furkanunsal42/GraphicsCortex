#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class UnifiedRendererAABBTest : public TestBench {
public:

	
	bool run() {

		default_window_desc = new WindowDescription();
		default_window_desc->w_resolution = glm::ivec2(1920, 1080);
		default_window_desc->f_swap_interval = 0;

		default_init();

		UnifiedRenderer renderer;

		std::vector<glm::vec3> verticies{
			glm::vec3(0, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 1, 0),
			glm::vec3(1, 1, 1),

		};

		uint32_t mesh = renderer.create_mesh();
		renderer.get_mesh(mesh).allocate_vertices(verticies.size() * sizeof(glm::vec3));
		renderer.get_mesh(mesh).allocate_indices(sizeof(uint32_t) * 12);
		renderer.get_mesh(mesh).load_vertices(verticies.data(), verticies.size() * sizeof(glm::vec3));
		renderer.get_mesh(mesh).set_attribute(0, UnifiedRenderer::f32_3, 0);
		renderer.get_mesh(mesh).set_vertex_stride(sizeof(glm::vec3));

		renderer.update_structures();

		renderer.compute_mesh_aabbs();

		AABB aabb = renderer.get_mesh(mesh).get_aabb();

		std::cout << aabb.low << ", " << aabb.high << std::endl;

		std::cin.get();
	}
};