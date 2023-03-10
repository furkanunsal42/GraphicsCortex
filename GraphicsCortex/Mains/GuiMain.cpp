#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "GraphicsCortex", 16, 0, false, true, false, false);
	Scene scene;

	Style simple_button;
	simple_button.color = gui::colorcode(0x282828);
	simple_button.corner_rounding = Persentage(0.2f, Persentage::MIN_SIZE_DIM);
	simple_button.on_hover.cursor_type = Frame::Hand;
	simple_button.on_hover.color = gui::colorcode(0x424242);
	simple_button.on_hover.displacement = gui::displacement(10, 0);
	simple_button.displacement_change = 0.1;
	simple_button.color_change = 0.1;
	simple_button.displacement_interpolation = Interpolation::polynomial(3);
	simple_button.color_interpolation = Interpolation::polynomial(3);

	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window(1, 1, 1, 1);
		frame.display_performance(180);
		
		Gui::new_frame(frame, frame_time);

		scene.render(frame_time);
		
		for (int i = 0; i < 10; i++) {
			Gui::box(AABB2(vec2f(100, (60 + 10) * i + 100), vec2f(100, 60)), simple_button, frame).render();
		}

	}
	Gui::_destroy();
}