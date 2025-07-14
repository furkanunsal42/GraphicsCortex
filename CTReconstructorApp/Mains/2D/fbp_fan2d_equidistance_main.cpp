#include "iostream"
#include "GraphicsCortex.h"

#include "FilteredBackProjection/FBP2D.h"
#include "FastFourierTransformGPU/FastFourierTransformGPU.h"

int main() {

	int screen_width = 1024;
	int screen_height = 1024;
	int projection_width = 1024;

	Frame frame(screen_width, screen_height, "CTAnalyzer", 0, 1, true, true, false, Frame::CallbackLevel::LOW, true);
	Scene scene(frame);
	scene.camera->fov = 90;
	scene.camera->max_distance = 1000;

	if (!GL_NV_shader_atomic_float) {
		std::cout << "[OpenGL Extension Error] GL_NV_shader_atomic_float is not suppoerted in this hardware, unable to initialize GraphicsCortex" << std::endl;
		ASSERT(false);
	}

	FBP2D fbp_solver;
	FFFT fft_solver;

	std::shared_ptr<Image> test_image = std::make_shared<Image>("../CTReconstructor/Images/lenna.png", 512, 512, 1, 1, 1, true);
	//std::shared_ptr<Image> test_image = std::make_shared<Image>("../CTAnalyzer/Images/middleslice.raw", 1, false);

	test_image->resize(projection_width, projection_width);
	std::shared_ptr<Texture2D> source_texture = std::make_shared<Texture2D>(*test_image, Texture2D::ColorTextureFormat::R32F, Texture2D::ColorFormat::RED, Texture2D::Type::UNSIGNED_BYTE, 1, 0, 0);

	std::shared_ptr<Texture2D> sinogram_parallel = fbp_solver.project_forward(*source_texture, projection_width, 1);
	std::shared_ptr<Texture2D> sinogram_original = fbp_solver.project_forward_equiangular(*source_texture, projection_width, 730.87f, 669.04f, 409.60f, 213.84f, 1);
	std::shared_ptr<Texture2D> sinogram_fan = sinogram_original->create_texture_with_same_parameters();
	std::shared_ptr<Texture2D> sinogram = sinogram_original->create_texture_with_same_parameters();
	fbp_solver._texture_blit_float1_to_float1(*sinogram_original, *sinogram_fan);
	fbp_solver.parallelize_fan_beam_sinogram(*sinogram_fan, *sinogram, 730.87f, 669.04f, 409.60f, 213.84f, 1);

	std::shared_ptr<Texture2D> sinogram_complex = fft_solver.create_complex_texture_from_real(*sinogram);
	fft_solver.fft_radix2(*sinogram_complex);
	fft_solver.fft_shift(*sinogram_complex);
	
	fbp_solver.apply_filter(*sinogram_complex, FBP2D::FilterType::RAM_LAK);
	
	fft_solver.inverse_fft_shift(*sinogram_complex);
	fft_solver.inverse_fft_radix2(*sinogram_complex);
	fft_solver.blit_texture_complex_to_real_r(*sinogram_complex, *sinogram);

	std::shared_ptr<Texture2D> reconstruction = fbp_solver.project_backward(*sinogram);

	std::shared_ptr<Texture2D> reconstruction_white = std::make_shared<Texture2D>(source_texture->get_size().x, source_texture->get_size().y, Texture2D::ColorTextureFormat::RGBA32F, 1, 0, 0);
	fbp_solver._texture_blit_float1_to_float4(*reconstruction, *reconstruction_white);

	std::shared_ptr<Framebuffer> framebuffer = std::make_shared<Framebuffer>();
	framebuffer->attach_color(0, reconstruction_white);

	double t = 0;

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		t += deltatime / 1000;

		frame.clear_window();
		frame.display_performance(32);

		//fbp_solver._texture_blit_float1_to_float1(*sinogram_original, *sinogram);
		//
		//fft_solver.blit_texture_real_to_complex(*sinogram, *sinogram_complex);
		//fft_solver.fft_radix2(*sinogram_complex);
		//fft_solver.fft_shift(*sinogram_complex);
		//
		//fbp_solver.apply_filter(*sinogram_complex, FBP::FilterType::RAM_LAK);
		//
		//fft_solver.inverse_fft_shift(*sinogram_complex);
		//fft_solver.inverse_fft_radix2(*sinogram_complex);
		//fft_solver.blit_texture_complex_to_real_r(*sinogram_complex, *sinogram);
		//
		//fbp_solver.project_backward(*sinogram, *reconstruction);
		//
		//fbp_solver._texture_blit_float1_to_float4(*reconstruction, *reconstruction_white);
		//
		//framebuffer->attach_color(0, reconstruction_white);

		framebuffer->set_read_buffer(0);
		framebuffer->blit_to_screen(0, 0, projection_width, projection_width, 0, 0, screen_width, screen_height, Framebuffer::Channel::COLOR, Framebuffer::Filter::NEAREST);
	}
}