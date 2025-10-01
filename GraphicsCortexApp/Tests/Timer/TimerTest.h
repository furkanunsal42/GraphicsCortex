#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "System/Timer.h"

class TimerTest : public TestBench {
public:

	bool run() {

		using namespace std::chrono_literals;

		default_init();

		Timer<true> timer;

		timer.get_newsletter().subscribe([&]() {
			std::cout << "timer triggered" << std::endl;
			});

		timer.start_periodic(150ms);

		//timer.stop();
		//std::cout << timer.is_running() << std::endl;

		//while (true)
		//	timer.handle_events();

		std::cin.get();
		return true;
	}
};
