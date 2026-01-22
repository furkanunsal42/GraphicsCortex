#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include <filesystem>

#include "nanosvg.h"
#include "nanosvgrast.h"
#include "stb_image.h"
#include "stb_image_write.h"

class SVGRenderingTest : public TestBench {
public:

	bool run() {

		default_init();

		Package::load_package("graphics_cortex.hbv");
		Package::loaded_package->print_headers();

		Image::ImageParameters params;
		params.bytes_per_channel = 1;
		params.channel_count = 4;
		params.depth = 1;
		params.height = 0;
		params.width = 0;
		params.vertical_flip = false;
		params.path = "x.svg";
		params.source_data = (unsigned char*)Package::loaded_package->get("box.svg").c_str();
		Image image(params);

		image.save_to_disc("mysvg.png");

		while (!default_window->should_close()) {
			default_window->handle_events(true);
			primitive_renderer::clear(0, 0, 0, 1);
			default_window->swap_buffers();
		}

		return true;
	}
};
