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
		
		gui_d.window_begin("My Window 0");
		gui_d.stack_begin();
			gui_d.stack_begin();
				gui_d.box_begin();
				gui_d.box_end();
				gui_d.grid_begin(glm::ivec2(1, 1));
				gui_d.grid_end();
				gui_d.box_begin();
				gui_d.box_end();
			gui_d.stack_end();
				gui_d.box_begin();
				gui_d.box_end();
				gui_d.box_begin();
				gui_d.box_end();
		gui_d.stack_end();
		gui_d.window_end();


		//gui_d.print_nodes();
		gui_d.print_layout();

		std::cin.get();

		return true;
	}
};