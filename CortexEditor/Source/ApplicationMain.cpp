#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080);
	Scene scene(frame);
	Gui gui(frame);

	Style header_style;
	header_style.color = gui::colorcode(0x15171c);

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window(1, 1, 1, 1);
		gui.new_frame(deltatime);
		frame.display_performance(180);

		gui.layout(vec2(0, 0), vec2(scene.camera->screen_width, 40), header_style, U"", Layout::Horizional);
		gui.layout_end();
	}
}