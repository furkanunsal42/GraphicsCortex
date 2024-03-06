#include "GraphicsCortex.h"

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, true, true, false, Frame::CallbackLevel::LOW, false);
	Scene scene(frame);
	Gui gui(frame);

	std::shared_ptr<Font> font = std::make_shared<Font>("../GraphicsCortex/Fonts/Roboto-Regular.ttf", 24);
	std::shared_ptr<Text> text = std::make_shared<Text>(font);

	text->set_text(U"\
						Long ago in a distant galaxy, an electro-optical workload and its trusty image processing software were unleashed upon the world of technology! \n\
						\n\
						But as they began to process images, a conflict arose between them, each claiming to be the true master of the satellite imagery domain. \n\
						\n\
						Before the final pixel was processed, the workload and its software opened a portal in cyberspace and flung the engineering team into the future, where their processing power is law!\n\
						\n\
						Now the team seeks to return to the present and restore order to the chaotic world of image processing that is Ascende Superius!\n");


	text->set_max_width(1);
	text->set_scale(1);
	scene.add(text);
	text->graphic->position = glm::vec3(0, 0, -1);
	text->set_color(vec4(242.0f / 255, 166.0f / 255, 0.0f / 255, 1.0f));
	
	while (frame.is_running()) {
		double frametime = frame.handle_window();
		frame.clear_window(0, 0, 0, 1);
		frame.display_performance(1000);

		text->graphic->position = text->graphic->position + glm::vec3(0, 6.0f / 100 * frametime / 1000, 0);
		
		gui.new_frame(frametime);

		scene.camera->handle_movements(frame.window, frametime);

		scene.render();

	}
}