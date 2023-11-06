#include "GraphicsCortex.h"

int main(){
	Frame frame(800, 600, "GraphicsCortex", 0, 0, true, true, true, false);
	Scene scene(frame);
	Gui gui(frame);
	scene.camera->max_distance = 1000;
	scene.camera->fov = 90.0f;

	Program_s program = default_program::solid_program_s();
	UnorderedMaterial_s material(1);
	material->set_texture("Images/GoldBlock.png", 4, 0, UnorderedMaterial::COLOR);

	Graphic_s graphic(default_geometry::cube());
	graphic->load_program(program);
	graphic->load_material(material);
	scene.add_graphic(graphic);
	
	AmbiantLight_s ambiant_light(glm::vec3(1.0f));
	scene.add_light(ambiant_light);

	Style box_style;
	box_style.color = gui::colorcode(0x1f2938);

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		gui.new_frame(deltatime);
		frame.clear_window(0.4f, 0.4f, 1.0f, 1.0f);
		frame.display_performance(180);
		
		scene.camera->handle_movements(frame.window, deltatime);

		scene.render();

		gui.layout(vec2(100, 100), vec2(100, 200), box_style, U"hello there");
		gui.layout_end();
	}
}