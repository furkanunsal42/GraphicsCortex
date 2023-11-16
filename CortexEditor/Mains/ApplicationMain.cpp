#include "GraphicsCortex.h"
#include "Editor.h"

#include "TopBar.h"
#include "ScenePanel.h"
#include "ViewPort.h"
#include "Inspector.h"

int main() {

	Editor::get().push_layer(std::make_shared<TopBar>());
	Editor::get().push_layer(std::make_shared<ScenePanel>());
	Editor::get().push_layer(std::make_shared<ViewPort>());
	Editor::get().push_layer(std::make_shared<Inspector>());

	std::shared_ptr<Scene> scene = Editor::get().get_current_scene();
	
	AmbiantLight_s ambiant_light(glm::vec3(0.1f));
	scene->add_light(ambiant_light);

	DirectionalLight_s sun(glm::vec3(0, 0, 0), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.8f));
	scene->add_light(sun);

	SpotLight_s spot(glm::vec3(-2.0f, -1.0f, -2.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.3f, 0.2f, 0.0f, 30);
	scene->add_light(spot);

	while (Editor::get().frame->is_running()) {
		Editor::get().render();
	}

}