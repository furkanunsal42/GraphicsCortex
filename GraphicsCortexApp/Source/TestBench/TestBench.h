#pragma once

#include <memory>
#include "Window.h"

class TestBench {
public:

	WindowDescription* default_window_desc = nullptr;
	std::shared_ptr<Window> default_window = nullptr;
	void default_init();

	virtual bool run() { return true; };

};