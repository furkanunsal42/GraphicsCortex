#include "GraphicsCortex.h"
#include "Editor.h"

#include "TopBar.h"
#include "ScenePanel.h"
#include "ViewPort.h"

int main() {

	Editor::get().push_layer(std::make_shared<TopBar>());
	Editor::get().push_layer(std::make_shared<ScenePanel>());
	Editor::get().push_layer(std::make_shared<ViewPort>());

	std::shared_ptr<Scene> scene = Editor::get().get_current_scene();
	
	Program_s program = default_program::solid_program_s();
	UnorderedMaterial_s material(1);
	material->set_texture("Images/GoldBlock.png", 4, 0, UnorderedMaterial::COLOR);

	Graphic_s graphic(default_geometry::cube());
	graphic->load_program(program);
	graphic->load_material(material);
	scene->add_graphic(graphic);

	AmbiantLight_s ambiant_light(glm::vec3(1.0f));
	scene->add_light(ambiant_light);

	while (Editor::get().frame->is_running()) {
		Editor::get().render();
	}

}