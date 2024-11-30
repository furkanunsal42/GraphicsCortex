#include "GraphicsCortex.h"

#include <iostream>

int main() {
	Frame frame(512, 512, "Mesh System Test", 0, 0, true, true, false, Frame::NOTIFICATION, false);
	
	std::shared_ptr<VertexAttributeBuffer> vab = std::make_shared<VertexAttributeBuffer>();
	
	std::shared_ptr<SingleModel2> single_model1 = std::make_shared<SingleModel2>();
	single_model1->unlock_verticies() = {
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, -1.0f),
	};

	std::shared_ptr<SingleModel2> single_model2 = std::make_shared<SingleModel2>();
	single_model2->unlock_verticies() = {
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(-1.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 1.0f),
	};

	std::shared_ptr<Model2> model = std::make_shared<Model2>();
	model->get_node(0).add_submodel(single_model1);
	model->get_node(0).add_submodel(single_model2);
	model->set_index_type(IndexType::i_ui32);

	std::cout << model->get_node_count() << std::endl;
	std::cout << model->get_submodel_count() << std::endl;

	std::shared_ptr<Buffer> vertex_buffer = model->create_vertex_buffer();
	std::shared_ptr<Buffer> index_buffer = model->create_index_buffer();

	vab->attach_vertex_buffer(0, vertex_buffer, VertexAttributeBuffer::a_f32, 3, sizeof(float) * 3, 0, true);

	std::shared_ptr<Program> program = std::make_shared<Program>(Shader("../GraphicsCortex/Source/GLSL/Debug/basic.vert", "../GraphicsCortex/Source/GLSL/Debug/flatcolor.frag"));
	
	program->update_uniform("model", glm::identity<glm::mat4>());
	program->update_uniform("view", glm::identity<glm::mat4>());
	program->update_uniform("projection", glm::identity<glm::mat4>());
	program->update_uniform("color", glm::vec4(1.0f));

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window();
		frame.display_performance(1000);

		primitive_renderer::render(*program, *vab, PrimitiveType::line);

	}

}