#include "GraphicsCortex.h"

int main() {

	std::shared_ptr<Window> window = std::make_shared<Window>(glm::ivec2(0, 0), "2DRenderer");



	while (!window->should_close()) {
		window->handle_events(true);
		
		

	}
}