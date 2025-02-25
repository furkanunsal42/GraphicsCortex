#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class ModelLoadingTest : public TestBench {
public:

	bool run() {

		default_init();

		//Asset asset("../GraphicsCortex/Models/boxes/boxes.obj");
		//Asset asset("../GraphicsCortex/Models/medival/medival.fbx");
		//Asset asset("../GraphicsCortex/Models/sculpture/scene.gltf");
		Asset asset("../GraphicsCortex/Models/dragon.obj");
		Model model = asset.load_model();
		Mesh mesh(model);

		std::shared_ptr<Program> program_flat = default_program::flatcolor_program_s();
		program_flat->update_uniform("model", glm::mat4(1));
		program_flat->update_uniform("color", glm::vec4(1));

		Camera camera;
		camera.mouse_sensitivity = 10;
		
		//mesh.traverse([&](Mesh::Node& node, glm::mat4& transform) {
		//
		//	std::cout << (transform == glm::mat4(1));
		//
		//	});

		while (true) {
			double deltatime = default_window->handle_events(true);
			primitive_renderer::clear(0, 0, 0, 1);

			camera.handle_movements((GLFWwindow*)default_window->get_handle(), deltatime);
			camera.update_matrixes();
			camera.update_default_uniforms(*program_flat);

			mesh[0].traverse([&](Mesh::Node& node, glm::mat4& transform) {

				for (mesh_t submesh : node.get_submeshes()) {
					program_flat->update_uniform("model", transform);
					primitive_renderer::render(
						*program_flat,
						*mesh.get_mesh(submesh)
					);
				}

				});

		}

		return true;
	}
};
