#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "GraphicsCortex", 16, 0, false, true, false, false);
	Scene scene;
	Ui ui(frame);

	Style toggle;
	toggle.color = vec3(0.6f, 0.2f, 0.2f);
	toggle.corner_rounding = vec4(30, 30, 30, 30);
	toggle.margin = vec4(10, 10, 10, 10);
	toggle.padding = vec4(120, 0, 0, 0);
	toggle.on_hover.cursor_type = Frame::Hand;
	toggle.on_hover.color = vec3(0.8, 0.8, 0.8);
	toggle.on_hover.corner_rounding = vec4(80, 80, 80, 80);
	toggle.on_hover.padding = vec4(20, 20, 20, 20);
	toggle.color_change = 0.1;
	toggle.corner_rounding_change = 0.1;
	toggle.padding_change = 0.2;

	Style background;
	background.color = vec3(0.6, 0.6, 0.6);
	//background.margin = vec4(20, 0, 0, 0);
	background.on_hover.color = vec3(0.50, 0.50, 0.50);
	//background.on_hover.margin = vec4(30, 30, 30, 30);
	
	Box box1(frame, toggle, AABB2(vec2(0, 0), vec2(100, 100)));
	box1.overwrite_style.corner_rounding = vec4(20, 20, 0, 0);
	Box box2(frame, toggle, AABB2(vec2(200, 200), vec2(150, 150)));

	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window(0, 0, 0, 1);
		frame.display_performance(180);
		
		//ui.new_frame();

		//scene.render();
		
		box1.render(0);

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