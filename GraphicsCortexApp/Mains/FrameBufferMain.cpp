#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "FrameBuffer App", 0, 0, true, true, true, Frame::CallbackLevel::LOW, false);
	Scene scene(frame);
	scene.camera->fov = 110;
	scene.camera->max_distance = 1000;

	std::shared_ptr<Program> bindless_program = std::make_shared<Program>(Shader("../GraphicsCortex/Source/GLSL/TextureArray.vert", "../GraphicsCortex/Source/GLSL/TextureArray.frag"));
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

	std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(1920, 1080, Texture2D::ColorTextureFormat::RGBA8, 1, 0);
	std::shared_ptr<Texture2D> depth_stencil_texture = std::make_shared<Texture2D>(1920, 1080, Texture2D::DepthStencilTextureFormat::DEPTH24_STENCIL8, 1, 0);
	Framebuffer2 framebuffer;
	framebuffer.attach_color(0, texture, 0);
	framebuffer.attach_depth_stencil(depth_stencil_texture, 0);
	framebuffer.activate_draw_buffer(0);

	while (frame.is_running()) {
		framebuffer.bind_draw();
		double deltatime = frame.handle_window();
		frame.clear_window();
		frame.display_performance(180);

		scene.camera->handle_movements(frame.window, deltatime);
		scene.render();

		framebuffer.blit_to_screen(0, 0, 1920, 1080, 0, 0, 1920, 1080, Framebuffer2::Channel::COLOR_DEPTH_STENCIL, Framebuffer2::Filter::NEAREST);
	}
}