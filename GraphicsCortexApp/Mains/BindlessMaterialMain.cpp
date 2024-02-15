#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "BindlessMaterials App", 0, 0, true, true, false, Frame::CallbackLevel::LOW, false);
	Scene scene(frame);
	scene.camera->fov = 110;
	scene.camera->max_distance = 1000;

	std::shared_ptr<UniformBuffer> uniform_buffer = std::make_shared<UniformBuffer>();
	uniform_buffer->push_variable<glm::mat4>();
	uniform_buffer->push_variable<glm::mat4>();

	std::shared_ptr<Program> bindless_program = std::make_shared<Program>(Shader("../GraphicsCortex/Source/GLSL/TextureArray.vert", "../GraphicsCortex/Source/GLSL/TextureArray.frag"));

	//bindless_program->attach_uniform_buffer("camera_matricies", uniform_buffer);

	std::shared_ptr<Mesh> circuit_mesh;
	std::shared_ptr<BindlessMaterial> circuit_material;
	{
		Model circuit_model("../GraphicsCortex/Models/circuit/nogaro.obj", 1, Model::ALL);
		circuit_mesh = std::make_shared<Mesh>(circuit_model);
		circuit_material = AssetImporter::generate_material("../GraphicsCortex/Models/circuit/nogaro.obj", bindless_program);
	}
	
	AssetImporter::clear_ram_all();
	std::shared_ptr<Graphic> circuit = std::make_shared<Graphic>(circuit_mesh, circuit_material);
	//graphic->rotation = glm::vec3(-glm::pi<float>() / 2, 0, 0);d

	std::shared_ptr<Mesh> dragon_mesh;
	std::shared_ptr<BindlessMaterial> dragon_material;
	{
		Model dragon_model("../GraphicsCortex/Models/dragon_new/dragon_new.fbx", 1, Model::ALL);
		dragon_mesh = std::make_shared<Mesh>(dragon_model);
		dragon_material = AssetImporter::generate_material("../GraphicsCortex/Models/dragon_new/dragon_new.fbx", bindless_program);
	}

	AssetImporter::clear_ram_all();
	std::shared_ptr<Graphic> dragon = std::make_shared<Graphic>(dragon_mesh, dragon_material);
	dragon->rotation = glm::vec3(-glm::pi<float>() / 2, 0, 0);

	scene.add(dragon);
	scene.add(circuit);

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window();
		frame.display_performance(180);

		scene.render();

		scene.camera->update_matrixes();

		uniform_buffer->set_data(0, glm::value_ptr(scene.camera->view_matrix));
		uniform_buffer->set_data(1, glm::value_ptr(scene.camera->projection_matrix));

		scene.camera->handle_movements(frame.window, deltatime);
	}
}