#include "GraphicsCortex.h"

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, true, false, true, Frame::CallbackLevel::MEDIUM, false);
	Scene scene(frame);
	scene.camera->screen_width = frame.window_width;
	scene.camera->screen_height = frame.window_height;
	scene.camera->max_distance = 10000;

	std::shared_ptr<Program> program = std::make_shared<Program>(Shader("../GraphicsCortex/Source/GLSL/TextureArray.vert", "../GraphicsCortex/Source/GLSL/TextureArray.frag"));
	std::shared_ptr<Program> solid_program = default_program::solid_program_s();

	{
		Model city_model("../GraphicsCortex/Models/circuit/nogaro.obj", 1.0f, Model::ALL);
		Model city_model_collision("../GraphicsCortex/Models/circuit/collision.obj", 1.0f, Model::COORD_XYZ);
		Model city_model_ground("../GraphicsCortex/Models/circuit/ground_physics.obj", 1.0f, Model::COORD_XYZ);
		std::shared_ptr<Mesh> city = std::make_shared<Mesh>(city_model);

		PhysicsObject map_physics(create_geometry::triangle_mesh(city_model_collision.get_partial_data<physx::PxVec3>("111"), city_model_collision.submodels[0].index_data), PhysicsObject::STATIC, true);
		map_physics.make_drivable();
		scene.add_physics(map_physics);

		PhysicsObject map_ground(create_geometry::triangle_mesh(city_model_ground.get_partial_data<physx::PxVec3>("111"), city_model_ground.submodels[0].index_data), PhysicsObject::STATIC, true);
		map_ground.make_drivable();
		scene.add_physics(map_ground);

		std::shared_ptr<Texture2D> default_texture = std::make_shared<Texture2D>("../GraphicsCortex/Images/orange.png", Texture2D::ColorTextureFormat::RGBA8, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE);
		std::shared_ptr<BindlessMaterial> city_mat = AssetImporter::generate_material("../GraphicsCortex/Models/circuit/nogaro.obj", program);
		//city_mat->add_texture("default_texture", default_texture);

		//city_mat->mipmap_bias = 0;
		//city_mat->texture_array.generate_mipmap = false;
		//city_mat->set_texture_size(512, 512);
		std::shared_ptr<Graphic> map = std::make_shared<Graphic>();
		map->set_mesh(city);
		map->set_material(city_mat);
		map->position = glm::vec3(0, 0, 0);
		scene.add(map);

		std::shared_ptr<DirectionalLight> sunlight = std::make_shared<DirectionalLight>(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.4, 0.4, 0.4));
		scene.add(sunlight);

		std::shared_ptr<AmbiantLight> ambinace = std::make_shared<AmbiantLight>(glm::vec3(0.3f, 0.3f, 0.3f));
		scene.add(ambinace);
	}

	{
		std::shared_ptr<Mesh> dragon_mesh;
		std::shared_ptr<BindlessMaterial> dragon_material;
		{
			Model dragon_model("../GraphicsCortex/Models/dragon_new/dragon_new.fbx", 1, Model::ALL);
			dragon_mesh = std::make_shared<Mesh>(dragon_model);
			dragon_material = AssetImporter::generate_material("../GraphicsCortex/Models/dragon_new/dragon_new.fbx", program);
		}

		AssetImporter::clear_ram_all();
		std::shared_ptr<Graphic> dragon = std::make_shared<Graphic>(dragon_mesh, dragon_material);
		dragon->rotation = glm::vec3(-glm::pi<float>() / 2, glm::pi<float>() / 2, 0);
		dragon->position = glm::vec3(-90, -1, -15);

		scene.add(dragon);
	}

	Vehicle vehicle_raw;
	std::shared_ptr<Vehicle> vehicle = std::make_shared<Vehicle>(vehicle_raw);

	{
		Model chassis_model("../GraphicsCortex/Models/teducar/teduCar.fbx", 0.006f, Model::COORD_XYZ | Model::TEX_COORD_XY | Model::NORMAL_XYZ);
		Model chassis_model_physics("../GraphicsCortex/Models/test2.obj", 1.0f, Model::COORD_XYZ | Model::TEX_COORD_XY | Model::NORMAL_XYZ);
		Model chassis_left_wheel_model("../GraphicsCortex/Models/porsche_wheel_left.obj", 1, Model::COORD_XYZ | Model::TEX_COORD_XY | Model::NORMAL_XYZ);
		Model chassis_right_wheel_model("../GraphicsCortex/Models/porsche_wheel_right.obj", 1, Model::COORD_XYZ | Model::TEX_COORD_XY | Model::NORMAL_XYZ);

		std::shared_ptr<Mesh> chassis = std::make_shared<Mesh>(chassis_model);
		std::shared_ptr<Mesh> left_wheel = std::make_shared<Mesh>(chassis_left_wheel_model);
		std::shared_ptr<Mesh> right_wheel = std::make_shared<Mesh>(chassis_right_wheel_model);

		std::shared_ptr<BindlessMaterial> tire_material_s = std::make_shared<BindlessMaterial>(program);
		//tire_material_s->add_texture("color_texture", std::make_shared<Texture2D>("../GraphicsCortex/Images/cartextures/911_22_930_tire_BaseColor.png", Texture2D::ColorTextureFormat::RGBA8, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE));

		std::shared_ptr<BindlessMaterial> chassis_material_s = AssetImporter::generate_material("../GraphicsCortex/Models/teducar/teduCar.fbx", program);

		vehicle->set_mesh_chassis_graphics(chassis);
		vehicle->set_model_chassis_physics(chassis_model);
		vehicle->set_material_chassis(chassis_material_s);

		vehicle->chassis->material->program->update_uniform("use_cube_map_reflection", 1);
		vehicle->chassis->material->program->update_uniform("cube_map_reflection_strength", 0.3f);

		vehicle->physics_representation.set_wheel_layout(2.6f, -3.4f, 3.8, -0.0f);
		//vehicle->physics_representation.chassisDims = physx::PxVec3(1.2f, 1.3f, 2.6f);
		vehicle->physics_representation.compile();

		vehicle->set_position(glm::vec3(0, 2, 0));
		vehicle->set_rotation(glm::vec3(0, -glm::pi<float>()/2 - 0.2, 0));
		scene.add(vehicle);
	}

	PhysicsObject ground_plane(create_geometry::plane(0, 1, 0, 2.4f));
	{
		ground_plane.make_drivable();
		scene.add_physics(ground_plane);
	}
	AssetImporter::clear_ram_all();

	std::shared_ptr<SkyBox> skybox = std::make_shared<SkyBox>();
	skybox->cubemap = std::make_shared<TextureCubeMap>(1024, TextureCubeMap::ColorTextureFormat::RGBA8, 1, 0);
	skybox->cubemap->load_data_async(TextureCubeMap::Face::RIGHT, "../GraphicsCortex/Images/CubeMap/Sky/px.jpg", TextureCubeMap::ColorFormat::RGBA, TextureCubeMap::Type::UNSIGNED_BYTE, 0);
	skybox->cubemap->load_data_async(TextureCubeMap::Face::LEFT, "../GraphicsCortex/Images/CubeMap/Sky/nx.jpg", TextureCubeMap::ColorFormat::RGBA, TextureCubeMap::Type::UNSIGNED_BYTE, 0);
	skybox->cubemap->load_data_async(TextureCubeMap::Face::UP, "../GraphicsCortex/Images/CubeMap/Sky/py.jpg", TextureCubeMap::ColorFormat::RGBA, TextureCubeMap::Type::UNSIGNED_BYTE, 0);
	skybox->cubemap->load_data_async(TextureCubeMap::Face::DOWN, "../GraphicsCortex/Images/CubeMap/Sky/ny.jpg", TextureCubeMap::ColorFormat::RGBA, TextureCubeMap::Type::UNSIGNED_BYTE, 0);
	skybox->cubemap->load_data_async(TextureCubeMap::Face::FRONT, "../GraphicsCortex/Images/CubeMap/Sky/pz.jpg", TextureCubeMap::ColorFormat::RGBA, TextureCubeMap::Type::UNSIGNED_BYTE, 0);
	skybox->cubemap->load_data_async(TextureCubeMap::Face::BACK, "../GraphicsCortex/Images/CubeMap/Sky/nz.jpg", TextureCubeMap::ColorFormat::RGBA, TextureCubeMap::Type::UNSIGNED_BYTE, 0);
	scene.set_skybox(skybox);

	while (frame.is_running()) {

		double frame_time = frame.handle_window();
		PhysicsScene::get().simulate_step(frame_time / 1000.0f);
		frame.clear_window(0.25f, 0.25f, 0.25f);
		frame.display_performance(180);

		//scene.camera->handle_movements(frame.window, frame_time);
		vehicle->physics_representation.vehicle_control(frame.window);
		vehicle->sync_with_physics();

		// let camera follow the car
		glm::quat camera_rotation = vehicle->chassis->rotation;
		glm::vec3 camera_position = vehicle->chassis->position;
		camera_rotation = camera_rotation * glm::quat(glm::vec3(0, 3.14f, 0));
		camera_position += glm::vec3(0.0f, 0.7f, 0.0f);
		camera_position += camera_rotation * glm::vec3(0.0f, 0.5f, 1.0f);
		scene.camera->set_rotation(camera_rotation);
		scene.camera->set_position(camera_position);

		scene.render();
	}


}
