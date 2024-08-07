#include "GraphicsCortex.h"

int main() {

	Frame frame(512, 512, "GraphicsCortexApp", 0, 0, true, true, true, Frame::NOTIFICATION, false);
	Scene scene(frame);

	std::shared_ptr<Image> image = std::make_shared<Image>("C:/Users/furkan.unsal/Desktop/Data2/projektion/Image_0002.TIFF", 1000, 1000, 1, 2, true);

	image->resize(1024, 1024);
	std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(1024, 1024, Texture2D::ColorTextureFormat::RGBA8, 1, 0, 0);
	texture->load_data(*image, Texture2D::ColorFormat::RED, Texture2D::Type::UNSIGNED_SHORT);

	std::shared_ptr<Framebuffer> framebuffer = std::make_shared<Framebuffer>();

	framebuffer->attach_color(0, texture);

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window();
		frame.display_performance(180);

		framebuffer->set_read_buffer(0);
		framebuffer->blit_to_screen(0, 0, 1024, 1024, 0, 0, 512, 512, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);
	}
}