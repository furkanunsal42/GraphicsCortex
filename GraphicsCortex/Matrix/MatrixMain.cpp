#include "GraphicsCortex.h"

int main() {
	
	Frame frame(1920, 1080, "Matrix", 0, 0, true, true, true, false);
	Scene scene(frame);

	Font_s matrix_font;
	

	Program_s default_program = default_program::solid_program_s();
	
	Shader matrix_shader("Shaders/Solid.vert", "Matrix/Shaders/Matrix.frag");
	Program_s matrix_program(matrix_shader.vertex_shader, matrix_shader.fragment_shader);
	
	UnorderedMaterial_s cube_material(1);
	cube_material->set_texture("Images/cobble.png", 4, 0);
	
	Graphic_s cube(default_geometry::cube());
	cube->load_material(cube_material);
	cube->load_program(matrix_program);
	cube->set_uniform_all(default_program::solid_default_uniform_queue(*scene.camera.obj, cube));
	scene.add_graphic(cube);

	AmbiantLight_s ambiant(glm::vec3(0.4, 0.4, 0.4), matrix_program);
	ambiant->set_uniform_upadte_queue(default_program::ambiant_light_default_uniform_queue(*ambiant.obj, 0));
	scene.add_light(ambiant);

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window(0, 0, 0, 0);
		frame.display_performance(180);

		scene.camera->handle_movements(frame.window, deltatime);

		scene.render();

	}

}