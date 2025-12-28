#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "GUI2/GUI2.h"
#include "GUI2/GUI2Dynamic.h"

#include <chrono>

class CortexGUITest2_Relative : public TestBench {
public:

	bool run() {

		constexpr auto avail		= GUI2Dynamic::avail;
		constexpr auto fit			= GUI2Dynamic::fit;
		constexpr auto invalid_id	= GUI2Dynamic::invalid_id;
		
		GUI2 gui;
		GUI2Dynamic gui_d;

		size_t window1	= invalid_id;
		size_t box0		= invalid_id;
		size_t box1		= invalid_id;
		size_t box2		= invalid_id;
		size_t box3		= invalid_id;
		size_t box4		= invalid_id;
		
		while (true) {
			
			gui_d.new_frame(gui);

			gui_d.window_begin(window1)
			.set_is_decorated(false)
			.set_color(glm::vec4(0.8, 0.8, 0.8, 1));

				gui_d.grid_begin()
					.add_column(200)
					.add_column(200)
					.add_row(200)
					.add_row(200);
					
					gui_d.grid_region(glm::ivec2(0, 0));
					gui_d.box_begin()
						.set_target_size(glm::vec2(avail, avail))
						.set_margin(glm::vec4(10));

					gui_d.grid_region(glm::ivec2(1, 0));
					gui_d.box_begin()
						.set_target_size(glm::vec2(avail, avail))
						.set_margin(glm::vec4(10));

					gui_d.grid_region(glm::ivec2(0, 1), glm::ivec2(2, 1));
					gui_d.stack_begin()
						.set_target_size(glm::vec2(avail));

						gui_d.box_begin(box0)
							.set_target_size(glm::vec2(avail, 32));

						if (gui_d.get_resolved_properties(box0).event & GUI2::MouseEvent::LeftHold) 
						{
							
							gui_d.box_prop().color = glm::vec4(1, 0, 0, 1);

							gui_d.box_begin()
								.set_target_size(glm::vec2(avail, 32))
								.set_color(glm::vec4(0, 1, 0, 1));

						}

						gui_d.box_begin()
							.set_target_size(glm::vec2(avail, 32));
					gui_d.stack_end();
					
				gui_d.grid_end();

			gui_d.window_end();

			gui_d.publish(gui);
			gui.render();
		
		}

		return true;
	}
};