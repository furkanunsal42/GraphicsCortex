#include "GraphicsCortex.h"

int main() {
	
	auto monitors = Monitor::get_all_monitors();
	std::cout << (const char*)monitors[0].get_name().c_str() << std::endl;
	
	//auto context = std::make_shared<GraphicsContext>();
	
	auto window = std::make_shared<Window>(glm::ivec2(512, 512), "Hello There");

	window->newsletters->on_filepath_drop_events.subscribe([](const std::vector<std::u8string>& paths) {
		for (auto& path : paths)
			std::cout << (const char*)path.c_str() << std::endl;
		});
	window->newsletters->on_should_close_events.subscribe([]() {
		exit(0);
		});


	while (true) {
		window->handle_events(true);
		//primitive_renderer::clear();
	}
}