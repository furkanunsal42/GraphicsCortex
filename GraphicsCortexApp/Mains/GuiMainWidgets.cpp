#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "Cortex GUI", 0, 0, true, true, true, Frame::CallbackLevel::LOW, false);
	Gui gui(frame);

	std::shared_ptr<Text> text = std::make_shared<Text>(Gui::font, "Haha");

	Style style1;
	style1.color = vec3(1, 1, 1);

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window(1, 1, 1, 1);
		frame.display_performance(2000);

		gui.layout("button_layout", vec2(100, 100), vec2(0, 0), style1);
		Button button("hello button", vec2(120, 35), U"Click Me to Print");
		button.on_release = [&frame]() {
			std::cout << "You pressed on me haha!" << std::endl;
			};
		gui.custom_widget(button);
		gui.layout_end();

		gui.layout("button2_layout", vec2(100, 200), vec2(0, 0), style1);
		Button button2("hello button2", vec2(30, 30), U"X");
		button2.button_style.color = gui::colorcode(0xd46b57);
		button2.button_style.on_hover.color = gui::colorcode(0xd95038);
		button2.on_release = [&frame]() {
			frame.close_frame();
			};
		gui.custom_widget(button2);
		gui.layout_end();

		gui.layout("textinput_layout", vec2(300, 100), vec2(0, 0), style1);
		TextInput textinput("hello textinput", vec2(120, 35), U"Write to Me");
		gui.override_style.corner_rounding = vec4(15, 15, 15, 15);
		gui.custom_widget(textinput);
		gui.override_style.clear();
		gui.layout_end();

		gui.new_frame(deltatime);
	}
}