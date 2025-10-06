#pragma once
#include "GUI/CortexGUI.h"

#include "CortexGUIWidgets/Container/GUIWidget_Grid.h"

#include "Window.h"
#include "WindowBoundGlobalResources.h"

class GUIWindow {
private:
	constexpr WindowDescription desc() {
		WindowDescription desc;
		desc.w_scale_framebuffer_size = false;
		desc.w_scale_window_size = false;
		desc.w_resolution = glm::ivec2(0, 0);
		desc.w_name = "GraphicsCortexGUI Window";
		desc.w_visible = false;
		return desc;
	}
public:
		
	Window* window = new Window(desc());
	bool visible = true;
	widget::Grid grid;

	GUIWindow() {

		grid.target_size = glm::vec2(600, 400);
		grid.color = glm::vec4(0.94, 0.94, 0.94, 1);

		grid.add_column(grid.target_size.x);
		grid.add_row(grid.target_size.y);
	}

	~GUIWindow(){
		delete window;
	}

	virtual void run() {

		while (!window->should_close()) {

			double deltatime = window->handle_events(true);
			primitive_renderer::clear(grid.color.r, grid.color.g, grid.color.b, grid.color.a);

			if (active_window != window)
				window->context_make_current();
				
			if (window->get_window_resolution() != glm::ivec2(grid.target_size)) {
				window->set_window_resolution(glm::ivec2(grid.target_size));
				primitive_renderer::set_viewport_size(grid.target_size);
			}
				
			if (window->is_window_visible() != visible)
				window->set_window_visibility(visible);

			GUI::get().render(grid);

			window->swap_buffers();
		}

		GUI::get().release();
		window->release();
	}

private:
};

//template<>
//inline WidgetHandle<GUIWindow> create() {
//
//	WindowDescription desc;
//	desc.w_scale_framebuffer_size = false;
//	desc.w_scale_window_size = false;
//	desc.w_resolution = glm::ivec2(0, 0);
//	desc.w_name = "GraphicsCortexGUI Window";
//	desc.w_visible = false;
//	Window* window = new Window(desc);
//
//	auto widget = GUI::get().create_widget<GUIWindow>();
//	widget->window = window;
//
//	return widget;
//}
