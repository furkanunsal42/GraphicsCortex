#include "GraphicsCortex.h"
#include "Framebuffer.h"

int main() {
	Frame frame(1920, 1080, "FrameBuffer App", 4, 0, true, true, true, Frame::CallbackLevel::LOW, false);
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

	std::shared_ptr<Renderbuffer> position_texture = std::make_shared<Renderbuffer>(1920, 1080, Renderbuffer::ColorTextureFormat::RGBA8, 0);
	std::shared_ptr<Renderbuffer> texcoord_texture = std::make_shared<Renderbuffer>(1920, 1080, Renderbuffer::ColorTextureFormat::RGBA8, 0);
	std::shared_ptr<Renderbuffer> normal_texture = std::make_shared<Renderbuffer>(1920, 1080, Renderbuffer::ColorTextureFormat::RGBA8, 0);
	std::shared_ptr<Renderbuffer> depth_stencil_texture = std::make_shared<Renderbuffer>(1920, 1080, Renderbuffer::DepthStencilTextureFormat::DEPTH24_STENCIL8, 0);

	std::shared_ptr<Framebuffer> gbuffer = std::make_shared<Framebuffer>();
	gbuffer->attach_color(0, position_texture);
	gbuffer->attach_color(1, texcoord_texture);
	gbuffer->attach_color(2, normal_texture);
	gbuffer->attach_depth_stencil(depth_stencil_texture);

	while (frame.is_running()) {
		gbuffer->deactivate_all_draw_buffers();
		gbuffer->activate_draw_buffer(0);
		gbuffer->activate_draw_buffer(1);
		gbuffer->activate_draw_buffer(2);
		gbuffer->bind_draw();

		double deltatime = frame.handle_window();
		frame.clear_window();
		frame.display_performance(180);

		scene.camera->handle_movements(frame.window, deltatime);
		scene.render();

		gbuffer->set_read_buffer(2);
		gbuffer->blit_to_screen(0, 0, 1920, 1080, 0, 0, 1920, 1080, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);
	}
}