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
#include "CortexGUIWidgets/Interactive/GUIWidget_TextInput.h"

#include <chrono>

class CortexGUITest_TextInput : public TestBench {
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

		auto stack = widget::create<widget::Stack>();
		stack->alignment = widget::Stack::Vertical;

		auto text_input = widget::create<widget::TextInput>();
		text_input->margin = glm::vec4(10);

		auto button = widget::create<widget::Button>();
		button->margin = glm::vec4(10);
		button->label->text = U"OK";

		stack->add(text_input);
		stack->add(button);

		//stext_input->target_size = glm::vec2(0);
		//stext_input->padding = glm::vec4(43, 13, 43, 13);
		//stext_input->label->text = U"Kendimi Şanslı Hissediyorum";
		//stext_input->label->text_color = glm::vec4(0, 0, 0, 1);
		//stext_input->color = glm::vec4(0.88, 0.88, 0.88, 1);
		//stext_input->border_thickness = glm::vec4(2);
		//stext_input->border_color0 = glm::vec4(0.0, 0.47, 0.84, 1);
		//stext_input->border_color1 = glm::vec4(0.0, 0.47, 0.84, 1);
		//stext_input->border_color2 = glm::vec4(0.0, 0.47, 0.84, 1);
		//stext_input->border_color3 = glm::vec4(0.0, 0.47, 0.84, 1);
		//s
		//stext_input->on_hover_border_color0 = glm::vec4(0.0, 0.47, 0.84, 1);
		//stext_input->on_hover_border_color1 = glm::vec4(0.0, 0.47, 0.84, 1);
		//stext_input->on_hover_border_color2 = glm::vec4(0.0, 0.47, 0.84, 1);
		//stext_input->on_hover_border_color3 = glm::vec4(0.0, 0.47, 0.84, 1);
		//stext_input->on_hover_border_color0_transition = 250ms;
		//stext_input->on_hover_border_color1_transition = 250ms;
		//stext_input->on_hover_border_color2_transition = 250ms;
		//stext_input->on_hover_border_color3_transition = 250ms;
		//stext_input->on_hover_color = glm::vec4(0.9, 0.95, 0.98, 1);
		//stext_input->on_hover_color_transition = 250ms;
		//stext_input->on_hover_border_thickness = glm::vec4(1);
		//stext_input->on_hover_border_thickness_transition = 250ms;
		//s
		//stext_input->on_hold_color = glm::vec4(0.8, 0.89, 0.97, 1);
		//stext_input->on_hold_color_transition = 50ms;
		//s
		//stext_input->events.subscribe([](GUIEvent event) {
		//s	if (event == GUIEvent::HoverBegin)
		//s		std::cout << "Hover Begin" << std::endl;
		//s	if (event == GUIEvent::HoverEnd)
		//s		std::cout << "Hover End" << std::endl;
		//s	if (event == GUIEvent::Clicked)
		//s		std::cout << "Clicked" << std::endl;
		//s	if (event == GUIEvent::HoldBegin)
		//s		std::cout << "Hold Begin" << std::endl;
		//s	if (event == GUIEvent::HoldEnd)
		//s		std::cout << "Hold End" << std::endl;
		//s	});

		while (true) {
			double deltatime = default_window->handle_events(true);
			primitive_renderer::clear(0.98, 0.98, 0.98, 1);

			GUI::get().render(stack);
			stack->poll_events(glm::vec2(0));

			GUI::get().end_frame();
			default_window->swap_buffers();
		}
	}
};