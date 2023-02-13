#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "GraphicsCortex", 16, 0, false, true, false, false);
	Scene scene;
	Ui ui(frame);

	Style toggle;
	toggle.color = Vec3<float>(0.2f, 0.2f, 0.2f);
	toggle.corner_rounding = Vec4<float>(50, 50, 50, 50);
	toggle.margin = Vec4<float>(10, 10, 10, 10);
	//toggle.padding = Vec4<float>(50, 0, 0, 0);
	toggle.on_hover.cursor_type = Frame::Hand;
	toggle.on_active.color = Vec3<float>(0.3, 0.3, 0.3);
	
	Style background;
	background.color = Vec3<float>(0.6, 0.6, 0.6);
	background.margin = Vec4<float>(20, 0, 0, 0);
	background.on_hover.color = Vec3<float>(0.50, 0.50, 0.50);
	background.on_hover.margin = Vec4<float>(30, 30, 30, 30);
	

	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window(0, 0, 0, 1);
		frame.display_performance(180);
		ui.new_frame();

		scene.render();
		
		ui.begin(Layout::Horizional);
		ui.box("box1", Vec2<float>(200, 200), toggle);
		ui.box("box2", Vec2<float>(200, 200), toggle);
		ui.end(background);
	}
}