#pragma once
#include "GUI/CortexGUI.h"

#include "CortexGUIWidgets/Container/GUIWidget_Grid.h"

#include "Window.h"
#include "WindowBoundGlobalResources.h"

namespace widget {

	class GUIWindow : public Grid {
	public:

		Window* window = nullptr;
		bool visible = true;

		GUIWindow() {

			target_size = glm::vec2(600, 400);
			color = glm::vec4(0.94, 0.94, 0.94, 1);

			add_column(target_size.x);
			add_row(target_size.y);
		}

		~GUIWindow(){
			delete window;
		}

		Element& get_element(glm::vec2 allocated_size) {

			set_column_size(allocated_size.x, 0);
			set_row_size(allocated_size.y, 0);

			return Grid::get_element(allocated_size);
		}

		void poll_events(glm::vec2 absolute_position) override {
			
			if (active_window != window)
				window->context_make_current();

			if (window->get_window_resolution() != glm::ivec2(element.size())) {
				window->set_window_resolution(glm::ivec2(element.size()));
				primitive_renderer::set_viewport_size(element.size());
			}

			if (window->is_window_visible() != visible)
				window->set_window_visibility(visible);

			Grid::poll_events(absolute_position);
		}

		virtual void run() {

			while (!window->should_close()) {

				double deltatime = window->handle_events(true);
				primitive_renderer::clear(color.r, color.g, color.b, color.a);

				GUI::get().render(*this);
				poll_events(glm::vec2(0));

				window->swap_buffers();
			}

			GUI::get().release();

		}

	private:
	};

	template<>
	inline WidgetHandle<GUIWindow> create() {

		WindowDescription desc;
		desc.w_scale_framebuffer_size = false;
		desc.w_scale_window_size = false;
		desc.w_resolution = glm::ivec2(0, 0);
		desc.w_name = "GraphicsCortexGUI Window";
		desc.w_visible = false;
		Window* window = new Window(desc);

		auto widget = GUI::get().create_widget<GUIWindow>();
		widget->window = window;

		return widget;
	}
}
