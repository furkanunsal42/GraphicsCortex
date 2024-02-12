#pragma once
#include "Editor.h"
#include <string>
#include <unordered_set>

class ScenePanel : public UILayer {

	Style layout_style;
	Style body_style;
	Style header_style;

	Style entry_style;
	Style selected_entry_style;

	vec2 position = vec2(0, 30);
	vec2 size = vec2(400, 800);
	int row_height = 20;

	Style add_menu_layout_style;
	Style add_menu_header_style;
	Style add_menu_body_style;
	Style add_menu_entry_style;

	vec2 add_menu_position;
	vec2 add_menu_size = vec2(200, 200);
	bool add_menu_enabled = false;

	void init() {
		// panel
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
		entry_style.on_hover.color = gui::colorcode(0x303030);
		entry_style.text_color = vec3(1, 1, 1);
		entry_style.text_size = 10;
		entry_style.text_allign_type = Style::TextAlligning::CenterY;

		selected_entry_style = entry_style;
		selected_entry_style.color = gui::colorcode(0x2566e8);
		selected_entry_style.on_hover.color = gui::colorcode(0x2566e8);

		// add menu
		add_menu_layout_style.color = gui::colorcode(0x242424);
		add_menu_layout_style.corner_rounding = vec4(10, 10, 10, 10);
		add_menu_layout_style.padding = vec4(5, 10, 10, 10);
		add_menu_layout_style.border_color = gui::colorcode(0x181818);
		add_menu_layout_style.border_thickness = vec4(1, 1, 2, 2);

		add_menu_header_style.color = gui::colorcode(0x242424);
		add_menu_header_style.text_allign_type = Style::CenterY;
		add_menu_header_style.margin = vec4(0, 10, 0, 0);
		add_menu_header_style.text_color = vec3(1, 1, 1);
		add_menu_header_style.text_size = 10;

		add_menu_body_style.color = gui::colorcode(0x202020);
		add_menu_body_style.padding = vec4(5, 5, 5, 5);
		add_menu_body_style.border_color = gui::colorcode(0x181818);
		add_menu_body_style.border_thickness = vec4(2, 2, 1, 1);
		add_menu_body_style.corner_rounding = vec4(10, 10, 10, 10);

		add_menu_entry_style.color = gui::colorcode(0x202020);
		add_menu_entry_style.on_hover.color = gui::colorcode(0x303030);
		add_menu_entry_style.text_color = vec3(1, 1, 1);
		add_menu_entry_style.text_size = 10;
		add_menu_entry_style.on_hover.cursor_type = Frame::Hand;
		add_menu_entry_style.text_allign_type = Style::TextAlligning::CenterY;

	}

	std::vector<Graphic>							selected_graphics;
	std::vector<std::shared_ptr<AmbiantLight>>		selected_ambiant_lights;
	std::vector<std::shared_ptr<DirectionalLight>>	selected_directional_lights;
	std::vector<std::shared_ptr<PointLight>>		selected_point_lights;
	std::vector<std::shared_ptr<SpotLight>>			selected_spot_lights;
	std::vector<std::shared_ptr<Object>>			selected_objects;
	std::vector<std::shared_ptr<Vehicle>>			selected_vehicles;
	std::vector<std::shared_ptr<Text>>				selected_texts;

	void clear_selection() {
		selected_graphics.clear();
		selected_ambiant_lights.clear();
		selected_directional_lights.clear();
		selected_point_lights.clear();
		selected_spot_lights.clear();
		selected_objects.clear();
		selected_vehicles.clear();
		selected_texts.clear();
	}

	void on_gui_render() {
		Editor& editor = Editor::get();
			
		_widget_info& layout_info = editor.gui->layout("layout_panel_layout", position, size-vec2(20, 50), layout_style);
		bool clicked_on_panel = layout_info.is_hovering && editor.frame->get_mouse_state(Frame::CursorState::LeftReleased);
		bool right_clicked_on_panel = layout_info.is_hovering && editor.frame->get_mouse_state(Frame::CursorState::RightReleased);

		editor.gui->content("layout_panel_name", vec2(size.x-20, 35), header_style, U"Scene Panel");
		editor.gui->layout_content("layout_panel_body", size-vec2(20, 50) - vec2(10, 10), body_style);
		vec2 body_size = size - vec2(20, 50)-vec2(10, 10);

		bool clicked_on_button = false;
		int i = 0;
		for (Graphic& graphic : editor.get_current_scene()->_graphics) {
			Style& style_to_use = (std::find(selected_graphics.begin(), selected_graphics.end(), graphic) != selected_graphics.end()) ? selected_entry_style : entry_style;
			if (editor.gui->content("scene_panel_graphic_button" + i++, vec2(body_size.x, row_height), style_to_use, U"Graphic").is_hovering && editor.frame->get_mouse_state(Frame::CursorState::LeftReleased)) {
				if (!editor.frame->get_key_press(Frame::Key::LEFT_CONTROL)) clear_selection();
				selected_graphics.push_back(graphic);
				clicked_on_button = true;
			}
		}

		i = 0;
		for (std::shared_ptr<Light> light : editor.get_current_scene()->_lights) {
			std::u32string name = U"Light";
			Style style_to_use;
			if (dynamic_cast<AmbiantLight*>(light.get())) { name = U"AmbiantLight"; style_to_use = (std::find(selected_ambiant_lights.begin(), selected_ambiant_lights.end(), light) != selected_ambiant_lights.end()) ? selected_entry_style : entry_style; }
			if (dynamic_cast<DirectionalLight*>(light.get())) { name = U"DirectionalLight"; style_to_use = (std::find(selected_directional_lights.begin(), selected_directional_lights.end(), light) != selected_directional_lights.end()) ? selected_entry_style : entry_style; };
			if (dynamic_cast<PointLight*>(light.get())) { name = U"PointLight"; style_to_use = (std::find(selected_point_lights.begin(), selected_point_lights.end(), light) != selected_point_lights.end()) ? selected_entry_style : entry_style; };
			if (dynamic_cast<SpotLight*>(light.get())) { name = U"SpotLight"; style_to_use = (std::find(selected_spot_lights.begin(), selected_spot_lights.end(), light) != selected_spot_lights.end()) ? selected_entry_style : entry_style; };
			

			if (editor.gui->content("scene_panel_light_button" + i++, vec2(body_size.x, row_height), style_to_use, name).is_hovering && editor.frame->get_mouse_state(Frame::CursorState::LeftReleased)) {
				if (!editor.frame->get_key_press(Frame::Key::LEFT_CONTROL)) clear_selection();
				if (dynamic_cast<AmbiantLight*>(light.get())) selected_ambiant_lights.push_back(std::static_pointer_cast<AmbiantLight>(light));
				if (dynamic_cast<DirectionalLight*>(light.get())) selected_directional_lights.push_back(std::static_pointer_cast<DirectionalLight>(light));
				if (dynamic_cast<PointLight*>(light.get())) selected_point_lights.push_back(std::static_pointer_cast<PointLight>(light));
				if (dynamic_cast<SpotLight*>(light.get())) selected_spot_lights.push_back(std::static_pointer_cast<SpotLight>(light));
				clicked_on_button = true;
			}
		}
		
		i = 0;
		for (std::shared_ptr<Object> object : editor.get_current_scene()->_objects) {
			Style& style_to_use = (std::find(selected_objects.begin(), selected_objects.end(), object) != selected_objects.end()) ? selected_entry_style : entry_style;
			if (editor.gui->content("scene_panel_object_button" + i++, vec2(body_size.x, row_height), style_to_use, U"Object").is_hovering && editor.frame->get_mouse_state(Frame::CursorState::LeftReleased)) {
				if (!editor.frame->get_key_press(Frame::Key::LEFT_CONTROL)) clear_selection();
				selected_objects.push_back(object);
				clicked_on_button = true;
			}
		}

		i = 0;
		for (std::shared_ptr<Vehicle> vehicle : editor.get_current_scene()->_vehicles) {
			Style& style_to_use = (std::find(selected_vehicles.begin(), selected_vehicles.end(), vehicle) != selected_vehicles.end()) ? selected_entry_style : entry_style;
			if (editor.gui->content("scene_panel_vehicle" + i++, vec2(body_size.x, row_height), style_to_use, U"Vehicle").is_hovering && editor.frame->get_mouse_state(Frame::CursorState::LeftReleased)) {
				if (!editor.frame->get_key_press(Frame::Key::LEFT_CONTROL)) clear_selection();
				selected_vehicles.push_back(vehicle);
				clicked_on_button = true;
			}
		}

		i = 0;
		for (std::shared_ptr<Text> text : editor.get_current_scene()->_texts) {
			Style& style_to_use = (std::find(selected_texts.begin(), selected_texts.end(), text) != selected_texts.end()) ? selected_entry_style : entry_style;
			if (editor.gui->content("scene_panel_text_button" + i++, vec2(body_size.x, row_height), style_to_use, U"Text").is_hovering && editor.frame->get_mouse_state(Frame::CursorState::LeftReleased)) {
				if (!editor.frame->get_key_press(Frame::Key::LEFT_CONTROL)) clear_selection();
				selected_texts.push_back(text);
				clicked_on_button = true;
			}
		}

		editor.gui->layout_content_end();
		editor.gui->layout_end();


		// add object menu
		if (right_clicked_on_panel) {
			add_menu_position = vec2(editor.frame->get_cursor_position().x, editor.frame->get_cursor_position().y);
			add_menu_enabled = true;
			std::cout << "right click detected" << std::endl;
		}
		if (add_menu_enabled) {
			editor.gui->z_index = 10;
			editor.gui->layout("scene_panel_add_menu", add_menu_position, add_menu_size, add_menu_layout_style);
			editor.gui->layout_content("scene_panel_add_menu_header", vec2(add_menu_size.x - 20 - 20, 25), add_menu_header_style);
			editor.gui->content("scene_panel_add_menu_header_name", vec2(100, 25), add_menu_header_style, U"Create New");
			editor.gui->layout_content_end();

			editor.gui->layout_content("scene_panel_add_menu_body", add_menu_size, add_menu_body_style, Layout::Vertical);
			vec2 body_size = add_menu_size;

			_widget_info info = editor.gui->content("scene_panel_add_menu_graphic", vec2(body_size.x, row_height), add_menu_entry_style, U"Graphic");
			if (info.is_hovering && editor.frame->get_mouse_state(Frame::LeftReleased)) {
			
			}

			info = editor.gui->content("scene_panel_add_menu_ambiant_ligth", vec2(body_size.x, row_height), add_menu_entry_style, U"AmbiantLight");
			if (info.is_hovering && editor.frame->get_mouse_state(Frame::LeftReleased)) {
				std::shared_ptr<AmbiantLight> ambiant_light = std::make_shared<AmbiantLight>(glm::vec3(0.1f));
				editor.get_current_scene()->add(ambiant_light);
			}

			info = editor.gui->content("scene_panel_add_menu_directional_ligth", vec2(body_size.x, row_height), add_menu_entry_style, U"DirectionalLight");
			if (info.is_hovering && editor.frame->get_mouse_state(Frame::LeftReleased)) {
				std::shared_ptr<DirectionalLight> directional_light = std::make_shared<DirectionalLight>(glm::vec3(0, 0, 0), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.8f));
				editor.get_current_scene()->add(directional_light);
			}

			info = editor.gui->content("scene_panel_add_menu_point_ligth", vec2(body_size.x, row_height), add_menu_entry_style, U"PointLight");
			if (info.is_hovering && editor.frame->get_mouse_state(Frame::LeftReleased)) {
				std::shared_ptr<PointLight> point_light = std::make_shared<PointLight>(glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f), 0.3f, 0.2f, 0.0f);
				editor.get_current_scene()->add(point_light);
			}

			info = editor.gui->content("scene_panel_add_menu_spot_ligth", vec2(body_size.x, row_height), add_menu_entry_style, U"SpotLight");
			if (info.is_hovering && editor.frame->get_mouse_state(Frame::LeftReleased)) {
				std::shared_ptr<SpotLight> spot = std::make_shared<SpotLight>(glm::vec3(-2.0f, -1.0f, -2.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.3f, 0.2f, 0.0f, 30);
				editor.get_current_scene()->add(spot);
			}

			editor.gui->layout_end();
		}
		
		if (editor.frame->get_key_press(Frame::DELETE)) {
			for (auto element : selected_graphics) editor.get_current_scene()->remove(element);
			for (auto element : selected_ambiant_lights) editor.get_current_scene()->remove(element);
			for (auto element : selected_directional_lights) editor.get_current_scene()->remove(element);
			for (auto element : selected_point_lights) editor.get_current_scene()->remove(element);
			for (auto element : selected_spot_lights) editor.get_current_scene()->remove(element);
			for (auto element : selected_objects) editor.get_current_scene()->remove(element);
			for (auto element : selected_vehicles) editor.get_current_scene()->remove(element);
			for (auto element : selected_texts) editor.get_current_scene()->remove(element);

			clear_selection();
		}

		if (clicked_on_panel)
			add_menu_enabled = false;
		if (clicked_on_panel && !clicked_on_button)
			clear_selection();
	}

};