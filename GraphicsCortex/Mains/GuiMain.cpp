#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, false, false, false, false);
	Scene scene;
	Ui ui(frame);

	Style toggle;
	toggle.color = Vec3<float>(0.2f, 0.2f, 0.2f);
	toggle.corner_rounding = Vec4<float>(15, 15, 15, 15);
	toggle.border_color = Vec3<float>(1, 1, 1);
	toggle.border_thickness = Vec4<float>(2, 2, 2, 2);
	toggle.on_hover.cursor_type = Frame::Hand;
	toggle.on_active.color = Vec3<float>(0.3, 0.3, 0.3);


	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window();
		frame.display_performance(180);
		ui.new_frame();

		scene.render();
		
		ui.begin(Layout::Horizional);
		ui.box("box1", Vec2<float>(50, 30), toggle);
		ui.end();
	}
}