#include "GraphicsCortex.h"

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 0, 1, true, true, true, false);
	Scene scene;
	scene.camera.screen_width = 1920;
	scene.camera.screen_height = 1080;
	scene.camera.max_distance = 100.0f;

	Program_s renderer(Shader("Custom Shaders/Depth.vert", "Custom Shaders/Depth.frag"));
	Graphic_s graphic(default_geometry::cube());
	graphic->load_program(renderer);
	graphic->set_uniform_all(default_program::flat_default_uniform_queue(scene, graphic));
	scene.add_graphic(graphic);

	scene.camera.update_matrixes();
	std::cout << scene.camera.projection_matrix * glm::vec4(0, 0, 0, 1) << std::endl;


	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window();
		frame.display_performance();
		
		scene.camera.handle_movements(frame.window, frame_time);

		scene.render(false);

	}
}