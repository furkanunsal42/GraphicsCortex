#pragma once

#include "Window.h"

#include <vector>
#include <memory>

class EditorPanel {
public:

	virtual void on_init() = 0;
	virtual void on_render() = 0;

};

class Editor {
public:

	Editor();
	void run();

	template<typename panel_t>
	void push_panel();

private:

	std::vector<std::shared_ptr<EditorPanel>> panels;
	
	Window window;
};

template<typename panel_t>
inline void Editor::push_panel() {
	panels.push_back(std::make_shared<panel_t>());
	panels.back()->on_init();
}