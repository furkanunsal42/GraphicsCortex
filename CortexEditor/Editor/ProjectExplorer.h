#pragma once

#include "Editor.h"

class ProjectExplorer : public UILayer {
	vec2 position = vec2(0, 830);
	vec2 size = vec2(1920, 0);

	Style layout_style;
	Style body_style;
	Style header_style;

	Style entry_style;
	Style selected_entry_style;

	int row_height = 20;

	void init() {
		layout_style.text_color = vec3(1, 1, 1);
		layout_style.text_size = 10;
		layout_style.color = gui::colorcode(0x242424);
		layout_style.padding = vec4(5, 10, 10, 10);
		layout_style.border_color = gui::colorcode(0x303030);
		layout_style.border_thickness = vec4(2, 2, 2, 2);

		header_style.color = gui::colorcode(0x242424);
		header_style.text_allign_type = Style::CenterY;
		header_style.margin = vec4(0, 10, 0, 0);
		header_style.text_color = vec3(1, 1, 1);
		header_style.text_size = 10;

		body_style = layout_style;
		body_style.color = gui::colorcode(0x202020);
		body_style.padding = vec4(5, 5, 5, 5);
		body_style.border_color = gui::colorcode(0x181818);
		body_style.border_thickness = vec4(2, 2, 1, 1);
		body_style.corner_rounding = vec4(10, 10, 10, 10);

		entry_style.color = gui::colorcode(0x202020);
		//entry_style.on_hover.color = gui::colorcode(0x303030);
		entry_style.text_color = vec3(1, 1, 1);
		entry_style.text_size = 10;
		entry_style.text_allign_type = Style::TextAlligning::CenterY;

		selected_entry_style = entry_style;
		selected_entry_style.color = gui::colorcode(0x2566e8);
		selected_entry_style.on_hover.color = gui::colorcode(0x2566e8);
	}

	void on_gui_render() {
		Editor& editor = Editor::get();
		size.y = editor.frame->window_height - 830;

		editor.gui->layout("project_explorer_layout", position, size - vec2(20, 50), layout_style);
		editor.gui->content("project_explorer_name", vec2(size.x - 20, 35), header_style, U"Project Explorer");
		editor.gui->layout_content("project_explorer_body", size - vec2(20, 50) - vec2(10, 10), body_style);
		vec2 body_size = size - vec2(20, 50) - vec2(10, 10);

		editor.gui->content("project_explorer_slot" + 0, vec2(body_size.x, row_height), entry_style, U"project explorer slot");
		editor.gui->content("project_explorer_slot" + 1, vec2(body_size.x, row_height), entry_style, U"WORK IN PROGRESS");

		editor.gui->layout_content_end();
		editor.gui->layout_end();

	}
};

