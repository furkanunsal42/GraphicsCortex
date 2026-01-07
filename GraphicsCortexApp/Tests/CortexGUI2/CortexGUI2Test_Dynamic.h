#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "GUI2/GUI2.h"
#include "GUI2/GUI2Dynamic.h"

class CortexGUITest2_Dynamic : public TestBench {
public:

	bool run() {

		GUI2 gui;
		GUI2Dynamic gui_d;
		
		size_t window_id = GUI2Dynamic::invalid_id;
		gui_d.window_begin(window_id);
		gui_d.stack_begin();
			gui_d.stack_begin();
				
				gui_d.box_begin();
				gui_d.box_prop().color = glm::vec4(0.1, 0.1, 0.1, 1);
				gui_d.box_end();

				gui_d.grid_begin();
				gui_d.grid_add_column(GUI2Dynamic::avail);
				gui_d.grid_add_row(GUI2Dynamic::avail);
				gui_d.grid_end();
				
				gui_d.box_begin();
				gui_d.box_prop().color = glm::vec4(0.1, 0.1, 0.1, 1);
				gui_d.box_end();
			
			gui_d.stack_end();
				
				gui_d.box_begin();
				gui_d.box_prop().color = glm::vec4(0.1, 0.1, 0.1, 1);
				gui_d.box_end();
				
				gui_d.box_begin();
				gui_d.box_prop().color = glm::vec4(0.1, 0.1, 0.1, 1);
				gui_d.box_end();

		gui_d.stack_end();
		gui_d.window_end();

		//gui_d.print_nodes();
		//std::cout << std::endl;
		//gui_d.print_layout();
		//std::cout << std::endl;
		gui_d.resolve();
		
		while (true) {
			
			gui_d.publish(gui);
			gui.render();
		}

		return true;
	}
};