#include "GraphicsCortex.h"

std::unordered_map<std::string, Style> define_styles() {
	std::unordered_map<std::string, Style> styles;
	styles["simple_button"].color = gui::colorcode(0x383838);
	styles["simple_button"].corner_rounding = Persentage(0.2f, Persentage::MIN_SIZE_DIM);
	styles["simple_button"].on_hover.cursor_type = Frame::Hand;
	styles["simple_button"].on_hover.color = gui::colorcode(0x424242);
	styles["simple_button"].color_change = 0.1;
	styles["simple_button"].color_interpolation = Interpolation::polynomial(3);

	styles["dropdown_button"].color = gui::colorcode(0x3F855A);
	styles["dropdown_button"].on_hover.color = gui::colorcode(0x475F95);
	styles["dropdown_button"].color_change = 0.1;
	styles["dropdown_button"].color_interpolation = Interpolation::polynomial(3);
	styles["dropdown_button"].cursor_type = Frame::Hand;

	styles["navbar_backround"].color = gui::colorcode(0x1E1E1E);

	styles["scene_elements"].color = gui::colorcode(0x383838);
	styles["scene_elements"].border_color = gui::colorcode(0x383838);
	styles["scene_elements"].on_hover.border_color = gui::colorcode(0x475F95);
	styles["scene_elements"].border_thickness = vec4(1);
	styles["scene_elements"].border_color_change = 0.1;
	styles["scene_elements"].border_color_interpolation = Interpolation::polynomial(3);

	styles["panel_elements"].color = gui::colorcode(0x383838);
	styles["panel_elements"].on_hover.color = gui::colorcode(0x475F95);
	styles["panel_elements"].margin = vec4(10);
	styles["panel_elements"].on_hover.corner_rounding = vec4(50);
	styles["panel_elements"].corner_rounding_change = 0.4;
	styles["panel_elements"].corner_rounding_interpolation = Interpolation::polynomial(3);


	
	return styles;
}

void gui_draw_topbar(Gui& gui, std::unordered_map<std::string, Style>& styles) {
	
	gui.box(vec2(0, 0), vec2(1920, 30), styles["navbar_backround"], true);		// topbar

	// File Button
	auto& dropdown = gui.box(vec2(100, 0), vec2(60, 30), styles["simple_button"], true);
	if (dropdown.click_released())
		dropdown.data["show"] = !(bool)dropdown.data["show"];
	gui.layout(vec2(100, 0 + 30), vec2(0, 0), styles["navbar_backround"], Layout::Vertical, dropdown.data["show"]);
	gui.change_layout_z(1);
	if (gui.content(vec2(180, 30), styles["dropdown_button"], true).click_released()) {
		std::cout << "1 pressed" << std::endl;
	}
	gui.content(vec2(180, 30), styles["dropdown_button"], true);
	gui.content(vec2(180, 30), styles["dropdown_button"], true);
	gui.content(vec2(180, 30), styles["dropdown_button"], true);
	gui.content(vec2(180, 30), styles["dropdown_button"], true);
	gui.content(vec2(180, 30), styles["dropdown_button"], true);
	gui.layout_end();
}

void gui_draw_scene_viewer(Gui& gui, std::unordered_map<std::string, Style>& styles) {

	// Scene Viewer
	gui.layout(vec2(0, 40), vec2(200, 100), styles["navbar_backround"], Layout::Vertical, true);
	gui.content(vec2(200, 30), styles["navbar_backround"], true);
	gui.content(vec2(200, 30), styles["scene_elements"], true);
	gui.content(vec2(200, 30), styles["scene_elements"], true);
	gui.content(vec2(200, 30), styles["scene_elements"], true);
	gui.content(vec2(200, 30), styles["scene_elements"], true);
	gui.content(vec2(200, 30), styles["scene_elements"], true);
	gui.content(vec2(200, 30), styles["scene_elements"], true);
	auto& scene_viewer_panel = gui.layout_end();
	scene_viewer_panel.overwrite_style.border_color = gui::color(0xff0000);
	scene_viewer_panel.overwrite_style.border_thickness = vec4(1);
}

void gui_draw_moveable_panel(Gui& gui, std::unordered_map<std::string, Style>& styles) {
	
	// Movable Panel
	auto& size_variable = gui.box(vec2(), vec2(), styles[""], false);
	size_variable.initialize_data("position_x", 100);
	size_variable.initialize_data("position_y", 100);
	gui.layout(vec2(size_variable.data["position_x"], size_variable.data["position_y"]), vec2(100, 100), styles["navbar_background"], Layout::Vertical, true);
	gui.change_layout_z(2);
	auto& panel_holder = gui.content(vec2(200, 30), styles["navbar_background"]);
	gui.content(vec2(200, 100), styles["panel_elements"]);
	gui.content(vec2(200, 100), styles["panel_elements"]);
	gui.content(vec2(200, 100), styles["panel_elements"]);
	auto& layout = gui.layout_end();
	layout.render();
	panel_holder.overwrite_style.on_hover.clear();
	if (panel_holder.click_holding()) {
		panel_holder.overwrite_style.on_hover.cursor_type = Frame::Crosshair;
		size_variable.data["position_x"] = gui.get_frame_referance().get_cursor_position().x - 100;
		size_variable.data["position_y"] = gui.get_frame_referance().get_cursor_position().y - 10;
	}
}

int main() {
	Frame frame(1920, 1080, "GraphicsCortex", 16, 0, true, true, true, false);
	Scene scene(frame);
	Gui gui(frame);

	auto styles = define_styles();

	Style layout_style;
	layout_style.color = gui::colorcode(0x0c0c0c);
	
	Style element1;
	element1.color = gui::colorcode(0x2f2f2f);
	element1.margin = Persentage(100, Persentage::MAX_SIZE_DIM);

	Font_s font("Fonts\\Roboto-Regular.ttf", 100);
	Text text(font, "File");
	text.set_scale(1080 / 3);
	text.set_wait_for_words(1080);
	text.graphic->set_position(glm::vec3(0, 0, -1));
	text.set_color(vec4(0, 0, 0, 1));
	text.graphic->set_uniform_all(default_program::text_uniform_queue(gui.camera, text));

	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.clear_window(1, 1, 1, 1);
		frame.display_performance(180);
		
		gui.new_frame(frame_time);
		
		scene.render(frame_time);
		
		text.render();
		
		gui_draw_topbar(gui, styles);
		gui_draw_scene_viewer(gui, styles);
		gui_draw_moveable_panel(gui, styles);

		//gui.layout(vec2(100, 100), vec2(100, 100), layout_style);
		//gui.content(vec2(80, 80), element1);
		//gui.layout_end();


	}
}

// positioning through recursive layouts
// custom objects with state
// floating layouts
// mouse hover and click catching
// dynamically changing gui
