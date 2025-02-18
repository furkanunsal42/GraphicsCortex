#include "TestBench.h"

void TestBench::default_init() {
	if (default_window != nullptr) return;

	WindowDescription desc = default_window_desc != nullptr ? *default_window_desc : WindowDescription();

	default_window = std::make_shared<Window>(desc);

	default_window->newsletters->on_should_close_events.subscribe([]() {
		exit(0);
		});
}
