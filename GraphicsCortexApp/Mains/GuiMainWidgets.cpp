#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "Cortex GUI", 0, 0, true, true, true, Frame::CallbackLevel::LOW, false);
	Gui gui(frame);

	std::shared_ptr<Text> text = std::make_shared<Text>(Gui::font, "Haha");

	Style style1;
	style1.color = gui::colorcode(0xfcba03);
	style1.border_color = vec3(0, 0, 0);
	style1.border_thickness = vec4(1, 1, 1, 1);

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window(1, 1, 1, 1);
		frame.display_performance(2000);
		gui.new_frame(deltatime);

		gui.layout("widget_layout", vec2(100, 100), vec2(0, 0), style1);
		
		Button button("hello button", vec2(120, 35), U"click me to close");
		button.on_release = [&frame]() {
			frame.close_frame();
			};
		gui.custom_widget(button);

		gui.layout_end();

	}
}