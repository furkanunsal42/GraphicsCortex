#include "Editor.h"

#include "Window.h"
#include "PrimitiveRenderer.h"
#include "Font.h"

Editor::Editor() : 
	window(glm::ivec2(1920, 1080), "GraphicsCortex Editor")
{
	font_id font = FontBank::get().load_font("../GraphicsCortex/Fonts/Roboto-Regular.ttf", 64);
}

void Editor::run() {

	window.newsletters->on_window_resolution_events.subscribe([&](const glm::ivec2& resolution) {
		primitive_renderer::set_viewport_size(resolution);
		});

	window.newsletters->on_window_refresh_events.subscribe([&]() {
		primitive_renderer::clear(0, 0, 0, 1);

		for (auto& panel : panels)
			panel->on_render();

		window.swap_buffers();
		});

	while (!window.should_close()) {
		
		window.handle_events(true);
		primitive_renderer::clear(0, 0, 0, 1);

		for (auto& panel : panels)
			panel->on_render();

		window.swap_buffers();
	}
}