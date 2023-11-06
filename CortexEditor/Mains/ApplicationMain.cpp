#include "GraphicsCortex.h"
#include "Editor.h"

#include "TopBar.h"

int main() {

	Editor::get().push_layer(std::make_shared<TopBar>());

	while (Editor::get().frame->is_running()) {
		Editor::get().render();
	}

}