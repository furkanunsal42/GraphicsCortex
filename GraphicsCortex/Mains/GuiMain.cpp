#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, false, false, false, false);
	Scene scene;
	Ui ui(frame);

	Style light_style;
	light_style.color = Vec3<float>(0.7, 0.7, 0.7);
	light_style.on_hover.color = Vec3<float>(0.8, 0.8, 0.8);
	light_style.on_active.color = Vec3<float>(0.9, 0.9, 0.9);
	light_style.corner_rounding = Vec4<float>(25, 25, 25, 25);
	light_style.on_hover.corner_rounding = Vec4<float>(25, 25, 25, 25);
	light_style.on_active.corner_rounding = Vec4<float>(25, 25, 25, 25);
	light_style.on_hover.cursor_type = Frame::CursorType::Hand;
	light_style.on_active.cursor_type = Frame::CursorType::Hand;

	Style dark_style;
	dark_style.color = Vec3<float>(0.15f, 0.15f, 0.15f);
	dark_style.on_hover.color = Vec3<float>(0.2, 0.2, 0.2);
	dark_style.on_active.color = Vec3<float>(0.3, 0.3, 0.3);
	dark_style.corner_rounding = Vec4<float>(50, 50, 50, 50);

	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window();
		frame.display_performance(180);
		ui.new_frame();

		scene.render();
		
		ui.begin(Layout::Horizional);
		ui.box("box1", Vec2<float>(200, 100), dark_style);
		ui.box("box2", Vec2<float>(200, 100), light_style);
		
		ui.begin(Layout::Vertical);
		ui.box("box3", Vec2<float>(100, 100), dark_style);
		ui.box("box4", Vec2<float>(100, 100), light_style);
		ui.end();

		ui.box("box5", Vec2<float>(100, 100), dark_style);
		ui.box("box6", Vec2<float>(100, 100), light_style);
		ui.end();
	}
}