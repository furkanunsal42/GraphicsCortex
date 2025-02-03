#include "GraphicsCortex.h"

int main() {
	
	auto monitors = Monitor::get_all_monitors();
	std::cout << (const char*)monitors[0].get_name().c_str() << std::endl;
	
	WindowDescription desc;
	auto window = std::make_shared<Window>(desc);
	
	window->newsletters->on_filepath_drop_events.subscribe([](const std::vector<std::u8string>& paths) {
		for (auto& path : paths)
			std::cout << (const char*)path.c_str() << std::endl;
		});
	window->newsletters->on_should_close_events.subscribe([]() {
		exit(0);
		});

	window->newsletters->on_window_refresh_events.subscribe([&]() {
		primitive_renderer::clear();
		window->swap_buffers();
		});

	while (true) {
		window->handle_events(true);
		primitive_renderer::clear();
	}
}