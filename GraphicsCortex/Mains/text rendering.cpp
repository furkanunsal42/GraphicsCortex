#include "GraphicsCortex.h"

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 16, 0, true, true, false, false);
	Scene scene(frame);
	scene.camera.perspective = true;

	Font font("Fonts\\Roboto-Regular.ttf", 200);
	font._font_atlas.texture_slot = 0;
	font.generate_text_graphic("\
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
									probes into the far reaches of space", scene, 1, 1, true);

	font.graphics_representation->set_position(glm::vec3(0, 0, 0));
	font.graphics_representation->set_uniform("text_color", 0.7f, 0.7f, 0.2f, 0.8f);

	while (frame.is_running()) {
		double frametime = frame.handle_window();
		frame.clear_window(0, 0, 0, 1);
		frame.display_performance(1000);

		scene.camera.handle_movements(frame.window, frametime);

		font._font_atlas.bind();
		scene.render();

		font.graphics_representation->set_position(font.graphics_representation->get_position() + glm::vec3(0, 3.0f/100 * frametime/1000, 0));

		font.graphics_representation->update_matrix();
		font.graphics_representation->update_uniforms();
		font.graphics_representation->draw(false);
		//scene.render(false);
	}
}