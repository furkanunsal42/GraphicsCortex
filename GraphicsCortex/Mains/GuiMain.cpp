#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "GraphicsCortex", 16, 0, false, true, false, false);
	Scene scene;
	//Ui ui(frame);

	Style button;
	button.color = gui::colorcode(0x282828);
	//button.corner_rounding = gui::corner_rounding(10, 10, 10, 10);
	button.corner_rounding = Persentage(0.2f, Persentage::MIN_SIZE_DIM);
	button.border_color = gui::colorcode(0xD6D6D6);
	button.border_thickness = vec4(3, 3, 3, 3);
	button.on_hover.cursor_type = Frame::Hand;
	button.on_hover.displacement = vec2(20, 0);
	button.on_hover.color = gui::colorcode(0x40eb34);
	button.displacement_change = 0.2;
	button.color_change = 0.2;
	button.displacement_interpolation = Interpolation::polynomial(3);
	button.color_interpolation = Interpolation::polynomial(3);

	std::vector<Box> boxes;
	for (int i = 0; i < 20; i++)
		boxes.push_back(Box(frame, button, AABB2(vec2(100, i * 70 + 100), vec2(200, 60))));

	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window(1, 1, 1, 1);
		frame.display_performance(180);
		
		Gui::new_frame(frame);
		//ui.new_frame();

		scene.render();
		

		for (Box& box : boxes){
			box.render((float)frame_time / 1000.0f);
		}


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
}