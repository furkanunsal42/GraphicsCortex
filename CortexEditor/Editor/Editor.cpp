#include "Editor.h"

Editor::Editor() {
	frame = std::make_shared<Frame>(1920, 1080, "Cortex Editor", 0, 1, true, true, true, false);
	gui = std::make_shared<Gui>(*frame);

	current_scene_index = 0;
	scenes.push_back(std::make_shared<Scene>(*frame));
}

void Editor::push_layer(std::shared_ptr<UILayer> layer) {
	layers.push_back(layer);
	layer->init();
}

Editor& Editor::get() {
	static Editor instance;
	return instance;
}

std::shared_ptr<Scene> Editor::get_current_scene() {
	return scenes[current_scene_index];
}

void Editor::render() {
	
	deltatime = frame->handle_window();
	frame->clear_window(1, 1, 1, 1);
	gui->new_frame(deltatime);
	frame->display_performance(180);

	for (std::shared_ptr<UILayer> layer : layers)
		layer->on_gui_render();

}