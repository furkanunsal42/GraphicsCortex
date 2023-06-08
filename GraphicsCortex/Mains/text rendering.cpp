#include "GraphicsCortex.h"

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 16, 0, true, true, false, false);
	Scene scene(frame);
	Gui gui(frame);

	Font_s font("Fonts\\Roboto-Regular.ttf", 200);
	Text text(font);

	text.set_text(U"\
						Long ago in a distant galaxy, an electro-optical workload and its trusty image processing software were unleashed upon the world of technology! \n\
						\n\
						But as they began to process images, a conflict arose between them, each claiming to be the true master of the satellite imagery domain. \n\
						\n\
						Before the final pixel was processed, the workload and its software opened a portal in cyberspace and flung the engineering team into the future, where their processing power is law!\n\
						\n\
						Now the team seeks to return to the present and restore order to the chaotic world of image processing that is Ascende Superius!\n");

	text.set_max_width(1);
	text.set_scale(1);
	text.render();
	text.graphic->set_position(glm::vec3(0, 0, -1));
	text.graphic->set_uniform_all(default_program::basic_uniform_queue(*scene.camera.obj, text.graphic));
	text.graphic->set_uniform("text_color", 242.0f / 255, 166.0f / 255, 0.0f / 255, 1.0f);
	text.graphic->set_uniform("texture_slot", 0);
	text.graphic->set_uniform("screen_resolution", (float*)&scene.camera->screen_width, (float*)&scene.camera->screen_height);

	while (frame.is_running()) {
		double frametime = frame.handle_window();
		frame.clear_window(0, 0, 0, 1);
		frame.display_performance(1000);

		gui.new_frame(frametime);

		scene.camera->handle_movements(frame.window, frametime);

		scene.render();

		text.graphic->set_position(text.graphic->get_position() + glm::vec3(0, 6.0f / 100 * frametime / 1000, 0));

		text.render();
	}
}