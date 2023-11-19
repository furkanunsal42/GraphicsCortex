#include "GraphicsCortex.h"
#include "Editor.h"

#include "TopBar.h"
#include "ScenePanel.h"
#include "ViewPort.h"
#include "Inspector.h"
#include "ProjectExplorer.h"

int main() {

	Editor::get().push_layer(std::make_shared<TopBar>());
	Editor::get().push_layer(std::make_shared<ScenePanel>());
	Editor::get().push_layer(std::make_shared<ViewPort>());
	Editor::get().push_layer(std::make_shared<Inspector<void>>());
	Editor::get().push_layer(std::make_shared<ProjectExplorer>());

	while (Editor::get().frame->is_running()) {
		Editor::get().render();
	}

}