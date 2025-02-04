#include "GraphicsCortex.h"
#include "GraphRenderer/GraphRenderer.h"
#include "TextureArithmatic/TextureArithmatic.h"
#include "TextureArithmatic/GraphicsOperation.h"

int main() {
	int window_width = 1024;

	Frame frame(window_width, window_width, "CTReconstructor GraphRenderer", 0, 0, true, true, false, Frame::CallbackLevel::LOW, false);
	Scene scene(frame);
	scene.camera->fov = 90;
	scene.camera->max_distance = 1000;

	GraphicsOperation op;

	std::shared_ptr<Texture2D> function_texture = std::make_shared<Texture2D>(1024, 1024, Texture2D::ColorTextureFormat::R32F, 1, 0, 0);
	std::shared_ptr<Texture2D> white_texture = std::make_shared<Texture2D>(1024, 1024, Texture2D::ColorTextureFormat::RGBA8, 1, 0, 0);

	float range = 16;
	op.set_constant("rho", 1.0f);
	op.set_constant("mean", 4.0f);
	op.set_constant("frequency", 1.0f);
	op.set_constant("range", 16.0f);
	op.set_constant("phase", 0.0f);

	op.set_precomputation_statement({
		"float gauss(vec2 x) { return 1/sqrt(2*3.14)/rho*exp(-1/2.0 * dot((x-vec2(mean)),(x-vec2(mean)))/rho/rho); }"
		"float gauss(float x) { return 1/sqrt(2*3.14)/rho*exp(-1/2.0 * dot((x-float(mean)),(x-float(mean)))/rho/rho); }"
		"float polynomial2(vec2 x) { return pow(length(x), 2); }"
		"float polynomial2(float x) { return pow(x, 2); }"
		"float logarithm(vec2 x) { return log(length(x)); }"
		"float logarithm(float x) { return log(x); }"
		"float sinusoidal(vec2 x) { return sin(length(frequency*x - phase)); }"
		"float sinusoidal(float x) { return sin(frequency*x - phase); }"
		"float sinc(vec2 x) { return sin(length(frequency*x - phase)) / length(frequency*x - phase); }"
		"float sinc(float x) { return sin(frequency*x - phase) / (frequency*x); }"
		"float sinc2(vec2 x) { return pow(sinc(x), 2); }"
		"float sinc2(float x) { return pow(sinc(x), 2); }"
		});

	GraphRenderer graph_renderer(glm::vec3(-range / 2, -range / 2, -range / 2), glm::vec3(range / 2, range / 2, range / 2), glm::vec3(1), glm::ivec2(4, 64));


	std::shared_ptr<Renderbuffer> render_target = std::make_shared<Renderbuffer>(window_width, window_width, Renderbuffer::ColorTextureFormat::RGBA8, 8);
	Framebuffer fb;
	fb.attach_color(0, render_target);

	float phase = 0;
	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.display_performance();

		scene.camera->handle_movements(frame.window, deltatime);

		fb.activate_draw_buffer(0);
		fb.bind_draw();
		fb.clear_bound_drawbuffer(1, 1, 1, 1);

		phase += 2.0 * deltatime / 1000;
		op.set_constant("phase", phase);
		op.set_constant("mean", std::fmod(phase, 36.0f) - 18);
		op.compute(
			*function_texture,
			//	"polynomial2(((id_n.xy - 0.5)*range))");
			//	"logarithm((id_n.xy - 0.5)*range)");
				"sinusoidal(length(id_n.xy - 0.5)*range)");
			//	"sinc(length(id_n.xy - 0.5)*range)");
			//	"sinc2(length(id_n.xy - 0.5)*range)");
			//	"gauss( length(id_n.xy - 0.5)*range) * 4");

		graph_renderer.load_data(*function_texture, glm::vec2(-range / 2, -range / 2), glm::vec2(range / 2, range / 2));

		graph_renderer.render(*scene.camera);

		fb.deactivate_all_draw_buffers();
		Framebuffer::bind_screen_draw();

		fb.blit_to_screen(0, 0, render_target->get_size().x, render_target->get_size().y, 0, 0, window_width, window_width, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);
	}
}