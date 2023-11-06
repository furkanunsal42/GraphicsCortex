#pragma once

#include <vector>
#include <memory>

#include "Frame.h"
#include "Scene.h"
#include "Gui.h"

class UILayer;

class Editor {
public:
	std::unique_ptr<Frame> frame;
	std::unique_ptr<Gui> gui;
	std::vector<std::unique_ptr<UILayer>> layers;

	int current_scene_index;
	std::vector<std::unique_ptr<Scene>> scenes;

	Editor();

	void render();
};

class UILayer {

	virtual void on_gui_render() {};

};