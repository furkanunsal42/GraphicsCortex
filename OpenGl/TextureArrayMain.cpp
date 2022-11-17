#include "API/GraphicsCortex.h"

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, true, false, true);
	Scene scene;

	Model model;
	UnorderedMaterial material = model.load_model("Models/dragon_new/dragon_new.fbx");
	material.bind();

	Image result = material.texture_array.save(-1);
	result.save_to_disc("texture_array_save.png");
	
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
	g.load_model(model);
	g.set_uniform_update_queue(default_program::basic_uniform_queue(scene, g));
	g.add_uniform_update_queue(uniform_update<int>("texture_array", 0));
	g.add_uniform_update_queue(uniform_update<int>("array_index", 1));

	g.set_position(glm::vec3(0, 0, -6));
	g.set_rotation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0, 0)));

	scene.add_graphic(g);
	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window(0.25f, 0.25f, 0.25f, 1.0f);
		scene.camera->handle_movements(frame.window, frame_time);
		scene.render(frame.window);
	}
}
