#pragma once
#include "Editor.h"

class ViewPort : public UILayer {

	Style layout_style;
	Style object_style;
	Style button_style;
	Style searchbar_style;


	vec2 position = vec2(400, 30);
	vec2 size = vec2(1100, 800);

	std::shared_ptr<Framebuffer> scene_render;

	void init() {
		std::shared_ptr<Texture2D> render_target = std::make_shared<Texture2D>(size.x, size.y, Texture2D::ColorTextureFormat::RGBA8, 1, 0, 0);
		std::shared_ptr<Texture2D> render_target_depthstencil = std::make_shared<Texture2D>(size.x, size.y, Texture2D::DepthStencilTextureFormat::DEPTH24_STENCIL8, 1, 0, 0);
		scene_render = std::make_shared<Framebuffer>();
		scene_render->attach_color(0, render_target, 0);
		scene_render->attach_depth_stencil(render_target_depthstencil, 0);

		layout_style.text_color = vec3(1, 1, 1);
		layout_style.text_size = 12;
		layout_style.color = gui::colorcode(0x242424);

		object_style.text_color = vec3(1, 1, 1);
		object_style.text_size = 12;
		object_style.color = gui::colorcode(0x242424);
		object_style.on_hover.color = gui::colorcode(0x303030);
		object_style.margin = vec4(0, 10, 0, 10);
		object_style.color_change = 0.1;
		object_style.color_interpolation = Interpolation::polynomial(0.5);

		button_style.text_color = vec3(1, 1, 1);
		button_style.text_size = 12;
		button_style.color = gui::colorcode(0x242424);
		button_style.on_hover.color = gui::colorcode(0x303030);
		button_style.color_change = 0.1;
		button_style.color_interpolation = Interpolation::polynomial(0.5);
		button_style.margin = vec4(10, 10, 10, 10);

		searchbar_style.text_color = vec3(1, 1, 1);
		searchbar_style.text_size = 10;
		searchbar_style.color = gui::colorcode(0x181818);
		searchbar_style.on_hover.color = gui::colorcode(0x303030);
		searchbar_style.color_change = 0.1;
		searchbar_style.color_interpolation = Interpolation::polynomial(0.5);
		searchbar_style.corner_rounding = vec4(10, 10, 10, 10);
		searchbar_style.on_hover.corner_rounding = vec4(0, 0, 0, 0);

		searchbar_style.margin = vec4(10, 10, 10, 10);

	}

	void on_gui_render() {
		Editor& editor = Editor::get();
		std::shared_ptr<Scene> current_scene = editor.scenes[editor.current_scene_index];

		current_scene->camera->screen_width = size.x;
		current_scene->camera->screen_height = size.y;
		current_scene->background_color = glm::vec4(1, 1, 1, 1);

		scene_render->activate_draw_buffer(0);
		scene_render->bind_draw();
		editor.frame->set_viewport(size.x, size.y);
		editor.frame->clear_window();

		current_scene->render();

		scene_render->blit_to_screen(0, 0, size.x, size.y, position.x, editor.frame->window_height - position.y - size.y, position.x + size.x, editor.frame->window_height - position.y, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);
		Framebuffer::bind_screen_draw();
		editor.frame->set_viewport(editor.frame->window_width, editor.frame->window_height);

		glm::vec2 viewport_area_midpoint = glm::vec2(editor.frame->window_height - position.y - size.y, position.x + size.x);
		viewport_area_midpoint = glm::vec2(position.x + size.x / 2, position.y + size.y / 2);

		if(AABB2(position, size).does_contain(editor.frame->get_cursor_position()))
			editor.get_current_scene()->camera->handle_movements(editor.frame->window, viewport_area_midpoint, editor.deltatime);
	}
};