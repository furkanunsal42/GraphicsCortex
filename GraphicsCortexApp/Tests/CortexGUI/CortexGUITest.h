#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "GUI/CortexGUI.h"
#include "Tools/GraphicsOperation/GraphicsOperation.h"

#include "CortexGUIControls/Basic/GUIWidget_Rectangle.h"
#include "CortexGUIControls/Basic/GUIWidget_Label.h"
#include "CortexGUIControls/Basic/GUIWidget_Image.h"

class CortexGUITest: public TestBench {
public:

	bool run() {

		default_window_desc = new WindowDescription();
		default_window_desc->w_resolution = glm::ivec2(1920, 1080);
		default_window_desc->f_multisample_count = 4;
		default_window_desc->f_swap_interval = 0;
		default_window_desc->w_scale_framebuffer_size = false;
		default_window_desc->w_scale_window_size = false;

		default_init();

		font_id font = FontBank::get().load_font("../GraphicsCortex/Fonts/Roboto-Regular.ttf", 64);
		
		widget::Label label;
		label.font = font;
		label.text = "Portakal Ortakal!";
		label.color = glm::vec4(0, 0, 1, 0);
		label.text_height = 16;
		label.target_size = glm::vec2(500, 50);
		label.z = 0;

		widget::Image image;
		image.load_image("../GraphicsCortex/Images/orange.png");
		image.image_fit = widget::Image::Contain;
		image.target_size = glm::vec2(500, 500);
		image.z = 0;
		 
		while (true) {
			double deltatime = default_window->handle_events(true);
			primitive_renderer::clear(0.5, 0.7, 0.8, 1);

			GUI::get().render(image.get_element());
			GUI::get().render(label.get_element());

			default_window->swap_buffers();
		}
	}
};