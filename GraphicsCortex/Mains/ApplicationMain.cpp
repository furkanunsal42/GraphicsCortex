#include "GraphicsCortex.h"

int main() {

	Frame frame(1920 *2, 1080*2, "GraphicsCortex", 0, 0, true, false, true);
	Scene scene(frame);
	scene.camera->screen_width = 1920*2;
	scene.camera->screen_height = 1080*2;
	scene.camera->max_distance = 1000;

	Program_s program(Shader("Shaders/TextureArray.vert", "Shaders/TextureArray.frag"));
	Program_s solid_program = default_program::solid_program_s();

	{
		Model city_model("Models/circuit/nogaro.obj", 1.0f, Model::ALL);
		Model city_model_collision("Models/circuit/collision.obj", 1.0f, Model::COORD_XYZ);
		Model city_model_ground("Models/circuit/ground_physics.obj", 1.0f, Model::COORD_XYZ);
		Mesh_s city(city_model);
		
		PhysicsObject map_physics(create_geometry::triangle_mesh(city_model_collision.get_partial_data<physx::PxVec3>("111"), city_model_collision.index_data), PhysicsObject::STATIC, true);
		map_physics.make_drivable();
		scene.add_physics(map_physics);
		
		PhysicsObject map_ground(create_geometry::triangle_mesh(city_model_ground.get_partial_data<physx::PxVec3>("111"), city_model_ground.index_data), PhysicsObject::STATIC, true);
		map_ground.make_drivable();
		scene.add_physics(map_ground);


		UnorderedMaterial_s city_mat("Models/circuit/nogaro.obj");
		city_mat->texture_array.mipmap_bias = 0;
		city_mat->texture_array.generate_mipmap = false;
		city_mat->set_texture_size(1024, 1024);
		Graphic_s map;
		map->load_model(city);
		map->load_material(city_mat);
		map->load_program(program);
		map->set_uniform_all(default_program::basic_uniform_queue(*scene.camera.obj, map));

		map->set_position(glm::vec3(0, 0, 0));

		scene.add_graphic(map);

		DirectionalLight_s sunlight(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.4, 0.4, 0.4), solid_program);
		sunlight->set_uniform_upadte_queue(default_program::directional_light_default_uniform_queue(*sunlight.obj, 0));
		scene.add_light(sunlight);

		AmbiantLight_s ambinace(glm::vec3(0.3f, 0.3f, 0.3f), solid_program);
		ambinace->set_uniform_upadte_queue(default_program::ambiant_light_default_uniform_queue(*ambinace.obj, 0));
		scene.add_light(ambinace);
	}


	Vehicle vehicle_raw;
	Vehicle_s vehicle(vehicle_raw);

	{
		Model chassis_model("Models/teducar/teduCar.fbx", 0.006f, Model::COORD_XYZ | Model::TEX_COORD_XY | Model::NORMAL_XYZ);
		Model chassis_model_physics("Models/test2.obj", 1.0f, Model::COORD_XYZ | Model::TEX_COORD_XY | Model::NORMAL_XYZ);
		Model chassis_left_wheel_model("Models/porsche_wheel_left.obj", 1, Model::COORD_XYZ | Model::TEX_COORD_XY | Model::NORMAL_XYZ);
		Model chassis_right_wheel_model("Models/porsche_wheel_right.obj", 1, Model::COORD_XYZ | Model::TEX_COORD_XY | Model::NORMAL_XYZ);

		Mesh_s chassis(chassis_model);
		Mesh_s left_wheel(chassis_left_wheel_model);
		Mesh_s right_wheel(chassis_right_wheel_model);

		UnorderedMaterial_s tire_material_s(1);
		tire_material_s->set_texture("Images/cartextures/911_22_930_tire_BaseColor.png", 4, 0, UnorderedMaterial::COLOR);

		UnorderedMaterial_s chassis_material_s("Models/teducar/teduCar.fbx");

		vehicle->load_mesh_chassis_graphics(chassis);
		//vehicle->load_model_left_wheel_physics(chassis_left_wheel_model);
		//vehicle->load_model_right_wheel_physics(chassis_right_wheel_model);
		//vehicle->load_model_chassis_physics(chassis_model_physics);
		//vehicle->physics_representation.
		vehicle->load_material_chassis(chassis_material_s);
		//vehicle->load_material_left_wheel(tire_material_s);
		//vehicle->load_material_right_wheel(tire_material_s);
		//vehicle->load_program_all(program);
		vehicle->load_program_chassis(solid_program);

		vehicle->set_default_uniform_queue_all(*scene.camera.obj);
		vehicle->chassis->set_uniform("use_cube_map_reflection", 1);
		vehicle->chassis->set_uniform("cube_map_reflection_strength", 0.3f);

		vehicle->physics_representation.set_wheel_layout(3.4, -1.4f, 4.2, -0.0f);

		vehicle->physics_representation.compile();

		vehicle->set_position(glm::vec3(0, 2, 0));
		scene.add_object(vehicle);
	}

	PhysicsObject ground_plane(create_geometry::plane(0, 1, 0, 2.4f));
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
		vehicle->physics_representation.vehicle_control(frame.window);
		vehicle->sync_with_physics();

		// let camera follow the car
		glm::quat camera_rotation = vehicle->chassis->get_rotation();
		glm::vec3 camera_position = vehicle->chassis->get_position();
		camera_rotation = camera_rotation * glm::quat(glm::vec3(0, 3.14f, 0));
		camera_position += glm::vec3(0.0f, 0.7f, 0.0f);
		camera_position += camera_rotation * glm::vec3(0.0f, 0.5f, 1.0f);
		scene.camera->set_rotation(camera_rotation);
		scene.camera->set_position(camera_position);

		//vehicle->set_position(vehicle->chassis->get_position() + glm::vec3(0, 0, -0.1f));

		cube_map.bind();

		cube_map.texture_slot = 13;
		cube_map.bind();

		scene.render();
		cube_map.texture_slot = 11;

		cube_map.draw();
	}


}
