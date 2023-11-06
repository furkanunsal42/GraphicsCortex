#pragma once
#include "Editor.h"

class TopBar : public UILayer {
	
	Style topbar_style;

	void init() {
		topbar_style.color = gui::colorcode(0x15171c);
	}

	void on_gui_render() {
		Editor& editor = Editor::get();
		
		editor.gui->layout(vec2(0, 0), vec2(editor.gui->camera.screen_width, 40), topbar_style, U"", Layout::Horizional);
	}
};