#pragma once

#include <vector>
#include <memory>

#include "Frame.h"
#include "Scene.h"
#include "Gui.h"

class UILayer {
public:
	virtual void init() = 0;
	virtual void on_gui_render() = 0;
};

class Editor {
private:
	Editor();
	Editor(const Editor&) = delete;
	void operator=(const Editor&) = delete;
	std::vector<std::shared_ptr<UILayer>> layers;
public:
	double deltatime;
	std::shared_ptr<Frame> frame;
	std::shared_ptr<Gui> gui;

	void push_layer(std::shared_ptr<UILayer> layer);

	int current_scene_index;
	std::vector<std::shared_ptr<Scene>> scenes;

	static Editor& get();

	std::shared_ptr<Scene> get_current_scene();

	void render();
};
