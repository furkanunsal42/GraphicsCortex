#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "Cortex GUI", 0, 0, true, true, true, false);
	Gui2 gui(frame);

	Style style;
	style.color = vec3(1, 1, 1);

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window(1, 1, 1, 1);
		frame.display_performance(2000);

		gui.box(0, vec2(100, 100), vec2(100, 100), style, U"Hello GUI");

		gui.layout(1, vec2(400, 100), vec2(100, 100), style);
		gui.content(2, vec2(130, 20), style, U"Hello Layout");
		gui.content(3, vec2(30, 10), style, U"Hello Layout");
		gui.content(4, vec2(50, 10), style, U"Hello Layout");
		gui.content(5, vec2(100, 10), style, U"Hello Layout");
		gui.layout_end();

		gui.new_frame(deltatime);
	}
}