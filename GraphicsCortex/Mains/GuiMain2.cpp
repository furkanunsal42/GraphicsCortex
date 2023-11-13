#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "Cortex GUI", 0, 0, true, true, true, false);
	Gui2 gui(frame);

	Style style1;
	style1.color = vec3(1, 1, 1);
	style1.border_color = vec3(0, 0, 0);
	style1.border_thickness = vec4(1, 1, 1, 1);

	Style style2;
	style2.color = vec3(1, 0, 0);
	style2.border_color = vec3(0, 0, 0);
	style2.border_thickness = vec4(1, 1, 1, 1);

	Style style3;
	style3.color = vec3(1, 1, 0);
	style3.border_color = vec3(0, 0, 0);
	style3.border_thickness = vec4(1, 1, 1, 1);

	Style style4;
	style4.color = vec3(1, 0, 1);
	style4.border_color = vec3(0, 0, 0);
	style4.border_thickness = vec4(1, 1, 1, 1);
	style4.on_hover.color = vec3(0, 0, 1);
	style4.on_hover.border_thickness = vec4(4, 4, 4, 4);
	style4.color_change = 0.3;
	style4.border_thickness_change = 0.3;

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window(1, 1, 1, 1);
		frame.display_performance(2000);

		gui.box(0, vec2(100, 100), vec2(100, 100), style1, U"Hello GUI");

		gui.layout(1, vec2(400, 100), vec2(100, 100), style1);
		gui.content(2, vec2(130, 20), style2, U"Hello Layout");

		gui.override_style.color = vec3(0, 1, 1);
		gui.override_style.margin = vec4(5, 5, 5, 5);
		gui.content(3, vec2(30, 10), style2, U"Hello Layout");
		gui.override_style.clear();
		gui.content(4, vec2(50, 10), style2, U"Hello Layout");
		gui.content(5, vec2(100, 10), style2, U"Hello Layout");

		gui.layout_content(6, vec2(100, 30), style1, Layout::Horizional);
		gui.content(7, vec2(30, 30), style3, U"Hello linked lists");
		gui.content(8, vec2(30, 30), style3, U"Hello linked lists");
		gui.content(9, vec2(30, 30), style3, U"Hello linked lists");


		gui.override_style.on_hover.margin = vec4(5, 5, 5, 5);
		gui.override_style.margin_change = 1;
		gui.layout_content(10, vec2(120, 30), style1, Layout::Vertical);
		gui.content(11, vec2(30, 30), style4, U"Hello Recursion");
		gui.override_style.clear();
		gui.content(12, vec2(30, 30), style4, U"Hello Recursion");
		gui.content(13, vec2(60, 60), style4, U"Hello Recursion");
		
		gui.layout_content_end();

		gui.content(14, vec2(30, 30), style3, U"Hello linked lists");

		gui.layout_content_end();

		gui.layout_end();

		gui.new_frame(deltatime);
	}
}