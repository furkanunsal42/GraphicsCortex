#include "API/GraphicsCortex.h"

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, true, false, false);
	Scene scene;

	std::vector<Image> images;
	images.push_back(Image("Images/full_white.png", 4));
	images.push_back(Image("Images/full_blue.png", 4));
	images.push_back(Image("Images/full_white.png", 4));

	TextureArray texture_array;
	texture_array.initialize_blank_images(256, 256, 3, 4);
	texture_array.load_images(images);
	texture_array.texture_slot = 0;
	images.clear();

	Camera cam;
	cam.screen_width = (float)1920;
	cam.screen_height = (float)1080;
	cam.position.z = 1.5f;
	cam.max_distance = 10000.0;
	cam.perspective = true;
	scene.camera = &cam;

	Shader shader("Shaders/TextureArray.vert", "Shaders/TextureArray.frag");
	Program renderer(shader.vertex_shader, shader.fragment_shader);

	Graphic g = default_geometry::cube();
	g.load_program(renderer);
	g.set_uniform_update_queue(default_program::flat_default_uniform_queue(scene, g));
	g.add_uniform_update_queue(uniform_update<int>("texture_array", 0));
	g.add_uniform_update_queue(uniform_update<int>("array_index", 1));

	g.set_position(glm::vec3(0, 0, -6));

	scene.add_graphic(g);
	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window();
		scene.camera->handle_movements(frame.window, frame_time);
		scene.render(frame.window);
	}

}
