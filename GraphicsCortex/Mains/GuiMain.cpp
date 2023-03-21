#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "GraphicsCortex", 16, 0, true, true, true, false);
	Scene scene(frame);
	Gui gui(frame);

	Style simple_button;
	simple_button.color = gui::colorcode(0x383838);
	//simple_button.corner_rounding = Persentage(0.2f, Persentage::MIN_SIZE_DIM);
	simple_button.on_hover.cursor_type = Frame::Hand;
	simple_button.on_hover.color = gui::colorcode(0x424242);
	simple_button.color_change = 0.1;
	simple_button.color_interpolation = Interpolation::polynomial(3);

	Style dropdown_button;
	dropdown_button.color = gui::colorcode(0x3F855A);
	dropdown_button.on_hover.color = gui::colorcode(0x475F95);
	dropdown_button.color_change = 0.1;
	dropdown_button.color_interpolation = Interpolation::polynomial(3);
	dropdown_button.cursor_type = Frame::Hand;

	Style navbar_backround;
	navbar_backround.color = gui::colorcode(0x1E1E1E);

	Style panel_elements;
	panel_elements.color = gui::colorcode(0x383838);
	panel_elements.on_hover.color = gui::colorcode(0x475F95);
	panel_elements.margin = vec4(10);
	

	Style scene_elements;
	scene_elements.color = gui::colorcode(0x383838);
	scene_elements.border_color = gui::colorcode(0x383838);
	scene_elements.on_hover.border_color = gui::colorcode(0x475F95);
	scene_elements.border_thickness = vec4(1);
	scene_elements.border_color_change = 0.1;
	scene_elements.border_color_interpolation = Interpolation::polynomial(3);

	Font text("Fonts\\Roboto-Regular.ttf");
	text.generate_text_graphic("File", scene, 0.2f);
	text.graphics_representation->set_uniform("text_color", 0.0f, 0.0f, 0.0f, 1.0f);
	text.graphics_representation->set_position(glm::vec3(0, 0, -1));

	bool show_dropdown = false;

	vec2 panel_location(200, 200);

	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window(1, 1, 1, 1);
		//frame.display_performance(180);
		
		gui.new_frame(frame_time);
		
		scene.render(frame_time);
		
		//text._font_atlas.bind();
		//text.graphics_representation->update_matrix();
		//text.graphics_representation->update_uniforms();
		//text.graphics_representation->draw(false);

		gui.box(vec2(0, 0), vec2(1920, 30), navbar_backround, true);		// topbar

		// File Button
		auto& dropdown = gui.box(vec2(100, 0), vec2(60, 30), simple_button, true);
		if (dropdown.click_released())
			show_dropdown = !show_dropdown;
		gui.layout(vec2(100, 0 + 30), vec2(0, 0), navbar_backround, Layout::Vertical, show_dropdown);
		gui.change_layout_z(1);
		if (gui.content(vec2(180, 30), dropdown_button, true).click_released()) {
			std::cout << "1 pressed" << std::endl;
		}
		gui.content(vec2(180, 30), dropdown_button, true);
		gui.content(vec2(180, 30), dropdown_button, true);
		gui.content(vec2(180, 30), dropdown_button, true);
		gui.content(vec2(180, 30), dropdown_button, true);
		gui.content(vec2(180, 30), dropdown_button, true);
		gui.layout_end();

		// Scene Viewer
		gui.layout(vec2(0, 40), vec2(200, 100), navbar_backround, Layout::Vertical, true);
		gui.content(vec2(200, 30), navbar_backround, true);
		gui.content(vec2(200, 30), scene_elements, true);
		gui.content(vec2(200, 30), scene_elements, true);
		gui.content(vec2(200, 30), scene_elements, true);
		gui.content(vec2(200, 30), scene_elements, true);
		gui.content(vec2(200, 30), scene_elements, true);
		gui.content(vec2(200, 30), scene_elements, true);
		auto& scene_viewer_panel = gui.layout_end();
		scene_viewer_panel.overwrite_style.border_color = gui::color(0xff0000);
		scene_viewer_panel.overwrite_style.border_thickness = vec4(1);


		// Movable Panel
		gui.layout(panel_location, vec2(100, 100), navbar_backround, Layout::Vertical, true);
		auto& panel_holder = gui.content(vec2(200, 30), navbar_backround);
		gui.content(vec2(200, 100), panel_elements);
		gui.content(vec2(200, 100), panel_elements);
		gui.content(vec2(200, 100), panel_elements);
		gui.layout_end();
		panel_holder.overwrite_style.on_hover.clear();
		if (panel_holder.click_holding()) {
			panel_holder.overwrite_style.on_hover.cursor_type = Frame::Crosshair;
			panel_location = vec2(frame.get_cursor_position().x, frame.get_cursor_position().y) + vec2(-100, -10);
		}



	}
}