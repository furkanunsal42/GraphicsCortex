#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class ChannelSplitingTest : public TestBench {
public:

	bool run() {

		default_init();

		Image image("../GraphicsCortex/Images/orange.png", 4, true);

		Image red = image.copy_channels(Image::red);
		Image green = image.copy_channels(Image::green);
		Image blue = image.copy_channels(Image::blue);
		Image alpha = image.copy_channels(Image::alpha);

		Image redgreen = image.copy_channels(Image::red, Image::green);

		Image bgra = image.copy_channels(Image::blue, Image::green, Image::red, Image::alpha);

		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(image.get_width(), image.get_height(), Texture2D::ColorTextureFormat::RGBA8, 1, 0, 0);
		
		//texture->load_data(image, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE, 0);
		//texture->load_data(red, Texture2D::ColorFormat::RED, Texture2D::Type::UNSIGNED_BYTE, 0);
		//texture->load_data(green, Texture2D::ColorFormat::RED, Texture2D::Type::UNSIGNED_BYTE, 0);
		//texture->load_data(blue, Texture2D::ColorFormat::RED, Texture2D::Type::UNSIGNED_BYTE, 0);
		//texture->load_data(alpha, Texture2D::ColorFormat::RED, Texture2D::Type::UNSIGNED_BYTE, 0);

		//texture->load_data(redgreen, Texture2D::ColorFormat::RG, Texture2D::Type::UNSIGNED_BYTE, 0);
		texture->load_data(bgra, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE, 0);

		Framebuffer fb;
		fb.attach_color(0, texture);
		fb.activate_draw_buffer(0);

		while (true) {

			double deltatime = default_window->handle_events(true);

			fb.blit_to_screen(0, 0, image.get_width(), image.get_height(), 0, 0, 512, 512, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);

			default_window->swap_buffers();
			glFinish();
		}
	}

};