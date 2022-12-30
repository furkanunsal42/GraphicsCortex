#include "API/GraphicsCortex.h"

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, true, false, false);
	Scene scene;
	
	Model model("Models/City/edited_city.obj");
	UnorderedMaterial material_u("Models/City/edited_city.obj");

	Mesh mesh;
	mesh.load_model(model);

	Material material;
	material.set_color_texture("Images/GoldBlock.png", 4);
	material.bind();
	
	scene.camera.screen_width = 1920;
	scene.camera.screen_height = 1080;

	Shader shader("Shaders/TextureArray.vert", "Shaders/TextureArray.frag");
	Program renderer(shader.vertex_shader, shader.fragment_shader);

	//Program renderer = default_program::flatcolor_program();

	Graphic g;
	g.load_program(renderer);
	g.load_model(mesh);
	g.load_material(material_u);
	g.set_uniform_all(default_program::basic_uniform_queue(scene, g));

	float a = 0.5f;
	g.set_uniform("color", &a, &a, &a, &a);

	scene.add_graphic(g);
	
	AmbiantLight light(glm::vec3(0.1, 0.1, 0.1), renderer);
	light.set_uniform_upadte_queue(default_program::ambiant_light_default_uniform_queue(light, 0));
	scene.add_light(light);
	
	DirectionalLight d_light(glm::vec3(1, -1, 1), glm::vec3(1, 1, 1), renderer);
	d_light.set_uniform_upadte_queue(default_program::directional_light_default_uniform_queue(d_light, 0));
	scene.add_light(d_light);

	float var = 0;
	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		var += frame_time;
		a = glm::sin(var / 1000.0f);
		frame.display_performance(180);
		frame.clear_window(0.25f, 0.25f, 0.25f, 1.0f);
		scene.camera.handle_movements(frame.window, frame_time);
		scene.render(frame.window);
	}
}
