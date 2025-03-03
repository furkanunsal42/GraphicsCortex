#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class HDRLoadingTest : public TestBench {
public:

	bool run() {

		default_window_desc = new WindowDescription();
		default_window_desc->w_resolution = glm::ivec2(1920, 1080);
		default_window_desc->f_swap_interval = 0;

		default_init();

		Image::ImageParameters params =  Image::detect_image_parameters("../GraphicsCortex/Images/HDR/sunflowers_puresky_4k.hdr");
		std::cout << "resolution : (" << params.width << ", " << params.height << ")  channel_count : " << params.channel_count << "  bytes_per_channel : " << params.bytes_per_channel << std::endl;

		Image image("../GraphicsCortex/Images/HDR/sunflowers_puresky_4k.hdr", 4, true);
		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(image.get_width(), image.get_height(), Texture2D::ColorTextureFormat::RGBA16F, 1, 0, 0);
		texture->load_data(image, Texture2D::ColorFormat::RGBA, Texture2D::Type::FLOAT, 0);

		Framebuffer fb;
		fb.attach_color(0, texture);
		fb.activate_draw_buffer(0);

		while (true) {

			double deltatime = default_window->handle_events(true);

			fb.blit_to_screen(0, 0, image.get_width(), image.get_height(), 0, 0, 1920, 1080, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);

			default_window->swap_buffers();
			glFinish();
		}
	}
};