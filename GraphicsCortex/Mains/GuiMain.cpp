#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "GraphicsCortex", 16, 0, false, true, false, false);
	Scene scene;
	Ui ui(frame);

	Style toggle;
	toggle.color = vec3(0.6f, 0.2f, 0.2f);

	Box box1(frame, toggle, AABB2(vec2(0, 0), vec2(100, 100)));
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