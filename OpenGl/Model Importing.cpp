#include "API/GraphicsCortex.h"

int main() {
	int width = 1920, height = 1080;
	Frame frame(width, height, "GraphicsCortex", 4, 0, true, false, false);

	Scene scene;
	Material material;
	material.set_color_texture("Models/thinker/textures/Rodin_Thinker_diffuse.jpg", 4);
	material.set_normal_texture("Models/thinker/textures/Rodin_Thinker_normal.png", 4);
	material.set_specular_texture("Models/thinker/textures/Rodin_Thinker_gloss.jpg", 4);

	Program solid_program = default_program::solid_program();

	Camera cam;
	cam.screen_width = (float)width;
	cam.screen_height = (float)height;
	cam.position.z = 1.5f;
	cam.max_distance = 10000.0;
	cam.perspective = true; 
	scene.camera = &cam;

	Graphic g;
	g.load_model(Model("Models/thinker/Rodin_Thinker.obj"));
	g.load_program(solid_program);
	g.load_material(material);
	g.set_uniform_update_queue(default_program::solid_default_uniform_queue(scene, g));
	g.change_uniform_update_queue(uniform_update<int>("use_cube_map_reflection", 1));
	g.change_uniform_update_queue(uniform_update<float>("cube_map_reflection_strength", 0.15));
	scene.add_graphic(g);
	g.set_position(glm::vec3(0, 0, -3));

	AmbiantLight ambiant(glm::vec3(0.1f, 0.1f, 0.1f), solid_program);
	DirectionalLight directional(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), solid_program);
	PointLight point(glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3(3.0f, 3.0f, 3.0f), 0.5f, 0.5f, 0.0f, solid_program);
	SpotLight spot(glm::vec3(-2.0f, -1.0f, -2.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.3f, 0.2f, 0.0f, 30, solid_program);

	ambiant.set_uniform_upadte_queue(default_program::ambiant_light_default_uniform_queue(ambiant, 0));
	directional.set_uniform_upadte_queue(default_program::directional_light_default_uniform_queue(directional, 0));
	point.set_uniform_upadte_queue(default_program::point_light_default_uniform_queue(point, 0));
	spot.set_uniform_upadte_queue(default_program::spot_light_default_uniform_queue(spot, 0));

	scene.add_light(ambiant);
	scene.add_light(directional);
	//scene.lights.push_back(&point);
	//scene.lights.push_back(&spot);

	Program framebuffer_program = default_program::framebuffer_program();
	FrameBuffer frame_buffer(width, height, 0, true);
	frame_buffer.program = &framebuffer_program;
	scene.frame_buffer = &frame_buffer;

	Program cubemap_program = default_program::cubemap_program();
	CubeMapTexture cube_map;
	cube_map.set_program(cubemap_program);
	cube_map.camera = scene.camera;
	cube_map.set_update_queue(default_program::cubemap_default_uniform_queue(cube_map));
	cube_map.face_texture_filepaths[RIGHT] = "Images/CubeMap/Sky/px.jpg";
	cube_map.face_texture_filepaths[LEFT] = "Images/CubeMap/Sky/nx.jpg";
	cube_map.face_texture_filepaths[TOP] = "Images/CubeMap/Sky/py.jpg";
	cube_map.face_texture_filepaths[BOTTOM] = "Images/CubeMap/Sky/ny.jpg";
	cube_map.face_texture_filepaths[FRONT] = "Images/CubeMap/Sky/pz.jpg";
	cube_map.face_texture_filepaths[BACK] = "Images/CubeMap/Sky/nz.jpg";

	cube_map.read_queue(3);
	cube_map.load_queue(true);

	float t = 0;

	while (frame.is_running()) {
		double frame_time = frame.handle_window();

		frame_buffer.bind();

		frame.clear_window(0, 0, 0, 1);
		frame.display_performance(180);

		scene.camera->handle_movements(frame.window);

		cube_map.draw();

		cube_map.texture_slot = 13;
		cube_map.bind();
		scene.render(frame.window);
		cube_map.texture_slot = 11;

		frame_buffer.unbind();

		frame_buffer.render(FrameBuffer::COLOR_TEXTURE);
	}

	return 0;
}
