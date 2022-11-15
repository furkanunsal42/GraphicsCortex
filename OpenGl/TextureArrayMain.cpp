#include "API/GraphicsCortex.h"
/*
int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, true, false, false);
	Scene scene;

	std::vector<Image> images;
	images.push_back(Image("Images/full_white.png", 4));
	images.push_back(Image("Images/full_blue.png", 4));
	images.push_back(Image("Images/full_black.png", 4));
	
	TextureArray texture_array;
	texture_array.load_images(images);
	
	Camera cam;
	cam.screen_width = (float)1920;
	cam.screen_height = (float)1080;
	cam.position.z = 1.5f;
	cam.max_distance = 10000.0;
	cam.perspective = true;
	scene.camera = &cam;

	//Shader shader("Shaders/TextureArray.vert", "Shaders/TextureArray.frag");
	//Program renderer(shader.vertex_shader, shader.fragment_shader);
	Program renderer = default_program::flatcolor_program();

	Graphic g = default_geometry::cube();
	g.load_program(renderer);
	g.set_uniform_update_queue(default_program::flat_default_uniform_queue(scene, g));

	g.set_position(glm::vec3(0, 0, -6));

	scene.add_graphic(g);
	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window();
		scene.camera->handle_movements(frame.window);
		scene.render(frame.window);
	}

}
*/

#include "API/GraphicsCortex.h"

int main() {
	int width = 1920, height = 1080;
	Frame frame(width, height, "GraphicsCortex", 4, 0, true, false, false);

	Scene scene;
	Material chassis_material;
	chassis_material.set_color_texture("Images/orange.png", 4);

	Material wheel_material;
	wheel_material.set_color_texture("Images/cobble.png", 4);
	
	Program solid_program = default_program::solid_program();

	Camera cam;
	cam.screen_width = (float)width;
	cam.screen_height = (float)height;
	cam.position.z = 1.5f;
	cam.max_distance = 10000.0;
	cam.perspective = true;
	scene.camera = &cam;
	
	AmbiantLight ambiant(glm::vec3(1.1f, 1.1f, 1.1f), solid_program);
	
	ambiant.set_uniform_upadte_queue(default_program::ambiant_light_default_uniform_queue(ambiant, 0));
	scene.add_light(ambiant);

	Graphic g = default_geometry::cube();
	g.load_program(solid_program);
	g.load_material(wheel_material);
	g.set_uniform_update_queue(default_program::solid_default_uniform_queue(scene, g));
	g.set_position(glm::vec3(-2, 0, -6));
	scene.add_graphic(g);

	float t = 0;

	while (frame.is_running()){
		double frame_time = frame.handle_window();

		frame.clear_window(0, 0, 0, 1);
		frame.display_performance(180);
		
		scene.camera->frame_time_ms = frame_time;
		scene.camera->handle_movements(frame.window);

		scene.render(frame.window);
		
	}
	
	return 0;
}
