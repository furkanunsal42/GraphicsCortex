#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class ModelConstructionTest : public TestBench {
public:

	bool run() {

		default_init();

		SingleModel plane;

		plane.verticies = {
			glm::vec3(+1, +1, 0) / 2.0f,
			glm::vec3(-1, +1, 0) / 2.0f,
			glm::vec3(-1, -1, 0) / 2.0f,
			glm::vec3(-1, -1, 0) / 2.0f,
			glm::vec3(+1, -1, 0) / 2.0f,
			glm::vec3(+1, +1, 0) / 2.0f,
		};

		Model model;
		model_t m_plane = model.add_model(plane);
		
		node_t n_root = model.add_node(Model::null_node_name);
		node_t n_child = model.add_node(n_root);
		
		model[n_root].add_submodel(m_plane);
		model[n_root].set_transform(glm::translate(glm::identity<glm::mat4>(), glm::vec3(0, 0, 2)));

		model[n_child].add_submodel(m_plane);
		model[n_child].set_transform(glm::translate(glm::identity<glm::mat4>(), glm::vec3(0, 0, -2)));

		std::shared_ptr<VertexAttributeBuffer> vab = model.create_vertex_attribute_buffer();
		//std::shared_ptr<Buffer> index_buffer = model.create_index_buffer();

		std::shared_ptr<Program> program_flat = default_program::debug::flatcolor_program_s();
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
				*vab,
				PrimitiveType::triangle
			);
		}

		return true;
	}
};
