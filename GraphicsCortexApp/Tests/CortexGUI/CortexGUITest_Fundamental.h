#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "GUI/GUI.h"


class CortexGUITest_Fundamental : public TestBench {
public:

	bool run() {

		GUI gui;

		while (true) {

			glm::vec4 color = glm::vec4(1, 0, 0, 1);

			gui.window_begin("window2", glm::vec2(128), glm::vec2(512));
			gui.window_prop().is_decorated = true;

			glm::vec2 resolution1 = gui.window_prop().size;
			if (gui.window_prop().mouse_event & GUI::MouseEvent::LeftHold)
				color = glm::vec4(0, 0, 1, 1);

			gui.window_end();

			gui.window_begin("window", glm::vec2(100), glm::vec2(128));
			gui.window_prop().name			= "New Window";
			gui.window_prop().position		= glm::vec2(100);
			gui.window_prop().size			= resolution1;//glm::vec2(128);
			gui.window_prop().color			= glm::vec4(0.7, 0.7, 0.7, 1);
			gui.window_prop().is_resizable	= false;

			if (gui.window_prop().mouse_event & GUI::MouseEvent::LeftPress)
				std::cout << "window hold" << std::endl;

			gui.box_begin(glm::vec2(0), glm::vec2(32));
			gui.box_prop().color	= color;
			
			if (gui.box_prop().mouse_event & GUI::MouseEvent::LeftPress)
				std::cout << "holding" << std::endl;

			gui.box_end();

			gui.window_end();

			gui.render();
		}
	}
};