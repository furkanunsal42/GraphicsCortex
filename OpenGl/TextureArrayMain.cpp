#include "API/GraphicsCortex.h"

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, true, false, false);
	Scene scene;
	
	Model model;
	UnorderedMaterial material_u = model.load_model("Models/City/edited_city.obj", 1, Model::ALL);
	//material.set_texture_size(1024, 1024);
	//material_u.bind();

	Material material;
	material.set_color_texture("Images/GoldBlock.png", 4);
	material.bind();
	
	//Image image = material.texture_array.save(-1);
	//image.save_to_disc("texture_array_save.png");
	
	Camera cam;
	cam.screen_width = 1920;
	cam.screen_height = 1080;
	cam.position.z = 1.5f;
	cam.max_distance = 1000.0;
	cam.perspective = true;
	scene.camera = &cam;

	//Shader shader("Shaders/TextureArray.vert", "Shaders/TextureArray.frag");
	//Program renderer(shader.vertex_shader, shader.fragment_shader);

	Program renderer = default_program::solid_program();

	Graphic g;
	g.load_program(renderer);
	g.load_model(model);
	g.load_material(material_u);
	//g.set_uniform_update_queue(default_program::basic_uniform_queue(scene, g));
	g.set_uniform_update_queue(default_program::solid_default_uniform_queue(scene, g));

	//g.set_position(glm::vec3(0, 0, -6));
	//g.set_rotation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0, 0)));

	scene.add_graphic(g);
	
	AmbiantLight light(glm::vec3(0.1, 0.1, 0.1), renderer);
	light.set_uniform_upadte_queue(default_program::ambiant_light_default_uniform_queue(light, 0));
	scene.add_light(light);
	
	DirectionalLight d_light(glm::vec3(1, -1, 1), glm::vec3(1, 1, 1), renderer);
	d_light.set_uniform_upadte_queue(default_program::directional_light_default_uniform_queue(d_light, 0));
	scene.add_light(d_light);

	
	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window(0.25f, 0.25f, 0.25f, 1.0f);
		scene.camera->handle_movements(frame.window, frame_time);
		scene.render(frame.window);
	}
}
