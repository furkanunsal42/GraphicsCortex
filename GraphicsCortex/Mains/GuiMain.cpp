#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "GraphicsCortex", 16, 0, false, true, false, false);
	Scene scene;
	Ui ui(frame);

	Style toggle;
	toggle.color = vec3(0xffffff);
	toggle.corner_rounding = vec4(13, 13, 13, 13);
	toggle.border_color = vec3(0xD6D6D6);
	toggle.border_thickness = vec4(3, 3, 3, 3);
	toggle.on_hover.cursor_type = Frame::Hand;
	toggle.on_hover.displacement = vec2(0, -10);
	toggle.on_hover.color = vec3(0xfdfdfd);
	toggle.displacement_change = 0.3;
	toggle.color_change = 0.3;
	toggle.displacement_interpolation = Interpolation::polynomial(0.65);
	
	Box box1(frame, toggle, AABB2(vec2(100, 100), vec2(200, 60)));
	Box box2(frame, toggle, AABB2(vec2(200, 200), vec2(150, 150)));

	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window(0, 0, 0, 1);
		frame.display_performance(180);
		
		Gui::new_frame(frame);
		//ui.new_frame();

		scene.render();
		

		box1.render((float)frame_time/1000.0f);


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