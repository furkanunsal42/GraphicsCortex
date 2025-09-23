#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "GUI/CortexGUI.h"
#include "Tools/GraphicsOperation/GraphicsOperation.h"

#include "CortexGUIWidgets/Basic/GUIWidget_Rectangle.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Label.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Image.h"

#include "CortexGUIWidgets/Container/GUIWidget_Stack.h"

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
		
		auto stack = widget::create<widget::Stack>();
		//stack->color = glm::vec4(1, 0, 0, 1);
		stack->alignment = widget::Stack::Horizontal;
		stack->target_size = glm::vec2(0);
		stack->z = 0;

		auto stack2 = widget::create<widget::Stack>();
		stack2->alignment = widget::Stack::Vertical;
		stack2->target_size = glm::vec2(0, -1);

		auto a = widget::create<widget::Rectangle>();
		a->target_size = glm::vec2(100, 30);
		a->color = glm::vec4(1, 0, 1, 1);
		stack2->push_back(a);

		auto b = widget::create<widget::Rectangle>();
		b->target_size = glm::vec2(200, -1);
		b->color = glm::vec4(0, 1, 0, 1);
		stack2->push_back(b);
		stack->push_back(stack2);

		auto rectangle = widget::create<widget::Rectangle>();
		rectangle->target_size = glm::vec2(100, 900);
		//rectangle->margin = glm::vec4(0, -1, 0, -1);
		rectangle->color = glm::vec4(1, 0, 1, 1);
		stack->push_back(rectangle);

		auto label = widget::create<widget::Label>();
		label->font = font;
		label->text = "Portakal Ortakal!";
		stack->push_back(label);

		auto image = widget::create<widget::Image>();
		image->load_image("../GraphicsCortex/Images/orange.png");
		image->image_fit = widget::Image::Stretch;
		image->target_size = glm::vec2(500, 500);
		stack->push_back(image);

		while (true) {
			double deltatime = default_window->handle_events(true);
			primitive_renderer::clear(0.5, 0.7, 0.8, 1);
				
			GUI::get().render(stack);

			default_window->swap_buffers();
		}
	}
};