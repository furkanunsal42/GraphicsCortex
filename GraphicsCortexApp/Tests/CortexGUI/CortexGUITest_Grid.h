#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "GUI/CortexGUI.h"
#include "Tools/GraphicsOperation/GraphicsOperation.h"

#include "CortexGUIWidgets/Basic/GUIWidget_Rectangle.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Label.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Image.h"

#include "CortexGUIWidgets/Container/GUIWidget_Stack.h"
#include "CortexGUIWidgets/Container/GUIWidget_Grid.h"

class CortexGUITest_Grid : public TestBench {
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

		auto grid = widget::create<widget::Grid>();
		grid->add_row(100);
		grid->add_row(100);
		grid->add_column(100);
		grid->add_column(100);

		auto label = widget::create<widget::Label>();
		label->font = font;
		label->text = "Portakal Ortakal!";
		label->margin = glm::vec4(0, 0, 0, 0);
		label->target_size = glm::vec2(0);
		label->color = glm::vec4(0, 0, 0, 1);
		label->text_color = glm::vec4(1, 1, 1, 1);
		label->z = 2;
		grid->add(label, 0, 1);

		//auto rectangle = widget::create<widget::Rectangle>();
		//rectangle->z = 1;
		//rectangle->target_size = glm::vec2(-1);
		//rectangle->margin = glm::vec4(-1);
		//grid->add(rectangle, 1, 0);
		//
		//auto image = widget::create<widget::Image>();
		//image->load_image("../GraphicsCortex/Images/orange.png");
		//image->image_fit = widget::Image::Stretch;
		//image->target_size = glm::vec2(100, 100);
		////image->margin = glm::vec4(50, 50, 0, 0);
		//grid->add(image, 1, 0);

		while (true) {
			double deltatime = default_window->handle_events(true);
			primitive_renderer::clear(0.5, 0.7, 0.8, 1);

			GUI::get().render(grid);

			default_window->swap_buffers();
		}
	}
};