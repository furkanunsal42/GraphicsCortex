#include "GraphicsCortex.h"

int main(){
	Frame frame(800, 600, "GraphicsCortex", 0, 0, true, true, true, false);
	Scene scene(frame);
	Gui2 gui(frame);
	scene.camera->max_distance = 1000;
	scene.camera->fov = 90.0f;

	std::shared_ptr<Program> program = default_program::solid_program_s();
	std::shared_ptr<UnorderedMaterial> material = std::make_shared<UnorderedMaterial>(1);
	material->set_texture("Images/GoldBlock.png", 4, 0, UnorderedMaterial::COLOR);

	std::shared_ptr<Graphic> graphic = std::make_shared<Graphic>(default_geometry::cube());
	graphic->load_program(program);
	graphic->load_material(material);
	scene.add(graphic);
	
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

		scene.render();

		gui.layout("layout", vec2(100, 100), vec2(100, 200), box_style);
		gui.layout_end();
	}
}