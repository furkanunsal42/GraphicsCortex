#include "GraphicsCortex.h"

int main() {

	glm::ivec2 window_resolution(1024, 1024);
	Frame frame(window_resolution.x, window_resolution.y, "GraphicsCortexComputeBuffers", 0, 0, true, true, false, Frame::NOTIFICATION, false);
	Scene scene(frame);

	glm::ivec2 resolution(4096, 64);
	glm::vec2 range_begin(-5, -5);
	glm::vec2 range_end(5, 5);
	std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(resolution.x * resolution.y * sizeof(glm::vec4));
	std::shared_ptr<ComputeProgram> kernel = std::make_shared<ComputeProgram>(Shader("../GraphicsCortex/Source/GLSL/Compute/buffer_point_cloud_test.comp"));

	kernel->update_uniform_as_storage_buffer("point_cloud_buffer", *buffer);
	kernel->update_uniform("point_cloud_resolution", resolution);
	kernel->update_uniform("range_begin",	glm::vec2(-5, -5));
	kernel->update_uniform("range_end",		glm::vec2(+5, +5));
	kernel->update_uniform("amplitude", 1.0f);
	kernel->dispatch_thread(resolution.x * resolution.y, 1, 1);

	std::vector<float> data = buffer->get_data<float>(0, 1024);
	for (int i = 0; i < data.size(); i++)
		std::cout << data[i];
	std::cout << std::endl;

	std::shared_ptr<Program> point_cloud_program = std::make_shared<Program>(Shader("../GraphicsCortex/Source/GLSL/Debug/basic.vert", "../GraphicsCortex/Source/GLSL/Debug/flatcolor.frag"));
	point_cloud_program->update_uniform("color", glm::vec4(1, 1, 1, 1));
	point_cloud_program->update_uniform("model", glm::scale(glm::identity<glm::mat4x4>(), glm::vec3(4, 1.0f / 10, 4)));

	std::shared_ptr<VertexAttributeBuffer> vab = std::make_shared<VertexAttributeBuffer>();
	vab->attach_vertex_buffer(0, buffer, VertexAttributeBuffer::a_f32, 3, sizeof(glm::vec4), 0, true);

	float angle = 3.14 / 4;

	while (frame.is_running()) {

		double deltatime = frame.handle_window();
		frame.clear_window(0, 0, 0, 1);
		frame.display_performance();

		scene.camera->handle_movements(frame.window, deltatime);
		scene.camera->update_matrixes();
		scene.camera->update_default_uniforms(*point_cloud_program);

		angle += deltatime / 1000;
		glm::mat4x4 model_matrix = glm::scale(glm::rotate(glm::identity<glm::mat4x4>(), angle, glm::vec3(0, 1, 0)), glm::vec3(1.0f / resolution.x, 1.0f / 1, 1.0f / resolution.y));
		point_cloud_program->update_uniform("model", model_matrix);
		
		primitive_renderer::render(*point_cloud_program, *vab, PrimitiveType::point);
	
		model_matrix = glm::scale(glm::rotate(glm::identity<glm::mat4x4>(), angle + 3.14f/2, glm::vec3(0, 1, 0)), glm::vec3(1.0f / resolution.x, 1.0f / 1, 1.0f / resolution.y));
		point_cloud_program->update_uniform("model", model_matrix);

		primitive_renderer::render(*point_cloud_program, *vab, PrimitiveType::point);

	}
}