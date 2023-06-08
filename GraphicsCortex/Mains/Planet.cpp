#include "GraphicsCortex.h"

int main() {

	Frame frame(1920, 1080, "Planet", 0, 0, true, true, true, false);
	Scene scene(frame);

	//Program_s renderer = default_program::solid_program_s();
	Program_s renderer = default_program::flatcolor_program_s();

	std::vector<float> verticies = {
		-1, -1, -1,
		1, -1, -1,
		0, 1, -1,
	};

	std::vector<unsigned int> indicies = {
		0, 1, 2
	};

	Graphic_s triangle;
	triangle->load_model(Mesh_s());
	triangle->mesh->array_buffer->load_buffer(verticies);
	triangle->mesh->array_buffer->push_attribute(3);
	triangle->mesh->index_buffer->load_buffer(indicies, 3);
	triangle->load_program(renderer);
	triangle->set_uniform_all(default_program::flat_default_uniform_queue(*scene.camera.obj, triangle));
	scene.add_graphic(triangle);

	while (frame.is_running()) {
		double frametime = frame.handle_window();
		frame.display_performance();
		frame.clear_window();
	
		scene.camera->handle_movements(frame.window, frametime);

		scene.render(false);
	}
}