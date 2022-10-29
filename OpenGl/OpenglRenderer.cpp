#include "API/GraphicsCortex.h"

#include <iostream>

#include <PxPhysicsAPI.h>

int main() {
	int width = 1920, height = 1080;
	GLFWwindow* window = frame::create_window(width, height, "GraphicsCortex", 4, 0, true, false, false);
	Scene scene;
	Material material;
	Texture color_texture;
	Texture specular_map;
	Texture normal_map;
	color_texture.queue_image("Images/orange.png", 4, false);
	//color_texture.queue_image("Images/cartextures/911_22_paint_BaseColor.png", 4, true);
	//specular_map.queue_image("Images/cartextures/911_22_paint_Roughness.png", 4, true);
	//normal_map.queue_image("Images/cartextures/911_22_paint_Normal.png", 3, true);
	//color_texture.queue_image("Images/StoneTiles/tiles_color.jpg", 4, true);
	//specular_map.queue_image("Images/StoneTiles/tiles_specular.jpg", 4, true);
	//normal_map.queue_image("Images/StoneTiles/tiles_normal.jpg", 3, true);
	
	Material wheel;
	Texture wheel_color;
	wheel_color.queue_image("Images/cartextures/911_22_930_tire_BaseColor.png", 4, false);
	wheel_color.compress_image = true;
	wheel.color_map = &wheel_color;

	bool compression = true;
	color_texture.compress_image = compression;
	specular_map.compress_image = compression;
	normal_map.compress_image = compression;

	material.color_map = &color_texture;
	//material.specular_map = &specular_map;
	//material.normal_map = &normal_map;

	Program solid_program = default_program::solid_program();

	Camera cam;
	cam.screen_width = (float)width;
	cam.screen_height = (float)height;
	cam.position.z = 1.5f;
	cam.perspective = true;
	scene.camera = &cam;
	
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
	FrameBuffer frame_buffer(width, height, frame::multisample, false);
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

	Vehicle vehicle;
	vehicle.load_model_all(Model("Models/porsche_chassis.obj"), Model("Models/porsche_wheel_left.obj"), Model("Models/porsche_wheel_right.obj"), true, false);
	vehicle.physics_representation.set_wheel_layout(2.4, -1.8, 4.2, 0.4);
	vehicle.physics_representation.compile();
	vehicle.load_material_all(material, wheel, wheel);
	vehicle.load_program_all(solid_program);
	vehicle.set_default_uniform_queue_all(scene);
	vehicle.chassis.change_uniform_update_queue(uniform_update<int>("use_cube_map_reflection", 1));
	vehicle.chassis.change_uniform_update_queue(uniform_update<float>("cube_map_reflection_strength", 0.55));
	scene.add_object(vehicle);

	vehicle.set_position(glm::vec3(0, 0, -5));

	PhysicsObject plane(create_geometry::plane(0, 1, 0, 4));
	plane.make_drivable();
	scene.add_physics(plane);

	float t = 0;
	while (!glfwWindowShouldClose(window)){
		double frame_time = frame::get_interval_ms();
		PhysicsScene::get().simulation_step_start(frame_time / 1000.0f);

		vehicle.sync_with_physics();
		vehicle.physics_representation.vehicle_control(window);

		frame_buffer.bind();
		
		glfwPollEvents();
		frame::clear_window(0, 0, 0, 1);
		frame::display_performance(180);
		
		scene.camera->frame_time_ms = frame_time;
		//scene.camera->handle_movements(window);

		cube_map.draw();

		cube_map.texture_slot = 13;
		cube_map.bind();
		scene.render(window);
		cube_map.texture_slot = 11;
		
		frame_buffer.unbind();

		frame_buffer.render(FrameBuffer::COLOR_TEXTURE);

		glfwSwapBuffers(window);
		PhysicsScene::get().simulation_step_finish();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
