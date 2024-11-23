#include "GraphicsCortex.h"

#include <iostream>

int main() {
	Frame frame(512, 512, "Mesh System Test", 0, 0, true, true, false, Frame::NOTIFICATION, false);
	
	std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(sizeof(float) * 3 * 6);
	buffer->set_data<float>({
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			0.0f, 1.0f, -1.0f,
		
			1.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,
			0.0f, -1.0f, 1.0f,

		});

	std::shared_ptr<Mesh2> mesh = std::make_shared<Mesh2>();
	mesh->attach_vertex_buffer(0, buffer, Mesh2::a_f32, 3, sizeof(float) * 3, 0, true);

	std::shared_ptr<Program> program = std::make_shared<Program>(Shader("../GraphicsCortex/Source/GLSL/Debug/basic.vert", "../GraphicsCortex/Source/GLSL/Debug/flatcolor.frag"));
	
	program->update_uniform("model", glm::identity<glm::mat4>());
	program->update_uniform("view", glm::identity<glm::mat4>());
	program->update_uniform("projection", glm::identity<glm::mat4>());
	program->update_uniform("color", glm::vec4(1.0f));

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window();
		frame.display_performance(1000);

		primitive_renderer::render(*program, *mesh);

	}

}