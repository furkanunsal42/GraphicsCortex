#include "GraphicsCortex.h"

int main(){
	Frame frame(800, 600, "GraphicsCortex", 0, 0, true, true, false, Frame::CallbackLevel::NOTIFICATION, false);
	Scene scene(frame);
	Gui gui(frame);
	scene.camera->max_distance = 1000;
	scene.camera->fov = 90.0f;

	std::shared_ptr<BindlessMaterial> material = std::make_shared<BindlessMaterial>();
	
	std::shared_ptr<Texture2D> gold_texture = std::make_shared<Texture2D>("../GraphicsCortex/Images/GoldBlock.png", Texture2D::ColorTextureFormat::RGBA8, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE, 32, 0, 0);
	material->add_albedo_texture(gold_texture);

	std::shared_ptr<Graphic> graphic = default_geometry::cube();
	graphic->set_material(material);
	scene.add(graphic);
	
	std::shared_ptr<RenderPipeline> pipeline = std::make_shared<RenderPipeline>(frame.window_width, frame.window_height, TextureBase2::ColorTextureFormat::RGBA8, 0);
	pipeline->push_render_pass(std::make_shared<RenderPass_GBuffer>());
	pipeline->push_render_pass(std::make_shared<RenderPass_Deferred>());
	scene.pipeline = pipeline;

	std::shared_ptr<AmbiantLight> ambiant_light = std::make_shared<AmbiantLight>(glm::vec3(1.0f));
	scene.add(ambiant_light);

	Style box_style;
	box_style.color = gui::colorcode(0x1f2938);

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		gui.new_frame(deltatime);
		frame.clear_window(0.4f, 0.4f, 1.0f, 1.0f);
		frame.display_performance(180);
		
		scene.camera->handle_movements(frame.window, deltatime);

		scene.render_pipeline();

		gui.layout("layout", vec2(100, 100), vec2(100, 200), box_style);
		gui.layout_end();
	}
}