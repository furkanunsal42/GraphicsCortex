#include "GraphicsCortex.h"

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, true, false, true);
	Scene scene(frame);
	scene.camera->max_distance = 1000.0f;

	RenderPipeline_MultiTextured pipeline = default_program::multitextured_pipeline(frame);
	pipeline.cameras["default_camera"] = scene.camera;
	pipeline.activate_camera("default_camera");

	{
		Mesh_s city(Model("Models/City/edited_city2.obj", 4.0f, Model::ALL));
		UnorderedMaterial_s city_mat("Models/City/edited_city2.obj");
		//Mesh_s city(Model("Models/City2/city2.obj", 1.0f, Model::COORD_XYZ | Model::NORMAL_XYZ | Model::TEX_COORD_XY | Model::TEX_COORD_Z_DIFFUSE | Model::TEX_COORD_Z_NORMAL | Model::TEX_COORD_Z_SPECULAR));
		//UnorderedMaterial_s city_mat("Models/City2/city2.obj");
		city_mat->texture_array.mipmap_bias = 0;
		city_mat->texture_array.generate_mipmap = false;
		city_mat->set_texture_size(1024, 1024);
		Graphic_s map;
		map->load_model(city);
		map->load_material(city_mat);
		map->set_position(glm::vec3(0, 0, 0));

		pipeline.multitextued_graphics["map"] = map;

		DirectionalLight_s sunlight(glm::vec3(0.0f, 200.0f, 0.0f), glm::vec3(1, -1.0f, 1), glm::vec3(0.8, 0.8, 0.8));
		AmbiantLight_s ambiance(glm::vec3(0.7f, 0.7f, 0.7f));

		pipeline.directional_ligths["sun"] = sunlight;
		pipeline.ambiant_ligths["ambiance"] = ambiance;
	}

	Vehicle_s vehicle;

	{
		Model chassis_model("Models/porsche_chassis.obj", 1, Model::COORD_XYZ | Model::NORMAL_XYZ | Model::TEX_COORD_XY);
		Model chassis_left_wheel_model("Models/porsche_wheel_left.obj", 1);
		Model chassis_right_wheel_model("Models/porsche_wheel_right.obj", 1);

		Mesh_s chassis(chassis_model);
		Mesh_s left_wheel(chassis_left_wheel_model);
		Mesh_s right_wheel(chassis_right_wheel_model);

		UnorderedMaterial_s tire_material_s(1);
		tire_material_s->set_texture("Images/cartextures/911_22_930_tire_BaseColor.png", 4, 0, UnorderedMaterial::COLOR);

		UnorderedMaterial_s chassis_material_s(1);
		chassis_material_s->set_texture("Images/full_black.png", 4, 0, UnorderedMaterial::COLOR);

		vehicle->load_mesh_all_graphics(chassis, left_wheel, right_wheel);
		vehicle->load_model_all_physics(chassis_model, chassis_left_wheel_model, chassis_right_wheel_model);
		vehicle->load_material_chassis(chassis_material_s);
		vehicle->load_material_left_wheel(tire_material_s);
		vehicle->load_material_right_wheel(tire_material_s);

		vehicle->chassis->set_reflection_strength(0.15f);

		pipeline.graphics["vehicle_chassis"] = vehicle->chassis;
		for (int i = 0; i < 4; i++)
			pipeline.graphics["wheel" + std::to_string(i)] = vehicle->wheels[i];

		vehicle->physics_representation.set_wheel_layout(2.4, -1.5, 4.2, 0.4);

		vehicle->physics_representation.compile();
		scene.add_object(vehicle);

		vehicle->set_position(glm::vec3(0, 2, 0));
		
	}

	PhysicsObject ground_plane(create_geometry::plane(0, 1, 0, 0));
	{
		ground_plane.make_drivable();
		scene.add_physics(ground_plane);
	}

	Program_s cubemap_program(default_program::cubemap_program());
	CubeMapTexture cube_map;
	cube_map.set_program(cubemap_program);
	cube_map.camera = scene.camera.obj.get();
	cube_map.set_update_queue(default_program::cubemap_default_uniform_queue(cube_map));
	cube_map.face_texture_filepaths[RIGHT] = "Images/CubeMap/Sky/px.jpg";
	cube_map.face_texture_filepaths[LEFT] = "Images/CubeMap/Sky/nx.jpg";
	cube_map.face_texture_filepaths[TOP] = "Images/CubeMap/Sky/py.jpg";
	cube_map.face_texture_filepaths[BOTTOM] = "Images/CubeMap/Sky/ny.jpg";
	cube_map.face_texture_filepaths[FRONT] = "Images/CubeMap/Sky/pz.jpg";
	cube_map.face_texture_filepaths[BACK] = "Images/CubeMap/Sky/nz.jpg";

	cube_map.read_queue(3);
	cube_map.load_queue(true);

	while (frame.is_running()) {

		double frame_time = frame.handle_window();
		PhysicsScene::get().simulate_step(frame_time / 1000.0f);
		frame.clear_window(0.25f, 0.25f, 0.25f);
		frame.display_performance(180);

		scene.camera->handle_movements(frame.window, frame_time);
		//vehicle->physics_representation.vehicle_control(frame.window);

		vehicle->sync_with_physics();

		// let camera follow the car
		glm::quat camera_rotation = vehicle->chassis->get_rotation();
		glm::vec3 camera_position = vehicle->chassis->get_position();
		camera_rotation = camera_rotation * glm::quat(glm::vec3(0, 3.14f, 0));
		camera_position += glm::vec3(0.0f, 0.5f, 0.0f);
		camera_position += camera_rotation * glm::vec3(0.0f, 0.5f, 1.0f);
		//scene.camera->set_rotation(camera_rotation);
		//scene.camera->set_position(camera_position);

		cube_map.bind();

		cube_map.texture_slot = 13;
		cube_map.bind();

		pipeline.render_sequence(frame);

		//scene.render();
		cube_map.texture_slot = 11;

		cube_map.draw();
	}


}
