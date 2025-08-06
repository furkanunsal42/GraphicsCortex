#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class BufferStreamingTest : public TestBench {
public:

	bool run() {
		default_init();


		while (!default_window->should_close()) {



		}
		
		return true;
	}
};
