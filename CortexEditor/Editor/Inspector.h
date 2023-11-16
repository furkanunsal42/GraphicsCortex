#include "Editor.h"

class Inspector : public UILayer {

	vec2 position = vec2(1100 + 400, 30);
	vec2 size	  = vec2(420, 800);

	Style background_style;
	Style entry_style;

	void init() {
		background_style.color = gui::colorcode(0x242424);
		background_style.text_color = vec3(1, 1, 1);
		background_style.text_size = 10;

		entry_style.color = gui::colorcode(0x242424);
		entry_style.text_color = vec3(1, 1, 1);
		entry_style.text_size = 10;
		entry_style.margin = vec4(10, 10, 10, 10);
	}

	void on_gui_render() {

		Editor& editor = Editor::get();

		editor.gui->layout("inspector_layout", position, size, background_style);
		editor.gui->content("inspector_name", vec2(100, 25), background_style, U"Inspector");

		editor.gui->content("inspector_header_seperator", vec2(1, 50), background_style, U"");
		editor.gui->content("inspector_position1", vec2(100, 25), entry_style, U"position");
		
		editor.gui->layout_end();

	}
};