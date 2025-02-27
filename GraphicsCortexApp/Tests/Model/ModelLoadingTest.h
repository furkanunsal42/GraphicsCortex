#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class ModelLoadingTest : public TestBench {
public:

	bool run() {

		default_init();

		//Asset asset("../GraphicsCortex/Models/boxes/boxes.obj");
		//Asset asset("../GraphicsCortex/Models/medival/medival.fbx");
		Asset asset("../GraphicsCortex/Models/sculpture/scene.gltf");
		//Asset asset("../GraphicsCortex/Models/dragon.obj");
		//Asset asset("../GraphicsCortex/Models/Thinker/Rodin_Thinker.obj");
		//Asset asset("../GraphicsCortex/Models/porsche.obj");
		//Asset asset("../GraphicsCortex/Models/teducar/teduCar.fbx");

		Model model = asset.load_model();
		Mesh mesh(model);

		std::shared_ptr<Program> program_flat = default_program::debug::flatcolor_program_s();
		std::shared_ptr<Program> program_uv = std::make_shared<Program>(
			Shader(
				"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
				"../GraphicsCortex/Source/GLSL/Debug/texcoord.frag")
		);

		program_uv->update_uniform("model", glm::mat4(1));
		program_uv->update_uniform("color", glm::vec4(1));

		Camera camera;
		camera.mouse_sensitivity = 10;
		
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		while (true) {
			double deltatime = default_window->handle_events(true);
			primitive_renderer::clear(0, 0, 0, 1);

			camera.handle_movements((GLFWwindow*)default_window->get_handle(), deltatime);
			camera.update_matrixes();
			camera.update_default_uniforms(*program_uv);
			
			//primitive_renderer::render(
			//	*program_uv,
			//	*mesh.get_mesh(0)
			//);

			mesh[0].traverse([&](Mesh::Node& node, glm::mat4& transform) {
			
				for (mesh_t submesh : node.get_submeshes()) {
					program_uv->update_uniform("model", transform);
					primitive_renderer::render(
						*program_uv,
						*mesh.get_mesh(submesh)
					);
				}
			
				});

		}

		return true;
	}
};
