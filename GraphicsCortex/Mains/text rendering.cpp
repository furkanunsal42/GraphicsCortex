#include "GraphicsCortex.h"

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 16, 0, true, true, false, false);
	Scene scene(frame);

	scene.camera.perspective = true;

	Font_s font("Fonts\\Roboto-Regular.ttf", 200);
	Text text(font);

	text.set_text("\
									It is a dark time for the Rebellion. Although \
									the Death Star has been destroyed, \
									Imperial troops have driven the Rebel forces \
									from their hidden base and pursued them \
									across the galaxy. \
									\n\n\
									Evading the dreaded Imperial Starfleet, a group \
									of freedom fighters led by Luke Skywalker has \
									established a new secret base on the remote ice \
									world of Hoth. \
									\n\n\
									The evil lord Darth Vader, obsessed with finding \
									young Skywalker, has dispatched thousands of remote \
									probes into the far reaches of space");

	text.set_max_width(1);
	text.render();
	text.graphic->set_position(glm::vec3(0, 0, 0));
	text.graphic->set_uniform_all(default_program::basic_uniform_queue(scene, text.graphic));
	text.graphic->set_uniform("text_color", 242.0f / 255, 166.0f / 255, 0.0f / 255, 1.0f);
	text.graphic->set_uniform("texture_slot", 0);
	text.graphic->set_uniform("screen_resolution", (float*)&scene.camera.screen_width, (float*)&scene.camera.screen_height);


	while (frame.is_running()) {
		double frametime = frame.handle_window();
		frame.clear_window(0, 0, 0, 1);
		frame.display_performance(1000);

		scene.camera.handle_movements(frame.window, frametime);

		scene.render();

		text.graphic->set_position(text.graphic->get_position() + glm::vec3(0, 6.0f / 100 * frametime / 1000, 0));

		text.render();
		//scene.render(false);
	}
}