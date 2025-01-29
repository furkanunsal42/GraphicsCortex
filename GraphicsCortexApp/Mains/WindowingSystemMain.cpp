#include "GraphicsCortex.h"

int main() {
	
	auto monitors = Monitor::get_all_monitors();
	std::cout << (const char*)monitors[0].get_name().c_str() << std::endl;
	
	auto context = std::make_shared<GraphicsContext>(GraphicsContext::OpenGL, 4, 6);

	while (true) {
		context->handle_events(true);

		primitive_renderer::clear();

		
	}
}