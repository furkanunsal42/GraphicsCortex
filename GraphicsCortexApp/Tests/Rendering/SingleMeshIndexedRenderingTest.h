#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class SingleMeshIndexedRenderingTest : public TestBench {
public:

	bool run() {

		default_init();

		SingleModel plane;

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

		std::shared_ptr<Buffer> vertex_buffer = plane.create_vertex_buffer();
		std::shared_ptr<Buffer> index_buffer = plane.create_index_buffer();

		std::shared_ptr<VertexAttributeBuffer> vab = std::make_shared<VertexAttributeBuffer>();
		vab->attach_vertex_buffer(0, vertex_buffer, sizeof(glm::vec3), 0, 0);
		vab->set_attribute_format(0, 0, VertexAttributeBuffer::a_f32, 3, 0);

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
				*index_buffer,
				PrimitiveType::triangle,
				IndexType::i_ui32,
				RenderParameters(),
				0,
				0,
				6
			);
		}

		return true;
	}
};
