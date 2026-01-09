#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "GUI/GUI.h"
#include "GUI/GUIDynamic.h"

#include <chrono>

class CortexGUITest_Hierarchy : public TestBench {
public:

	bool run() {

		constexpr auto avail		= GUIDynamic::avail;
		constexpr auto fit			= GUIDynamic::fit;
		constexpr auto invalid_id	= GUIDynamic::invalid_id;
		
		GUI gui;
		GUIDynamic gui_d;

		size_t window1	= invalid_id;
		size_t box0		= invalid_id;
		size_t box1		= invalid_id;
		size_t box2		= invalid_id;
		size_t box3		= invalid_id;
		size_t box4		= invalid_id;
		
		while (true) 
		{
			
			gui_d.new_frame(gui);

			gui_d.window_begin(window1)
			.set_is_decorated(false)
			.set_color(glm::vec4(0.8, 0.8, 0.8, 1));

				gui_d.grid_begin()
					.add_column(200)
					.add_column(200)
					.add_row(fit);
					
					gui_d.grid_region(glm::ivec2(1, 0));
					
					gui_d.grid_begin()
						//.set_target_size(glm::vec2(avail))
						.add_column(avail)
						.add_row(100)
						.add_row(fit);
					
					gui_d.grid_region(glm::ivec2(0, 1));

						gui_d.box_begin(box0)
							.set_target_size(glm::vec2(100, 100))
							.set_color(glm::vec4(1, 1, 0, 1));

					gui_d.grid_end();

				gui_d.grid_end();

			gui_d.window_end();

			//gui_d.print_layout();

			gui_d.publish(gui);
			gui.render();
		
		}

		return true;
	}
};