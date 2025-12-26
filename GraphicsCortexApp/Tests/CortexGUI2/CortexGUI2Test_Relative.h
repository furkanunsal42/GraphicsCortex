#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "GUI2/GUI2.h"
#include "GUI2/GUI2Dynamic.h"

class CortexGUITest2_Relative : public TestBench {
public:

	bool run() {

		GUI2 gui;
		GUI2Dynamic gui_d;

		gui_d.window_begin("My Window 0");
		gui_d.window_prop().is_decorated = false;
		gui_d.window_prop().color = glm::vec4(0.8, 0.8, 0.8, 1);

			gui_d.stack_begin();
				
				gui_d.box_begin();
				gui_d.box_prop().target_size = glm::vec2(64, 256);
				gui_d.box_end();

				gui_d.box_begin();
				gui_d.box_prop().target_size = glm::vec2(64, 256);
				gui_d.box_prop().margin = glm::vec4(30, 0, 30, 0);
				gui_d.box_end();

				gui_d.box_begin();
				gui_d.box_prop().target_size = glm::vec2(64, 256);
				gui_d.box_end();

				gui_d.box_begin();
				gui_d.box_prop().target_size = glm::vec2(64, 256);
				gui_d.box_end();

			gui_d.stack_end();
		
		gui_d.window_end();
		gui_d.resolve();

		while (true) {

			gui_d.publish(gui);
			gui.render();
		}

		return true;
	}
};