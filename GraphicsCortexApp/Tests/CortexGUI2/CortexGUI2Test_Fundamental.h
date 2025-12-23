#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "GUI2/GUI2.h"


class CortexGUITest2_Fundamental : public TestBench {
public:

	bool run() {

		GUI2 gui;

		while (true) {

			glm::vec4 color = glm::vec4(1, 0, 0, 1);

			if ((gui.window_begin("window2", glm::vec2(128), glm::vec2(512)) & GUI2::MouseEvent::LeftHold)) {
				color = glm::vec4(0, 0, 1, 1);
			}
			gui.window_prop().is_decorated = true;
			glm::vec2 resolution1 = gui.window_prop().size;
			gui.window_end();

			gui.window_begin("window", glm::vec2(100), glm::vec2(128));
			gui.window_prop().name			= "New Window";
			gui.window_prop().position		= glm::vec2(100);
			gui.window_prop().size			= resolution1;//glm::vec2(128);
			gui.window_prop().color			= glm::vec4(0.7, 0.7, 0.7, 1);
			gui.window_prop().is_resizable	= false;

			gui.box_begin(glm::vec2(0), glm::vec2(32));
			gui.box_prop().color	= color;
			gui.box_end();

			gui.window_end();


			gui.render();
		}
	}
};