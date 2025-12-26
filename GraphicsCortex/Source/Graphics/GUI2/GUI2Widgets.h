#pragma once
#include "GUI2Dynamic.h"


namespace widget {

	struct Window {

		glm::vec2 drag_begin;
		glm::vec2 drag_end;

		bool dockable = false;

		Window(std::string idstr);
		void publish(GUI2Dynamic& gui_dynamic);

	private:

	};

	struct DockSurface {

		DockSurface(std::string idstr);
		void publish(GUI2Dynamic& gui_dynamic);
		BANA K?TAP AL!!!
			BANA K?TAP AL!!!
				BANA K?TAP AL!!!
					BANA K?TAP AL!!!
	private: 

	};

	struct Box {

		Box& set_color(glm::vec4);
		void publish(GUI2Dynamic& gui_dynamic);

	};

	struct Label {

		std::string text;
		void publish(GUI2Dynamic& gui_dynaimc);
	
	};

	struct Image {

		void publish(GUI2Dynamic& gui_dynaimc);

	};

	struct TextArea {

		void publish(GUI2Dynamic& gui_dynaimc);

	};

	struct Slider {

		void publish(GUI2Dynamic& gui_dynaimc);

	};

	struct DragFloat {

		void publish(GUI2Dynamic& gui_dynaimc);

	};

	struct Button {

		void publish(GUI2Dynamic& gui_dynaimc);

	};

	struct CheckBox {

		void publish(GUI2Dynamic& gui_dynaimc);

	};

	struct ComboBox {

		void publish(GUI2Dynamic& gui_dynaimc);

	};

	struct Menu {

		void publish(GUI2Dynamic& gui_dynaimc);

	};

	struct Tab {

		void publish(GUI2Dynamic& gui_dynaimc);

	};

	struct Collapsible {

		void publish(GUI2Dynamic& gui_dynaimc);

	};

}

