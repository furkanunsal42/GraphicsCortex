#include "GraphicsCortex.h"
#include "GraphRenderer/GraphRenderer.h"

int main() {
	int window_width = 1024;

	Frame frame(window_width, window_width, "CTReconstructor GraphRenderer", 0, 2, true, true, false, Frame::CallbackLevel::NOTIFICATION, false);
	Scene scene(frame);
	scene.camera->fov = 90;
	scene.camera->max_distance = 1000;
	
	GraphRenderer graph_renderer(glm::vec3(-5, -5, -5), glm::vec3(5, 5, 5), glm::ivec2(4096, 32));

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window();
		frame.display_performance();

		scene.camera->handle_movements(frame.window, deltatime);

		graph_renderer.render(*scene.camera);
	}
}