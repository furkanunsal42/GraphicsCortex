#include "GraphicsCortex.h"
#include "Editor.h"

#include "TopBar.h"
#include "ScenePanel.h"

int main() {

	Editor::get().push_layer(std::make_shared<TopBar>());
	Editor::get().push_layer(std::make_shared<ScenePanel>());

	while (Editor::get().frame->is_running()) {
		Editor::get().render();
	}

}