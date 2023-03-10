#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "GraphicsCortex", 16, 0, false, true, false, false);
	Scene scene;
	//Ui ui(frame);

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
		//ui.new_frame();

		scene.render(frame_time);
		
		Gui::box("box1",  AABB2(vec2f(100, 100),  vec2f(100, 100)), simple_button, frame);
		Gui::box("box2",  AABB2(vec2f(100, 200),  vec2f(100, 100)), simple_button, frame);
		Gui::box("box3",  AABB2(vec2f(100, 300),  vec2f(100, 100)), simple_button, frame);
		Gui::box("box4",  AABB2(vec2f(100, 400),  vec2f(100, 100)), simple_button, frame);
		Gui::box("box5",  AABB2(vec2f(100, 500),  vec2f(100, 100)), simple_button, frame);
		Gui::box("box6",  AABB2(vec2f(100, 600),  vec2f(100, 100)), simple_button, frame);
		Gui::box("box7",  AABB2(vec2f(100, 700),  vec2f(100, 100)), simple_button, frame);
		Gui::box("box8",  AABB2(vec2f(100, 800),  vec2f(100, 100)), simple_button, frame);
		Gui::box("box9",  AABB2(vec2f(100, 900),  vec2f(100, 100)), simple_button, frame);
		Gui::box("box10", AABB2(vec2f(100, 1000), vec2f(100, 100)), simple_button, frame);
		Gui::box("box11", AABB2(vec2f(100, 1100), vec2f(100, 100)), simple_button, frame);

		//box.render((float)frame_time / 1000.0f);


		//ui.begin(Layout::Horizional);
		//
		//ui.box("box1", vec2(200, 200), toggle);
		//ui.box("box2", vec2(200, 200), toggle);
		//ui.box("box3", vec2(200, 200), toggle);
		//ui.box("box4", vec2(200, 200), toggle);
		//ui.box("box1", vec2(200, 200), toggle);
		//ui.box("box1", vec2(200, 200), toggle);
		//ui.box("box1", vec2(200, 200), toggle);
		//ui.box("box1", vec2(200, 200), toggle);
		//
		//if (true)
		//ui.end(background);
	}
	Gui::_destroy();
}