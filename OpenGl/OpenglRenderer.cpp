#include "API/GraphicsCortex.h"

#include <iostream>

int main() {
	int width = 1024, height = 768;
	GLFWwindow* window = frame::create_window(width, height, "My Window", 4, 0, true, false, true);
	Scene scene;
	Material material;
	Texture color_texture;
	Texture specular_map;
	Texture normal_map;
	color_texture.queue_image("Images/GoldBlock.png", 4, true);
	//color_texture.queue_image("Images/Bricks/brickcolor.jpg", 4, true);
	//specular_map.queue_image("Images/Bricks/brickreflection.jpg", 4, true);
	//normal_map.queue_image("Images/Bricks/bricknormal.png", 3, true);
	//color_texture.queue_image("Images/StoneTiles/tiles_color.jpg", 4, true);
	//specular_map.queue_image("Images/StoneTiles/tiles_specular.jpg", 4, true);
	//normal_map.queue_image("Images/StoneTiles/tiles_normal.jpg", 3, true);
	
	bool compression = false;
	color_texture.compress_image = compression;
	//specular_map.compress_image = compression;
	//normal_map.compress_image = compression;

	material.color_map = &color_texture;
	//material.specular_map = &specular_map;
	//material.normal_map = &normal_map;
	
	//Shader normal_shader("Shaders/Solid.vert", "Shaders/solid.geom", "Shaders/NormalTest.frag");
	//Program soild_program(normal_shader.vertex_shader, normal_shader.geometry_shader, normal_shader.fragment_shader);

	Program solid_program = default_program::solid_program();
	Program flat_program = default_program::flatcolor_program();

	Graphic cube = default_geometry::cube(material, solid_program, glm::vec3(1.0f));
	cube.load_model("Models/dragon.obj");
	scene.meshes.push_back(&cube);

	Camera cam;
	cam.screen_width = (float)width;
	cam.screen_height = (float)height;
	cam.position.z = 1.5f;
	cam.perspective = true;
	scene.camera = &cam;
	
	cube.add_uniform_update_queue(new uniform_update<int>("use_cube_map_reflection", 1));
	cube.add_uniform_update_queue(new dynamic_uniform_update<glm::mat4>("model", &(cube.model_matrix)));
	cube.add_uniform_update_queue(new uniform_update<int>("cube_map", 13));
	cube.add_uniform_update_queue(new uniform_update<int>("use_cube_map_reflection", 1));
	cube.add_uniform_update_queue(new dynamic_uniform_update<glm::mat4>("view", &(cam.view_matrix)));
	cube.add_uniform_update_queue(new dynamic_uniform_update<glm::mat4>("projection", &(cam.projection_matrix)));
	cube.add_uniform_update_queue(new dynamic_uniform_update<float>("camera_coords", &(cam.position.x), &(cam.position.y), &(cam.position.z))); // glm vec3
	cube.add_uniform_update_queue(new uniform_update<int>("use_color_map", (int)(cube.material->color_map != nullptr)));
	cube.add_uniform_update_queue(new uniform_update<int>("use_specular_map", (int)(cube.material->specular_map != nullptr)));
	cube.add_uniform_update_queue(new uniform_update<int>("use_normal_map", (int)(cube.material->normal_map != nullptr)));
	cube.add_uniform_update_queue(new uniform_update<int>("color_map_slot", cube.material->color_map_slot));
	cube.add_uniform_update_queue(new uniform_update<int>("specular_map_slot", cube.material->specular_map_slot));
	cube.add_uniform_update_queue(new uniform_update<int>("normal_map_slot", cube.material->normal_map_slot));

	AmbiantLight ambiant(glm::vec3(0.1f, 0.1f, 0.1f), solid_program);
	DirectionalLight directional(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), solid_program);
	PointLight point(glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3(3.0f, 3.0f, 3.0f), 0.5f, 0.5f, 0.0f, solid_program);
	SpotLight spot(glm::vec3(-2.0f, -1.0f, -2.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.3f, 0.2f, 0.0f, 30, solid_program);
	
	scene.lights.push_back(&ambiant);
	scene.lights.push_back(&directional);
	//scene.lights.push_back(&point);
	//scene.lights.push_back(&spot);

	Program framebuffer_program = default_program::framebuffer_program();
	FrameBuffer frame_buffer(width, height, frame::multisample);
	frame_buffer.program = &framebuffer_program;
	scene.frame_buffer = &frame_buffer;
	
	Program cubemap_program = default_program::cubemap_program();
	CubeMapTexture cube_map;
	cube_map.program = &cubemap_program;
	cube_map.camera = scene.camera;
	cube_map.face_texture_filepaths[RIGHT] = "Images/CubeMap/Sky/px.jpg";
	cube_map.face_texture_filepaths[LEFT] = "Images/CubeMap/Sky/nx.jpg";
	cube_map.face_texture_filepaths[TOP] = "Images/CubeMap/Sky/py.jpg";
	cube_map.face_texture_filepaths[BOTTOM] = "Images/CubeMap/Sky/ny.jpg";
	cube_map.face_texture_filepaths[FRONT] = "Images/CubeMap/Sky/pz.jpg";
	cube_map.face_texture_filepaths[BACK] = "Images/CubeMap/Sky/nz.jpg";
	/*
	cube_map.face_texture_filepaths[RIGHT] = "Images/CubeMap/Street/px.png";
	cube_map.face_texture_filepaths[LEFT] = "Images/CubeMap/Street/nx.png";
	cube_map.face_texture_filepaths[TOP] = "Images/CubeMap/Street/py.png";
	cube_map.face_texture_filepaths[BOTTOM] = "Images/CubeMap/Street/ny.png";
	cube_map.face_texture_filepaths[FRONT] = "Images/CubeMap/Street/pz.png";
	cube_map.face_texture_filepaths[BACK] = "Images/CubeMap/Street/nz.png";
	*/

	cube_map.read_queue(3);
	cube_map.load_queue(true);
	
	float t = 0;
	int i = 0;
	while (!glfwWindowShouldClose(window)){
		
		//i++; 
		if (i == 2000)
			return 0;

		frame_buffer.bind();
		
		glfwPollEvents();
		frame::clear_window(0, 0, 0, 1);
		frame::display_performance(180);
		
		double frame_time = frame::get_interval_ms();
		scene.camera->frame_time_ms = frame_time;
		scene.camera->handle_movements(window);
		
		cube_map.draw();

		//cube.position.x += 0.01f;
		cube.rotation.y += 0.04f * frame_time;
		t += 0.001f * frame_time;
		//point.position.y = 5*glm::cos(t);
		
		cube_map.texture_slot = 13;
		cube_map.bind();
		scene.render(window);
		cube_map.texture_slot = 11;
		
		frame_buffer.unbind();
		frame_buffer.render();

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
