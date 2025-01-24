#include "GraphicsCortex.h"
#include "GraphRenderer/GraphRenderer.h"
#include "TextureArithmatic/TextureArithmatic.h"
#include "TextureArithmatic/GraphicsOperation.h"

int main() {
	int window_width = 1024;

	Frame frame(window_width, window_width, "CTReconstructor GraphRenderer", 0, 0, true, true, false, Frame::CallbackLevel::NOTIFICATION, false);
	Scene scene(frame);
	scene.camera->fov = 90;
	scene.camera->max_distance = 1000;

	GraphicsOperation op;
	
	std::shared_ptr<Texture2D> function_texture = std::make_shared<Texture2D>(512, 512, Texture2D::ColorTextureFormat::R16F, 1, 0, 0);
	
	op.compute(
		*function_texture, 
		"pow(length( (id_n.xy - 0.5) * 10.0f), 2)");

	std::shared_ptr<Texture2D> white_texture = std::make_shared<Texture2D>(512, 512, Texture2D::ColorTextureFormat::RGBA8, 1, 0, 0);
	op.compute(
		*white_texture,
		*function_texture, false,
		"source.xxxx");

	GraphRenderer graph_renderer(glm::vec3(-5, -5, -5), glm::vec3(5, 5, 5), glm::ivec2(4096, 32));
	graph_renderer.load_data(*function_texture, glm::vec2(-5, -5), glm::vec2(5, 5));

	Framebuffer fb;
	fb.attach_color(0, white_texture);

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window();
		frame.display_performance();

		scene.camera->handle_movements(frame.window, deltatime);

		//graph_renderer.render(*scene.camera);

		fb.activate_draw_buffer(0);
		fb.blit_to_screen(0, 0, 512, 512, 0, 0, window_width, window_width, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);

	}
}