#include "GraphicsCortex.h"

int main() {
	
	int window_width = 1024;
	Frame frame(window_width, window_width, "CTReconstructor", 0, 0, true, true, false, Frame::CallbackLevel::DISABLED, false);
	Scene scene(frame);
	scene.camera->fov = 90;
	scene.camera->max_distance = 1000;

	size_t size = 1024 * 3 * sizeof(float);
	std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(size);
	std::shared_ptr<ComputeProgram> kernel = std::make_shared<ComputeProgram>(Shader("../GraphicsCortex/Source/GLSL/Compute/buffer_test.comp"));

	kernel->update_uniform_as_storage_buffer("my_buffer", *buffer, 0, 1024 * 3 * sizeof(float));
	kernel->dispatch_thread(1024 * 3, 1, 1);

	std::shared_ptr<VertexAttributeBuffer> vab = std::make_shared<VertexAttributeBuffer>();
	vab->attach_vertex_buffer(0, buffer, VertexAttributeBuffer::a_f32, 3, sizeof(glm::vec3), 0, true);

	std::shared_ptr<Program> program = std::make_shared<Program>(Shader("../GraphicsCortex/Source/GLSL/Debug/basic.vert", "../GraphicsCortex/Source/GLSL/Debug/flatcolor.frag"));
	program->update_uniform("model", glm::identity<glm::mat4x4>());
	program->update_uniform("color", glm::vec4(1, 1, 0, 1));

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window();
		frame.display_performance();

		scene.camera->handle_movements(frame.window, deltatime);
		scene.camera->update_matrixes();
		scene.camera->update_default_uniforms(*program);
	
		primitive_renderer::render(*program, *vab, PrimitiveType::point);
	}
}