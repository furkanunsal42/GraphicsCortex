#include "GraphicsCortex.h"
#include "Tools/GraphRenderer/GraphRenderer.h"
#include "Tools/GraphicsOperation/GraphicsOperation.h"

int main() {
	int window_width = 1024;

	Frame frame(window_width, window_width, "CTReconstructor GraphRenderer", 0, 0, true, true, false, Frame::CallbackLevel::NOTIFICATION, false);
	Scene scene(frame);
	scene.camera->fov = 90;
	scene.camera->max_distance = 1000;

	GraphicsOperation op;
	
	std::shared_ptr<Texture2D> function_texture = std::make_shared<Texture2D>(512, 512, Texture2D::ColorTextureFormat::R16F, 1, 0, 0);
	std::shared_ptr<Texture2D> white_texture = std::make_shared<Texture2D>(512, 512, Texture2D::ColorTextureFormat::RGBA8, 1, 0, 0);
	
	float range = 16;
	{
		op.push_constant("rho", 1.0f);
		op.push_constant("mean", 4.0f);
		op.push_constant("frequency", 4.0f);
		op.push_constant("range", 16.0f);

		op.push_precomputation_statement({
			"float gauss(vec2 x) { return 1/sqrt(2*3.14)/rho*exp(-1/2.0 * dot((x-vec2(mean)),(x-vec2(mean)))/rho/rho); }"
			"float gauss(float x) { return 1/sqrt(2*3.14)/rho*exp(-1/2.0 * dot((x-float(mean)),(x-float(mean)))/rho/rho); }"
			"float polynomial2(vec2 x) { return pow(length(x), 2); }"
			"float polynomial2(float x) { return pow(x, 2); }"
			"float logarithm(vec2 x) { return log(length(x)); }"
			"float logarithm(float x) { return log(x); }"
			"float sinusoidal(vec2 x) { return sin(length(frequency*x)); }"
			"float sinusoidal(float x) { return sin(frequency*x); }"
			});
		
		op.compute(
			*function_texture,
		//	"polynomial2(((id_n.xy - 0.5)*range))");
		//	"logarithm((id_n.xy - 0.5)*range)");
		//	"sinusoidal((id_n.xy - 0.5)*range)");
			"gauss( length(id_n.xy - 0.5)*range) * 4");

	
		op.compute(
			*white_texture,
			*function_texture, true,
			"source.xxxx");
	}
	
	
	GraphRenderer graph_renderer(glm::vec3(-range/2, -range/2, -range/2), glm::vec3(range/2, range/2, range/2), glm::vec3(1), glm::ivec2(8, 64));
	graph_renderer.load_data(*function_texture, glm::vec2(-range/2, -range/2), glm::vec2(range/2, range/2));
	
	
	std::shared_ptr<Renderbuffer> render_target = std::make_shared<Renderbuffer>(window_width, window_width, Renderbuffer::ColorTextureFormat::RGBA8, 8);
	Framebuffer fb;
	fb.attach_color(0, render_target);

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.display_performance();

		scene.camera->handle_movements(frame.window, deltatime);
		
		fb.activate_draw_buffer(0);
		fb.bind_draw();
		fb.clear_bound_drawbuffer(1, 1, 1, 1);
		
		graph_renderer.render(*scene.camera);
		
		fb.deactivate_all_draw_buffers();
		Framebuffer::bind_screen_draw();

		fb.blit_to_screen(0, 0, render_target->get_size().x, render_target->get_size().y, 0, 0, window_width, window_width, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);
	}
}