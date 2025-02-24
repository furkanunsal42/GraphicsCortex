#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class MeshConstructionTest : public TestBench {
public:

	bool run() {

		default_init();

		SingleModel plane;

		//plane.verticies = {
		//	glm::vec3(+1, +1, 0) / 2.0f,
		//	glm::vec3(-1, +1, 0) / 2.0f,
		//	glm::vec3(-1, -1, 0) / 2.0f,
		//	glm::vec3(-1, -1, 0) / 2.0f,
		//	glm::vec3(+1, -1, 0) / 2.0f,
		//	glm::vec3(+1, +1, 0) / 2.0f,
		//};

		plane.verticies = {
			glm::vec3(-1, -1, 0) / 2.0f,
			glm::vec3(-1, +1, 0) / 2.0f,
			glm::vec3(+1, -1, 0) / 2.0f,
			glm::vec3(+1, +1, 0) / 2.0f,
		};
		
		plane.indicies = {
			1, 0, 2,
			2, 3, 1,
		};


		Mesh mesh;
		mesh.load_model(plane);

		std::shared_ptr<Program> program_flat = default_program::flatcolor_program_s();
		program_flat->update_uniform("model", glm::mat4(1));
		program_flat->update_uniform("color", glm::vec4(1));

		Camera camera;
		camera.mouse_sensitivity = 10;

		while (true) {
			double deltatime = default_window->handle_events(true);
			primitive_renderer::clear(0, 0, 0, 1);

			camera.handle_movements((GLFWwindow*)default_window->get_handle(), deltatime);
			camera.update_matrixes();
			camera.update_default_uniforms(*program_flat);

			primitive_renderer::render(
				*program_flat,
				*mesh.get_mesh(0)
			);
		}

		return true;
	}
};
