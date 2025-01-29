#include "GraphicsCortex.h"

int main() {
	
	auto monitors = Monitor::get_all_monitors();

	auto frame = std::make_shared<Frame2>(1024, 1024, "GraphicsCortex Window", monitors[0]);


	while (!frame->should_close()) {
		frame->handle_window_events();

		primitive_renderer::clear();

		
	}
}