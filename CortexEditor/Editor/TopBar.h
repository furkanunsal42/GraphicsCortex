#pragma once
#include "Editor.h"
#include <iostream>

class TopBar : public UILayer {
	
	Style topbar_style;
	Style button_style;
	void init() {
		topbar_style.color = gui::colorcode(0x242424);
	
		button_style.text_color = vec3(1, 1, 1);
		button_style.text_size = 10;
		button_style.color = gui::colorcode(0x242424);
		button_style.on_hover.color = gui::colorcode(0x303030);
		button_style.on_hover.cursor_type = Frame::CursorType::Hand;
		button_style.margin = vec4(0, 10, 0, 10);
	}

	void on_gui_render() {
		Editor& editor = Editor::get();
		
		editor.gui->layout("topbar_layout", vec2(0, 0), vec2(editor.frame->window_width, 30), topbar_style, Layout::Horizional);
		editor.gui->content("topbar_file", vec2(100, 30), button_style, U"File");
		editor.gui->content("topbar_edit", vec2(100, 30), button_style, U"Edit");
		editor.gui->content("topbar_view", vec2(100, 30), button_style, U"View");
		editor.gui->content("topbar_project", vec2(100, 30), button_style, U"Project");
		editor.gui->content("topbar_build", vec2(100, 30), button_style, U"Build");
		editor.gui->content("topbar_debug", vec2(100, 30), button_style, U"Debug");
		editor.gui->layout_end();
	}
};