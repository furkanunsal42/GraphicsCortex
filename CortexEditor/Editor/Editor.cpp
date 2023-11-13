#include "Editor.h"

Editor::Editor() {
	frame = std::make_shared<Frame>(1920, 1080, "Cortex Editor", 0, 1, true, true, true, false);
	gui = std::make_shared<Gui>(*frame);

	current_scene_index = 0;
	scenes.push_back(std::make_shared<Scene>(*frame));
}

Editor::~Editor() {
	for (std::shared_ptr<UILayer>& layer : layers) 
		layer = nullptr;

	for (std::shared_ptr<Scene>& scene : scenes)
		scene = nullptr;
	
	gui = nullptr;
	frame = nullptr;
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
	frame->clear_window(0, 0, 0, 1);
	gui->new_frame(deltatime);
	frame->display_performance(180);

	for (std::shared_ptr<UILayer> layer : layers)
		layer->on_gui_render();

}