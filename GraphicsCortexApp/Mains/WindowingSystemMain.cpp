#include "GraphicsCortex.h"

int main() {
	
	auto monitors = Monitor::get_all_monitors();
	std::cout << (const char*)monitors[0].get_name().c_str() << std::endl;
	
	FrameDescription desc;
	auto frame = std::make_shared<Frame2>(desc);
	
	frame->set_window_opacity(0.5);

	frame->newsletters->on_filepath_drop_events.subscribe([](const std::vector<std::u8string>& paths) {
		for (auto& path : paths)
			std::cout << (const char*)path.c_str() << std::endl;
		});
	frame->newsletters->on_should_close_events.subscribe([]() {
		exit(0);
		});


	while (true) {
		frame->handle_events(true);
		primitive_renderer::clear();
	}
}