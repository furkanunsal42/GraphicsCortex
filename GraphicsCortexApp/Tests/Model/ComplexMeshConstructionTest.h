#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class ComplexMeshConstructionTest : public TestBench {
public:

	bool run() {

		default_init();

		SingleModel triangle;
		triangle.verticies = {
			glm::vec3(-1, 1, +1),
			glm::vec3(-1, 1, -1),
			glm::vec3(+1, 1, +1),
		};

		SingleModel plane;
		plane.verticies = {
			glm::vec3(-1, -1, 0),
			glm::vec3(-1, +1, 0),
			glm::vec3(+1, -1, 0),
			glm::vec3(+1, +1, 0),
		};
		plane.indicies = {
			1, 0, 2,
			2, 3, 1,
		};
		
		Model model;
		model_t m_plane = model.add_model(plane);
		model_t m_triangle = model.add_model(triangle);

		model[1].add_submodel(m_plane);
		model[1].set_transform(glm::translate(glm::identity<glm::mat4>(), glm::vec3(0, 0, 1)));

		model[2].add_submodel(m_plane);
		model[2].set_transform(glm::scale(glm::translate(glm::identity<glm::mat4>(), glm::vec3(0, 0, -1)), glm::vec3(2.0f)));

		model[3].set_parent(1);
		model[3].add_submodel(m_triangle);

		Mesh mesh(model);

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
