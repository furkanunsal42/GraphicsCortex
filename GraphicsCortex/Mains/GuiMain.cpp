#include "GraphicsCortex.h"


int main() {
	Frame frame(1920, 1080, "GraphicsCortex", 16, 0, false, true, false, false);
	Scene scene;

	Style simple_button;
	simple_button.color = gui::colorcode(0x282828);
	//simple_button.corner_rounding = Persentage(0.2f, Persentage::MIN_SIZE_DIM);
	simple_button.border_color = gui::colorcode(0x1e1e1e);
	simple_button.border_thickness = Persentage(0.1f, Persentage::MIN_SIZE_DIM);
	simple_button.on_hover.cursor_type = Frame::Hand;
	simple_button.on_hover.color = gui::colorcode(0x424242);
	simple_button.color_change = 0.1;
	simple_button.color_interpolation = Interpolation::polynomial(3);

	Style dropdown_button;
	dropdown_button.color = gui::colorcode(0x282828);
	dropdown_button.on_hover.color = gui::colorcode(0x475F95);
	dropdown_button.color_change = 0.1;
	dropdown_button.color_interpolation = Interpolation::polynomial(3);
	dropdown_button.cursor_type = Frame::Hand;

	DropBox dropbox(vec2(100, 100), vec2(100, 40), vec2(200, 40), simple_button, dropdown_button, frame);



	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window(1, 1, 1, 1);
		frame.display_performance(180);
		
		Gui::new_frame(frame, frame_time);

		scene.render(frame_time);

		dropbox.render();
		

	}
	Gui::_destroy();
}