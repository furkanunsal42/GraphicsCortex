#include "GraphicsCortex.h"
#include "Framebuffer.h"

int main() {
	Frame frame(1920, 1080, "FrameBuffer App", 0, 0, true, true, true, Frame::CallbackLevel::LOW, false);
	Scene scene(frame);
	scene.camera->fov = 90;
	scene.camera->max_distance = 1000;

	std::shared_ptr<Program> program = std::make_shared<Program>(Shader("../GraphicsCortex/Source/GLSL/TextureArray.vert", "../GraphicsCortex/Source/GLSL/TextureArray.frag"));
	std::shared_ptr<Mesh> medival_mesh;
	std::shared_ptr<BindlessMaterial> medival_material = std::make_shared<BindlessMaterial>(program);
	{
		//Model medival_model("../GraphicsCortex/Models/medival/medival.fbx", 1, Model::COORD_XYZ | Model::NORMAL_XYZ | Model::TEX_COORD_XY);
		Model medival_model("../GraphicsCortex/Models/test.obj", 100, Model::COORD_XYZ | Model::NORMAL_XYZ | Model::TEX_COORD_XY);

		medival_mesh = std::make_shared<Mesh>(medival_model);
	}
	AssetImporter::clear_ram_all();
	std::shared_ptr<Graphic> medival = std::make_shared<Graphic>(medival_mesh, medival_material);
	//medival->rotation = glm::vec3(-glm::pi<float>() / 2, 0, 0);
	medival->scale = glm::vec3(0.01f, 0.01f, 0.01f);
	medival->update_matrix();
	scene.add(medival);

	std::shared_ptr<RenderPipeline> pipeline = std::make_shared<RenderPipeline>(1920, 1080, TextureBase2::ColorTextureFormat::RGBA8, 0);
	pipeline->push_render_pass(std::make_shared<RenderPass_GBuffer>());
	pipeline->push_render_pass(std::make_shared<RenderPass_Deferred>());
	scene.pipeline = pipeline;

	medival->material->add_albedo_texture(std::make_shared<Texture2D>("../GraphicsCortex/Models/medival/textures/ID01_Base_color.png", Texture2D::ColorTextureFormat::RGBA8, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE, 1, 0, 0));
	medival->material->add_metalic_texture(std::make_shared<Texture2D>("../GraphicsCortex/Models/medival/textures/ID01_Metallic.png", Texture2D::ColorTextureFormat::RGBA8, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE, 1, 0, 0));
	medival->material->add_ambient_occlusion_texture(std::make_shared<Texture2D>("../GraphicsCortex/Models/medival/textures/ID01_Mixed_AO.png", Texture2D::ColorTextureFormat::RGBA8, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE, 1, 0, 0));
	medival->material->add_normal_texture(std::make_shared<Texture2D>("../GraphicsCortex/Models/medival/textures/ID01_Normal_DirectX.png", Texture2D::ColorTextureFormat::RGBA8, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE, 1, 0, 0));
	medival->material->add_roughness_texture(std::make_shared<Texture2D>("../GraphicsCortex/Models/medival/textures/ID01_Roughness.png", Texture2D::ColorTextureFormat::RGBA8, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE, 1, 0, 0));

	while (frame.is_running()) {
		Framebuffer::bind_screen_read_draw();
		double deltatime = frame.handle_window();
		frame.clear_window();
		frame.display_performance(180);

		scene.camera->handle_movements(frame.window, deltatime);
		scene.render_pipeline();
	}
}