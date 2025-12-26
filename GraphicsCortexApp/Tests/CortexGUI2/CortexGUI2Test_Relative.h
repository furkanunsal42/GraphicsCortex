#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "GUI2/GUI2.h"
#include "GUI2/GUI2Dynamic.h"

class CortexGUITest2_Relative : public TestBench {
public:

	bool run() {

		constexpr auto avail	= GUI2Dynamic::avail;
		constexpr auto fit		= GUI2Dynamic::fit;

		GUI2 gui;
		GUI2Dynamic gui_d;

		//gui_d.window_begin("My Window 0")
		//	.set_is_decorated(true)
		//	.set_color(glm::vec4(0.8, 0.8, 0.8, 1));
		//
		//	gui_d.stack_begin()
		//		.set_target_size(glm::vec2(300, GUI2Dynamic::fit));
		//		
		//		gui_d.box_begin()
		//			.set_target_size(glm::vec2(64, 256));
		//
		//		gui_d.box_begin()
		//			.set_target_size(glm::vec2(avail, avail))
		//			.set_margin(glm::vec4(avail, 0, avail, 0));
		//
		//		gui_d.box_begin()
		//			.set_target_size(glm::vec2(64, 256));
		//
		//		gui_d.box_begin()
		//			.set_target_size(glm::vec2(64, 256));
		//
		//	gui_d.stack_end();
		//
		//gui_d.window_end();


		gui_d.window_begin("My Window 1")
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

				gui_d.grid_region(glm::ivec2(0, 1));
				gui_d.box_begin()
					.set_target_size(glm::vec2(avail, avail))
					.set_margin(glm::vec4(10));

				gui_d.grid_region(glm::ivec2(1, 1));
				gui_d.box_begin()
					.set_target_size(glm::vec2(avail, avail))
					.set_margin(glm::vec4(10));

				//gui_d.box_begin()
				//	.set_target_size(glm::vec2(avail, avail))
				//	.set_margin(glm::vec4(avail, 0, avail, 0));
				//
				//gui_d.box_begin()
				//	.set_target_size(glm::vec2(64, 256));
				//
				//gui_d.box_begin()
				//	.set_target_size(glm::vec2(64, 256));

			gui_d.grid_end();

		gui_d.window_end();


		gui_d.resolve();

		while (true) {

			gui_d.publish(gui);
			gui.render();
		}

		return true;
	}
};