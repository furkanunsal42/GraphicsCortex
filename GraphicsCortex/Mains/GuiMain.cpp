#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "GraphicsCortex", 16, 0, true, true, true, false);
	Scene scene(frame);
	Gui gui(frame);

	Style simple_button;
	simple_button.color = gui::colorcode(0x383838);
	//simple_button.corner_rounding = Persentage(0.2f, Persentage::MIN_SIZE_DIM);
	simple_button.on_hover.cursor_type = Frame::Hand;
	simple_button.on_hover.color = gui::colorcode(0x424242);
	simple_button.color_change = 0.1;
	simple_button.color_interpolation = Interpolation::polynomial(3);

	Style dropdown_button;
	dropdown_button.color = gui::colorcode(0x383838);
	dropdown_button.on_hover.color = gui::colorcode(0x475F95);
	dropdown_button.color_change = 0.1;
	dropdown_button.color_interpolation = Interpolation::polynomial(3);
	dropdown_button.cursor_type = Frame::Hand;

	Style navbar_backround;
	navbar_backround.color = gui::colorcode(0x1E1E1E);

	Font text("Fonts\\Roboto-Regular.ttf");
	text.generate_text_graphic("File", scene, 0.2f);
	text.graphics_representation->set_uniform("text_color", 0.0f, 0.0f, 0.0f, 1.0f);
	text.graphics_representation->set_position(glm::vec3(0, 0, -1));

	bool show_dropdown = false;

	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window(1, 1, 1, 1);
		//frame.display_performance(180);
		
		gui.new_frame(frame_time);
		
		scene.render(frame_time);
		
		gui.box(vec2(0, 0), vec2(1920, 30), navbar_backround, true);		// topbar

		auto& dropdown = gui.box(vec2(100, 0), vec2(60, 30), simple_button, true);
		if (dropdown.click_released())
			show_dropdown = !show_dropdown;
		gui.layout(vec2(100, 0 + 30), vec2(0, 0), navbar_backround, show_dropdown);
		if (gui.content(vec2(180, 30), dropdown_button, true).click_released()) {
			std::cout << "1 pressed" << std::endl;
		}
		gui.content(vec2(180, 30), dropdown_button, true);
		gui.content(vec2(180, 30), dropdown_button, true);
		gui.content(vec2(180, 30), dropdown_button, true);
		gui.layout_end();

	}
}