#include "CTReconstructor.h"

#include "FastFourierTransformCPU/FastFourierTransform.h"

int main() {

	WindowDescription desc;
	desc.w_resolution = glm::ivec2(2);
	Window window(desc);

	glm::ivec2 texture_resolution = glm::ivec2(600, 600);

	Image image("../CTReconstructor/Images/lenna.png", 4, true);
	Texture2D texture = Texture2D(texture_resolution.x, texture_resolution.y, Texture2D::ColorTextureFormat::R16F, 1, 0, 0);
	
	image.resize(texture_resolution.x, texture_resolution.y);
	texture.load_data(image, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE, 0);

	//GraphicsOperation op;
	//
	//op.push_constant("texture_resolution", texture_resolution);
	//op.push_constant("frequency", glm::vec2(42, 42));
	//
	//op.compute(
	//	texture,
	//	"imageStore(target_data, ivec2(id.xy), vec4(cos((id.x / float(texture_resolution.x) * frequency.x + id.y / float(texture_resolution.y) * frequency.y) * 2 * 3.14159265358979323846)));"
	//);

	FFFT fft_solver;

	std::shared_ptr<Texture2D> complex_texture = fft_solver.create_complex_texture_from_real(texture);
	std::shared_ptr<Texture2D> complex_texture2 = complex_texture->create_texture_with_same_parameters();

	//std::shared_ptr<Texture2D> padded_complex_texture = fft_solver.create_padded_complex_texture(*complex_texture);

	//auto signal_image = padded_complex_texture->get_image(Texture2D::ColorFormat::RG, Texture2D::Type::FLOAT, 0, 0, 0, padded_complex_texture->get_size().x, 1);
	//float* signal = (float*)signal_image->get_image_data();
	//std::vector<FFT::complex> signal_v;
	//for (int32_t i = 0; i < padded_complex_texture->get_size().x; i++)
	//	signal_v.push_back(FFT::complex(signal[i * 2 + 0], signal[i * 2 + 1]));
	//
	//std::ofstream f("cosf42.txt");
	//for (int32_t i = 0; i < signal_v.size(); i++)
	//	f << signal_v[i].r << std::endl;
	//f.close();

	//fft_solver.fft(*complex_texture);
	
	fft_solver.dft(*complex_texture, FFFT::XY);
	fft_solver.inverse_dft(*complex_texture, FFFT::XY);
	
	//fft_solver.shift(*complex_texture);

	//fft_solver.inverse_dft(*complex_texture);

	//op.clear();
	//op.push_constant("sample_count", padded_complex_texture->get_size().x);
	//op.compute(
	//	*padded_complex_texture,
	//	"vec4(target.xy / sample_count * 128, 0, 1)"
	//);

	//for (int32_t y = 0; y < padded_complex_texture->get_size().y; y++) {
	//
	//	auto signal_image = padded_complex_texture->get_image(Texture2D::ColorFormat::RG, Texture2D::Type::FLOAT, 0, 0, y, padded_complex_texture->get_size().x, 1);
	//	float* signal = (float*)signal_image->get_image_data();
	//	std::vector<FFT::complex> signal_v;
	//	for (int32_t i = 0; i < padded_complex_texture->get_size().x; i++)
	//		signal_v.push_back(FFT::complex(signal[i * 2 + 0], signal[i * 2 + 1]));
	//
	//	FFT::fft_radix2(signal_v);
	//
	//	for (int32_t i = 0; i < padded_complex_texture->get_size().x; i++) {
	//		signal[2*i + 0] = signal_v[i].r;
	//		signal[2*i + 1] = signal_v[i].i;
	//	}
	//
	//	padded_complex_texture->load_data(signal, Texture2D::ColorFormat::RG, Texture2D::Type::FLOAT, 0, y, padded_complex_texture->get_size().x, 1);
	//
	//	if (y == 0) {
	//		std::ofstream f("cosf42_cortex.txt");
	//		for (int32_t i = 0; i < signal_v.size(); i++)
	//			f << "(" << signal_v[i].r << ", " << signal_v[i].i << ")" << std::endl;
	//		f.close();
	//	}
	//}

	//for (int32_t y = 0; y < complex_texture->get_size().y; y++) {
	//
	//	auto signal_image = complex_texture->get_image(Texture2D::ColorFormat::RG, Texture2D::Type::FLOAT, 0, 0, y, complex_texture->get_size().x, 1);
	//	float* signal = (float*)signal_image->get_image_data();
	//	std::vector<FFT::complex> signal_v;
	//	for (int32_t i = 0; i < complex_texture->get_size().x; i++)
	//		signal_v.push_back(FFT::complex(signal[i * 2 + 0], signal[i * 2 + 1]));
	//
	//	std::vector<FFT::complex> temp;
	//	FFT::dft(signal_v, temp);
	//	FFT::inverse_dft(temp, signal_v);
	//
	//	for (int32_t i = 0; i < complex_texture->get_size().x; i++) {
	//		signal[2 * i + 0] = signal_v[i].r;
	//		signal[2 * i + 1] = signal_v[i].i;
	//	}
	//
	//	complex_texture->load_data(signal, Texture2D::ColorFormat::RG, Texture2D::Type::FLOAT, 0, y, complex_texture->get_size().x, 1);
	//
	//	if (y == 0) {
	//		std::ofstream f("cosf42_cortex.txt");
	//		for (int32_t i = 0; i < signal_v.size(); i++)
	//			f << "(" << signal_v[i].r << ", " << signal_v[i].i << ")" << std::endl;
	//		f.close();
	//	}
	//}

	//for (int32_t y = 0; y < complex_texture->get_size().y; y++) {
	//
	//	auto signal_image = texture.get_image(Texture2D::ColorFormat::RED, Texture2D::Type::FLOAT, 0, 0, y, texture.get_size().x, 1);
	//	float* signal = (float*)signal_image->get_image_data();
	//	std::vector<float> signal_v;
	//	for (int32_t i = 0; i < complex_texture->get_size().x; i++)
	//		signal_v.push_back(signal[i]);
	//
	//	std::vector<FFT::complex> temp;
	//	FFT::dft_real(signal_v, temp);
	//	FFT::inverse_dft_real(temp, signal_v);
	//
	//	texture.load_data(signal_v.data(), Texture2D::ColorFormat::RED, Texture2D::Type::FLOAT, 0, y, texture.get_size().x, 1);
	//
	//	if (y == 0) {
	//		std::ofstream f("cosf42_cortex.txt");
	//		for (int32_t i = 0; i < signal_v.size(); i++)
	//			f << "(" << temp[i].r << ", " << temp[i].i << ")" << std::endl;
	//		f.close();
	//	}
	//}

	//fft_solver.fft_shift(*padded_complex_texture);
	//
	//fft_solver.inverse_fft_shift(*padded_complex_texture);
	//fft_solver.inverse_fft(*padded_complex_texture, *padded_complex_texture);

	Framebuffer fb;
	fb.attach_color(0, *complex_texture);
	fb.activate_draw_buffer(0);

	while (!window.should_close()) {
		window.handle_events(true);
		primitive_renderer::clear(0, 0, 0, 0);

		window.set_window_resolution(complex_texture->get_size());
		fb.blit_to_screen(complex_texture->get_size(), window.get_framebuffer_resolution(), Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);

		window.swap_buffers();
	}

	return 0;
}