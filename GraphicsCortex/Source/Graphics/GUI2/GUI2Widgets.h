#pragma once
#include "GUI2Dynamic.h"

class Widget {
public:
	virtual void publish(GUI2Dynamic& gui_dynaimc) = 0;
};

class Label : public Widget {
public:
	std::string text;
	void publish(GUI2Dynamic& gui_dynaimc) override {};
};

