#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class SingleMeshSystemTest : public TestBench {
public:

	bool run() {

		default_init();
		
		SingleModel single_model;
		
		single_model.verticies = {
			glm::vec3(0, 0, 0),
			glm::vec3(0, 0, 1),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 1),
		};

		single_model.indicies = {
			0, 1, 2,
			1, 3, 2,
		};

		std::shared_ptr<Program> program_flat = default_program::flatcolor_program_s();

		Camera camera;

		while (true) {
			default_window->handle_events(true);
			primitive_renderer::clear(0, 0, 0, 1);
		
			camera.handle_movements((GLFWwindow*)default_window->get_handle());
			camera.update_matrixes();
			camera.update_default_uniforms(*program_flat);

			//primitive_renderer::render(program_flat, )
		}

		return true;
	}
};
