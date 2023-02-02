#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, false, false, false, false);
	Scene scene;
	Ui ui(frame);

	Style light_style;
	light_style.color = Vec3<float>(0, 1, 0);
	light_style.on_hover.color = Vec3<float>(0, 0.8, 0);
	light_style.on_active.color = Vec3<float>(0.8, 0.8, 0.8);

	Style dark_style;
	dark_style.color = Vec3<float>(0, 0, 1);
	dark_style.on_hover.color = Vec3<float>(0, 0, 0.8);
	dark_style.on_active.color = Vec3<float>(0.8, 0.8, 0.8);

	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window();
		frame.display_performance(180);
		ui.new_frame();

		scene.render();
		
		ui.begin(Layout::Horizional);
		ui.box("box1", Vec2<float>(100, 100), dark_style);
		ui.box("box2", Vec2<float>(100, 100), light_style);
		
		ui.begin(Layout::Vertical);
		ui.box("box3", Vec2<float>(100, 100), dark_style);
		ui.box("box4", Vec2<float>(100, 100), light_style);
		ui.end();

		ui.box("box5", Vec2<float>(100, 100), dark_style);
		ui.box("box6", Vec2<float>(100, 100), light_style);
		ui.end();
	}
}