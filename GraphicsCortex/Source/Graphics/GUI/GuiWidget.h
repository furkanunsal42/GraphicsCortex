#pragma once
#include <functional>
#include <string>

class Gui;

class InteractEvents {
public:
	bool was_hovering_last_frame = false;
	bool was_pressing_last_frame = false;

	std::string name;

	std::function<void()> on_press		 = []() {};
	std::function<void()> on_release	 = []() {};
	std::function<void()> on_hover_begin = []() {};
	std::function<void()> on_hover_end	 = []() {};
};

class GuiWidget : public InteractEvents {
public:
	virtual void on_render(Gui& gui) = 0;
};
