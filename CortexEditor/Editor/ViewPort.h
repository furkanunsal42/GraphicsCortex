#pragma once
#include "Editor.h"

class ViewPort : public UILayer {

	Style layout_style;
	Style object_style;
	Style button_style;
	Style searchbar_style;


	vec2 position = vec2(400, 30);
	vec2 size = vec2(1100, 800);

	std::shared_ptr<FrameBuffer> scene_render;

	void init() {
		scene_render = std::make_shared<FrameBuffer>(size.x, size.y, 0, false);
		scene_render->load_program(default_program::framebuffer_program_s());
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

		current_scene->render_to_framebuffer(*scene_render, *editor.frame);
		
		scene_render->blit_section_to_screen(glm::vec4(0, 0, size.x, size.y), glm::vec4(position.x, editor.gui->window_size.y - position.y - size.y, position.x + size.x, editor.gui->window_size.y - position.y));

		//editor.get_current_scene()->camera->handle_movements(editor.frame->window, editor.deltatime);
	}
};