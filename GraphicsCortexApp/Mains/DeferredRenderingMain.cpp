#include "GraphicsCortex.h"
#include "Framebuffer.h"

int main() {
	Frame frame(1920, 1080, "FrameBuffer App", 0, 0, true, true, true, Frame::CallbackLevel::LOW, false);
	Scene scene(frame);
	scene.camera->fov = 110;
	scene.camera->max_distance = 1000;

	std::shared_ptr<Program> deferred_program = std::make_shared<Program>(Shader("../GraphicsCortex/Source/GLSL/Surface/surface.vert", "../GraphicsCortex/Source/GLSL/Surface/surface.geom", "../GraphicsCortex/Source/GLSL/Surface/gbuffer.frag"));
	std::shared_ptr<Mesh> dragon_mesh;
	std::shared_ptr<BindlessMaterial> dragon_material;
	{
		Model dragon_model("../GraphicsCortex/Models/dragon_new/dragon_new.fbx", 1, Model::COORD_XYZ | Model::NORMAL_XYZ | Model::TEX_COORD_XY);
		dragon_mesh = std::make_shared<Mesh>(dragon_model);
		dragon_material = AssetImporter::generate_material("../GraphicsCortex/Models/dragon_new/dragon_new.fbx", deferred_program);
	}
	AssetImporter::clear_ram_all();
	std::shared_ptr<Graphic> dragon = std::make_shared<Graphic>(dragon_mesh, dragon_material);
	dragon->rotation = glm::vec3(-glm::pi<float>() / 2, 0, 0);

	scene.add(dragon);

	dragon->material->_pbr_ambient_occlusion = std::make_shared<Texture2D>(1024, 1024, Texture2D::ColorTextureFormat::RGBA8, 1, 0, 0);
	dragon->material->_pbr_ambient_occlusion->load_data_async("../GraphicsCortex/Images/orange.png", Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE, 0);
	dragon->material->_pbr_const_albedo = glm::vec4(1, 1, 1, 1);

	std::shared_ptr<RenderPipeline> pipeline = std::make_shared<RenderPipeline>(1920, 1080, TextureBase2::ColorTextureFormat::RGBA8, 0);
	pipeline->push_render_pass(std::make_shared<RenderPass_GBuffer>());
	dragon->material->_pbr_const_albedo = glm::vec4(1, 1, 1, 1);
	//pipeline->push_render_pass(std::make_shared<RenderPass_Deferred>());

	scene.pipeline = pipeline;

	while (frame.is_running()) {
		Framebuffer::bind_screen_read_draw();
		double deltatime = frame.handle_window();
		frame.clear_window();
		frame.display_performance(180);

		scene.camera->handle_movements(frame.window, deltatime);
		scene.render_pipeline();
	}
}