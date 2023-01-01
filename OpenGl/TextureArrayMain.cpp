#include "API/GraphicsCortex.h"

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 4, 0, true, false, false);
	Scene scene;
	scene.camera.screen_width = 1920;
	scene.camera.screen_height = 1080;
	
	Program_s program(Program(Shader("Shaders/TextureArray.vert", "Shaders/TextureArray.frag")));
	Mesh_s city(Mesh(Model("Models/City/edited_city.obj")));
	UnorderedMaterial_s city_mat(UnorderedMaterial("Models/City/edited_city.obj"));

	for (int i = 0; i < 1; i++) {
		
	}

	Graphic g2;
	g2.load_model(city);
	g2.load_material(city_mat);
	g2.load_program(program);
	g2.set_uniform_all(default_program::basic_uniform_queue(scene, g2));
	
	g2.set_position(glm::vec3(0, -30, 0));
	
	scene.add_graphic(g2);
	
	
	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window(0.25f, 0.25f, 0.25f);
		frame.display_performance(180);
	
		scene.camera.handle_movements(frame.window, frame_time);
		
		scene.render();
	}

}
