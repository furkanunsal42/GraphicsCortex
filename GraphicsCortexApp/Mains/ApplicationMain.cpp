#include "GraphicsCortex.h"

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, true, true, true, Frame::CallbackLevel::MEDIUM, false);
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

		std::shared_ptr<BindlessMaterial> city_mat = AssetImporter::generate_material("../GraphicsCortex/Models/circuit/nogaro.obj", program);
		std::shared_ptr<Texture2D> default_texture = std::make_shared<Texture2D>("../GraphicsCortex/Images/orange.png", Texture2D::ColorTextureFormat::RGBA8, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE);
		//city_mat->add_texture("default_texture", default_texture);
		
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


	while (frame.is_running()) {

		double frame_time = frame.handle_window();
		PhysicsScene::get().simulate_step(frame_time / 1000.0f);
		frame.clear_window(0.25f, 0.25f, 0.25f);
		frame.display_performance(180);

		scene.camera->handle_movements(frame.window, frame_time);

		// let camera follow the car
		scene.render();
	}


}
