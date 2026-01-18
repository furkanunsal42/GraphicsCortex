#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "GUI/GUI.h"
#include "GUI/GUIDynamic.h"

class CortexGUITest_Dynamic : public TestBench {
public:

	bool run() {

		GUI gui;
		GUIDynamic gui_d;
		
		gui_d.new_frame(gui);

		size_t window_id = GUIDynamic::invalid_id;
		gui_d.window_begin(window_id);
		gui_d.stack_begin();
			gui_d.stack_begin();
				
				gui_d.box_begin();
				gui_d.box_prop().color = glm::vec4(0.1, 0.1, 0.1, 1);
				gui_d.box_end();

				gui_d.grid_begin();
				gui_d.grid_add_column(GUIDynamic::avail);
				gui_d.grid_add_row(GUIDynamic::avail);
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
		gui_d.print_layout();
		//std::cout << std::endl;
		//gui_d.resolve();
		
		while (true) {
			
			gui_d.publish(gui);
			gui.render();
		}

		return true;
	}
};