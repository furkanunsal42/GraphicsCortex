#include "GraphicsCortex.h"

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, true, true, true, false);
	Scene scene(frame);
	scene.camera.screen_width = 1920;
	scene.camera.screen_height = 1080;
	scene.camera.max_distance = 100.0f;

	Program_s solid_program = default_program::solid_program_s();

	DirectionalLight_s sunlight(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(1, 1, 1), solid_program);
	sunlight->set_uniform_upadte_queue(default_program::directional_light_default_uniform_queue(*sunlight.obj, 0));
	scene.add_light(sunlight);
	
	Program_s renderer(Shader("Custom Shaders/Depth.vert", "Custom Shaders/Depth.frag"));

	UnorderedMaterial_s material(2);
	material->set_texture("Images/StoneTiles/tiles_color.jpg", 4, 0, UnorderedMaterial::COLOR);
	material->set_texture("Images/StoneTiles/tiles_normal.jpg", 4, 1, UnorderedMaterial::NORMAL);

	Graphic_s graphic(default_geometry::cube());
	graphic->load_material(material);
	graphic->load_program(solid_program);
	graphic->set_uniform_all(default_program::solid_default_uniform_queue(scene.camera, graphic));
	scene.add_graphic(graphic);


	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window(0.4, 0.4, 0.8);
		frame.display_performance();
		
		scene.camera.handle_movements(frame.window, frame_time);

		scene.render(false);

	}
}