#pragma once
#include "CortexGUIWindow.h"
#include "CortexGUIWindowHierarchy.h"

namespace {
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
}

GUIWindow::GUIWindow(Window& window)
{
	this->window = std::make_unique<Window>(desc(window));
	grid = widget::create<widget::Grid>();

	grid->target_size = glm::vec2(600, 400);
	grid->color = glm::vec4(0.94, 0.94, 0.94, 1);

	grid->add_column(grid->target_size.x);
	grid->add_row(grid->target_size.y);
}

GUIWindow::GUIWindow()
{
	window = std::make_unique<Window>(desc());
	grid = widget::create<widget::Grid>();

	grid->target_size = glm::vec2(600, 400);
	grid->color = glm::vec4(0.94, 0.94, 0.94, 1);

	grid->add_column(grid->target_size.x);
	grid->add_row(grid->target_size.y);
}

void GUIWindow::run() {

	std::vector<guiwindow_t> children;

	children.push_back(id);

	for (auto child : GUIWindowHierarchy::get().get_window_children(id))
		children.push_back(child);

	while (!window->should_close()) {
		
		for (auto child : children) {
			
			GUIWindow& child_window = GUIWindowHierarchy::get().get_window_data(child);

			child_window.window->context_make_current();

			if (child_window.window->should_close()) {
				GUIWindowHierarchy::get().release_window(child);
				std::erase(children, child);
				break;
			}

			child_window.handle_events();
			child_window.render();

			child_window.window->swap_buffers();
		}
	}

	GUIWindowHierarchy::get().release_window(id);
}

void GUIWindow::handle_events() {

	double deltatime = window->handle_events(true);
	primitive_renderer::clear(grid->color.r, grid->color.g, grid->color.b, grid->color.a);

	if (window->get_window_resolution() != glm::ivec2(grid->target_size)) {
		window->set_window_resolution(glm::ivec2(grid->target_size));
		primitive_renderer::set_viewport_size(grid->target_size);
	}

	if (window->is_window_visible() != visible)
		window->set_window_visibility(visible);
}

void GUIWindow::render() {

	GUI::get().render(grid);

}