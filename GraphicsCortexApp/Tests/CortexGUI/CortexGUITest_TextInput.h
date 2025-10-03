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
#include "CortexGUIWidgets/Interactive/GUIWidget_Scroll.h"

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

		default_window->newsletters->on_should_close_events.unsubscribe_everyone();
		default_window->newsletters->on_should_close_events.subscribe([]() {
			GUI::get().release();
			exit(0);
			});

		font_id font = FontBank::get().load_font("../GraphicsCortex/Fonts/Roboto-Regular.ttf", 64);

		auto stack = widget::create<widget::Stack>();
		stack->alignment = widget::Stack::Vertical;

		auto text_input = widget::create<widget::TextInput>();
		text_input->margin = glm::vec4(10);
		stack->add(text_input);
		
		auto button = widget::create<widget::Button>();
		button->margin = glm::vec4(10);
		button->label->text = U"Clear";
		button->events.subscribe([&](GUIEvent e, glm::vec2 absolute_position, glm::vec2 cursor_position) {
			if (e == GUIEvent::Clicked)
				text_input->text = U"";
			});
		stack->add(button);

		auto scroll = widget::create<widget::Scroll>();
		scroll->margin = glm::vec4(10);
		stack->add(scroll);


		//text_input->target_size = glm::vec2(0);
		//text_input->padding = glm::vec4(43, 13, 43, 13);
		//text_input->label->text = U"Kendimi Şanslı Hissediyorum";
		//text_input->label->text_color = glm::vec4(0, 0, 0, 1);
		//text_input->color = glm::vec4(0.88, 0.88, 0.88, 1);
		//text_input->border_thickness = glm::vec4(2);
		//text_input->border_color0 = glm::vec4(0.0, 0.47, 0.84, 1);
		//text_input->border_color1 = glm::vec4(0.0, 0.47, 0.84, 1);
		//text_input->border_color2 = glm::vec4(0.0, 0.47, 0.84, 1);
		//text_input->border_color3 = glm::vec4(0.0, 0.47, 0.84, 1);
		//
		//text_input->on_hover_border_color0 = glm::vec4(0.0, 0.47, 0.84, 1);
		//text_input->on_hover_border_color1 = glm::vec4(0.0, 0.47, 0.84, 1);
		//text_input->on_hover_border_color2 = glm::vec4(0.0, 0.47, 0.84, 1);
		//text_input->on_hover_border_color3 = glm::vec4(0.0, 0.47, 0.84, 1);
		//text_input->on_hover_border_color0_transition = 250ms;
		//text_input->on_hover_border_color1_transition = 250ms;
		//text_input->on_hover_border_color2_transition = 250ms;
		//text_input->on_hover_border_color3_transition = 250ms;
		//text_input->on_hover_color = glm::vec4(0.9, 0.95, 0.98, 1);
		//text_input->on_hover_color_transition = 250ms;
		//text_input->on_hover_border_thickness = glm::vec4(1);
		//text_input->on_hover_border_thickness_transition = 250ms;
		//
		//text_input->on_hold_color = glm::vec4(0.8, 0.89, 0.97, 1);
		//text_input->on_hold_color_transition = 50ms;
		//
		//text_input->events.subscribe([](GUIEvent event) {
		//	if (event == GUIEvent::HoverBegin)
		//		std::cout << "Hover Begin" << std::endl;
		//	if (event == GUIEvent::HoverEnd)
		//		std::cout << "Hover End" << std::endl;
		//	if (event == GUIEvent::Clicked)
		//		std::cout << "Clicked" << std::endl;
		//	if (event == GUIEvent::HoldBegin)
		//		std::cout << "Hold Begin" << std::endl;
		//	if (event == GUIEvent::HoldEnd)
		//		std::cout << "Hold End" << std::endl;
		//	});

		while (true) {
			double deltatime = default_window->handle_events(true);
			primitive_renderer::clear(0.98, 0.98, 0.98, 1);
		
			GUI::get().render(stack);
			stack->poll_events(glm::vec2(0));
		
			GUI::get().end_frame();
			default_window->swap_buffers();
		}

		return true;
	}
};