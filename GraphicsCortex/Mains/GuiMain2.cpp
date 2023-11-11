#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "Cortex GUI", 0, 1, true, true, true, false);
	Gui2 gui(frame);

	Style style;
	style.color = vec3(0.5f, 0.5f, 0.5f);

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window(1, 1, 1, 1);
		frame.display_performance(180);

		gui.box(0, vec2(100, 100), vec2(100, 100), style, U"Hello GUI");

		gui.new_frame(deltatime);
	}
}