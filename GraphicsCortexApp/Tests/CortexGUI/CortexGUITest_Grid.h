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

#include "CortexGUIWidgets/Interactive/GUIWidget_Button.h"

#include <chrono>

class CortexGUITest_Grid : public TestBench {
public:

	bool run() {

		using namespace std::chrono_literals;

		default_window_desc = new WindowDescription();
		default_window_desc->w_resolution = glm::ivec2(1920, 1080);
		default_window_desc->f_multisample_count = 4;
		default_window_desc->f_swap_interval = 0;
		default_window_desc->w_scale_framebuffer_size = false;
		default_window_desc->w_scale_window_size = false;

		default_init();

		font_id font = FontBank::get().load_font("../GraphicsCortex/Fonts/Roboto-Regular.ttf", 64);

		//auto rectangle = widget::create<widget::Rectangle>();
		//rectangle->target_size = glm::vec2(100, 40);
		//rectangle->color = glm::vec4(0.5, 0.5, 0.5, 1);
		////rectangle->border_rounding = glm::vec4(20);
		//rectangle->border_thickness = glm::vec4(10);

		auto button = widget::create<widget::Button>();
		button->target_size = glm::vec2(200, 100);
		//button->padding = glm::vec4(43, 13, 43, 13);
		button->padding = glm::vec4(0);
		button->label->text = U"Kendimi Şanlı Hissediyorum";
		button->label->text_color = glm::vec4(0, 0, 0, 1);
		button->color = glm::vec4(0.88, 0.88, 0.88, 1);
		//button->color = glm::vec4(0.12, 0.12, 0.12, 1);
		button->border_thickness = glm::vec4(10, 20, 10, 10);
		button->border_color0 = glm::vec4(1, 0, 0, 1);
		button->border_color1 = glm::vec4(0, 1, 0, 1);
		button->border_color2 = glm::vec4(0, 0, 1, 1);
		button->border_color3 = glm::vec4(1, 1, 0, 1);

		button->border_rounding = glm::vec4(50);

		//auto grid = widget::create<widget::Grid>();
		//grid->color = glm::vec4(1, 0, 0, 1);
		//grid->target_size = glm::vec4(0);
		//grid->add_row(100);
		//grid->add_row(100);
		//grid->add_column(100);
		//grid->add_column(100);
		//
		//auto label = widget::create<widget::Label>();
		//label->font = font;
		//label->text = U"Portakal Ortakal!";
		//label->margin = glm::vec4(0, -1, 0, -1);
		//label->target_size = glm::vec2(0);
		//label->color = glm::vec4(0, 0, 0, 0);
		//label->text_color = glm::vec4(0, 0, 0, 1);
		//label->z = 2;
		//grid->add(label, 1, 0);
		//
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
			primitive_renderer::clear(0.94, 0.94, 0.94, 1);

			GUI::get().render(button);

			default_window->swap_buffers();
		}
	}
};