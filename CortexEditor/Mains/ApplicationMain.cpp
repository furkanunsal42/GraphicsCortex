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

	while (Editor::get().frame->is_running()) {
		Editor::get().render();
	}

}