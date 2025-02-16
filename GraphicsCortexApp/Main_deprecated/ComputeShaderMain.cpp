#include "GraphicsCortex.h"

int main() {
	Frame frame(1024, 1024, "ComputeShader Test", 0, 0, true, true, true, Frame::CallbackLevel::LOW, false);
	Scene scene(frame);
	scene.camera->max_distance = 1000;
	scene.camera->fov = 90;

	std::shared_ptr<Texture2D> texture2d = std::make_shared<Texture2D>(1024, 1024, Texture2D::ColorTextureFormat::RGBA8, 1, 0, 0);
	texture2d->is_bindless = true;

	std::shared_ptr<Framebuffer> framebuffer = std::make_shared<Framebuffer>();
	framebuffer->attach_color(0, texture2d, 0);

	std::shared_ptr<ComputeProgram> compute_program = std::make_shared<ComputeProgram>(Shader("../GraphicsCortex/Source/GLSL/Compute/test.comp"));
	compute_program->update_uniform_as_image("image", *texture2d, 0);
	compute_program->dispatch(std::ceil(frame.window_width / 8.0f), std::ceil(frame.window_height / 8.0f), 1);

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window();
		frame.display_performance(180);

		framebuffer->blit_to_screen(0, 0, 1024, 1024, 0, 0, 1024, 1024, Framebuffer::Channel::COLOR, Framebuffer::Filter::NEAREST);
	}
}