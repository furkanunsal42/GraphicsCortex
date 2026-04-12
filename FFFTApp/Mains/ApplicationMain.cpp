#include "GraphicsCortex.h"
#include "FFT/FFT.h"

//#include "FastFourierTransformCPU/FastFourierTransform.h"

int main() {

	WindowDescription desc;
	desc.w_resolution = glm::ivec2(2);
	desc.w_scale_framebuffer_size = false;
	desc.w_scale_window_size = false;
	Window window(desc);

	glm::ivec2 texture_resolution = glm::ivec2(600, 600);

	Image image("../CTReconstructor/Images/lenna.png", 4, true);
	std::shared_ptr<Texture2D> texture0 = std::make_shared<Texture2D>(texture_resolution.x, texture_resolution.y, Texture2D::ColorTextureFormat::RG16F, 1, 0, 0);
	image.resize(texture_resolution.x, texture_resolution.y);
	texture0->load_data(image, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE, 0);

	FFFT2 fft_solver;

	//std::shared_ptr<Texture2D> padded_texture = fft_solver.pad(*texture0, glm::ivec3(1024, 1024, 0), glm::ivec3(100, 100, 0), glm::vec2(1));
	//fft_solver.i_pad(*padded_texture, *texture0, glm::ivec3(100, 100, 0));

	std::shared_ptr<Texture2D> texture1 = texture0->create_texture_with_same_parameters();
	
	//fft_solver.i_shift(*texture1, *texture0, glm::ivec3(300, 300, 0));
	//fft_solver.split(*texture0, *texture1, glm::ivec3(2, 1, 1));
	//fft_solver.split(*texture1, *texture0, glm::ivec3(3, 1, 1), glm::ivec3(2, 1, 1));
	
	fft_solver.fft(*texture0, *texture1, FFFT2::xy, FFFT2::fft_dft);
	//fft_solver.dft(*texture0, *texture1, FFFT2::x);
	//fft_solver.dft(*texture1, *texture0, FFFT2::y);
	
	fft_solver.shift(*texture1, *texture1, glm::ivec3(texture0->get_size() / 2, 0));
	fft_solver.i_shift(*texture1, *texture1, glm::ivec3(texture0->get_size() / 2, 0));

	fft_solver.i_fft(*texture1, *texture0, FFFT2::xy, FFFT2::fft_dft);
	//fft_solver.dft(*texture0, *texture1, FFFT2::y, true);
	//fft_solver.dft(*texture1, *texture0, FFFT2::x, true);
	
	Texture2D& texture_to_render = *texture0;

	FFFT2::fft_plan plan = fft_solver.create_plan(1000, 64);
	for (int32_t i = 0; i < plan.iterations.size(); i++)
		std::cout << plan.iterations[i].radix << ",\t\t" << plan.iterations[i].chunk_size << std::endl;

	Framebuffer fb;
//fb.attach_color(0, *complex_texture);
	fb.attach_color(0, texture_to_render);
	fb.activate_draw_buffer(0);

	while (!window.should_close()) {
		window.handle_events(true);
		primitive_renderer::clear(0, 0, 0, 0);

		//window.set_window_resolution(complex_texture->get_size());
		window.set_window_resolution(texture_to_render.get_size());

		//fb.blit_to_screen(complex_texture->get_size(), window.get_framebuffer_resolution(), Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);
		fb.blit_to_screen(texture_to_render.get_size(), window.get_framebuffer_resolution(), Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);

		window.swap_buffers();
	}

	return 0;
}