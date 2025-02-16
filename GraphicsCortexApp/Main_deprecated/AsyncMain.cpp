#include "GraphicsCortex.h"
#include <chrono>

int main() {

	Frame frame(1, 1, "GrpahicsCortex", 0, 0, true, true, false, Frame::DISABLED, false);
	
	std::shared_ptr<Texture3D> texture = std::make_shared<Texture3D>(1024, 1024, 1024, Texture2D::ColorTextureFormat::RGBA8, 1, 0);
	std::shared_ptr<Texture3D> texture2 = std::make_shared<Texture3D>(1024, 1024, 1024, Texture2D::ColorTextureFormat::RGBA8, 1, 0);
	texture->force_allocation();

	auto begin = std::chrono::system_clock::now();

	bool async = true;

	if (async) 
	{
		std::shared_ptr<AsyncBuffer> readback = texture->get_image_async(Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE, 0);
		texture2->load_data(*readback, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE);

		readback->wait_to_sycronize_download();
		readback->wait_to_sycronize_upload();
	}

	if (!async)
	{
		std::shared_ptr<Image> image = texture->get_image(Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE, 0);
		texture2->load_data(*image, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE, 0);
	}
	
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - begin).count() << std::endl;

	std::shared_ptr<Framebuffer> framebuffer = std::make_shared<Framebuffer>();
	//framebuffer->attach_color(0, texture2);

	frame.resize(texture2->get_size().x, texture2->get_size().y);
	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window();
		
		//framebuffer->set_read_buffer(0);
		//framebuffer->blit_to_screen(0, 0, texture2->get_size().x, texture2->get_size().y, 0, 0, frame.window_width, frame.window_height, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);
	}
}