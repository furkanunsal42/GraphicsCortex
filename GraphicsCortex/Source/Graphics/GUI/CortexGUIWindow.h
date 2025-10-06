#pragma once
#include "GUI/CortexGUI.h"

#include "CortexGUIWidgets/Container/GUIWidget_Grid.h"

#include "Window.h"
#include "WindowBoundGlobalResources.h"

class GUIWindow {
private:
	WindowDescription desc() {
		WindowDescription desc;
		desc.w_scale_framebuffer_size = false;
		desc.w_scale_window_size = false;
		desc.w_resolution = glm::ivec2(0, 0);
		desc.w_name = "GraphicsCortexGUI Window";
		desc.w_visible = false;
		desc.context_shared = nullptr;
		return desc;
	}
	WindowDescription desc(Window& other) {
		WindowDescription desc;
		desc.w_scale_framebuffer_size = false;
		desc.w_scale_window_size = false;
		desc.w_resolution = glm::ivec2(0, 0);
		desc.w_name = "GraphicsCortexGUI Window";
		desc.w_visible = false;
		desc.context_shared = other.get_handle();
		return desc;
	}
public:
		
	Window window;
	WidgetHandle<widget::Grid> grid;
	bool visible = true;
	bool root;

	GUIWindow(GUIWindow& other) :
		window(desc(other.window)),
		grid(widget::create<widget::Grid>()),
		root(false)
	{
		grid->target_size = glm::vec2(600, 400);
		grid->color = glm::vec4(0.94, 0.94, 0.94, 1);

		grid->add_column(grid->target_size.x);
		grid->add_row(grid->target_size.y);
	}

	GUIWindow() :
		window(desc()),
		grid(widget::create<widget::Grid>()),
		root(true)
	{
		grid->target_size = glm::vec2(600, 400);
		grid->color = glm::vec4(0.94, 0.94, 0.94, 1);

		grid->add_column(grid->target_size.x);
		grid->add_row(grid->target_size.y);
	}

	virtual void run() {

		while (!window.should_close()) {

			if (active_window != &window)
				window.context_make_current();

			double deltatime = window.handle_events(true);
			primitive_renderer::clear(grid->color.r, grid->color.g, grid->color.b, grid->color.a);
				
			if (window.get_window_resolution() != glm::ivec2(grid->target_size)) {
				window.set_window_resolution(glm::ivec2(grid->target_size));
				primitive_renderer::set_viewport_size(grid->target_size);
			}
				
			if (window.is_window_visible() != visible)
				window.set_window_visibility(visible);

			GUI::get().render(grid);

			window.swap_buffers();
		}

		window.release();
	}

private:

};