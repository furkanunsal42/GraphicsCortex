#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "BindlessMaterials App", 0, 0, true, true, false, Frame::CallbackLevel::LOW, false);
	Scene scene(frame);
	scene.camera->fov = 110;
	scene.camera->max_distance = 1000;

	std::shared_ptr<UniformBuffer> uniform_buffer = std::make_shared<UniformBuffer>();
	uniform_buffer->bind();
	uniform_buffer->push_variable<glm::mat4>();
	uniform_buffer->push_variable<glm::mat4>();
	uniform_buffer->bind(0);

	std::shared_ptr<Program> bindless_program = std::make_shared<Program>(Shader("../GraphicsCortex/Custom Shaders/Texture.vert", "../GraphicsCortex/Custom Shaders/Texture.frag"));
	bindless_program->attach_uniform_buffer("camera_matricies", uniform_buffer);

	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<BindlessMaterial> material;
	{
		Model dragon_model = AssetImporter::generate_model("../GraphicsCortex/Models/dragon_new/dragon_new.fbx", 0.1, Model::COORD_XYZ | Model::TEX_COORD_XY | Model::TEX_COORD_Z_DIFFUSE);
		mesh = std::make_shared<Mesh>(dragon_model);
		material = AssetImporter::generate_material("../GraphicsCortex/Models/dragon_new/dragon_new.fbx", bindless_program);
	}
	
	AssetImporter::clear_ram_all();
	std::shared_ptr<Graphic> graphic = std::make_shared<Graphic>(mesh, material);
	graphic->rotation = glm::vec3(-glm::pi<float>() / 2, 0, 0);
	scene.add(graphic);

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window();
		frame.display_performance(180);

		scene.render();

		scene.camera->update_matrixes();

		glm::mat4 view = scene.camera->view_matrix;

		uniform_buffer->set_data(0, 0, 64, glm::value_ptr(scene.camera->view_matrix));
		uniform_buffer->set_data(64, 0, 64, glm::value_ptr(scene.camera->projection_matrix));

		scene.camera->handle_movements(frame.window, deltatime);
	}
}