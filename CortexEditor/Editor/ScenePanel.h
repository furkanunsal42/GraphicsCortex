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

	enum selectable_object_type {
		graphic,
		ambiant_light,
		directional_light,
		point_light,
		spot_light,
		object,
		vehicle,
		text,
	};

	std::unordered_map<std::shared_ptr<void>, selectable_object_type> selected_pointers;

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
		for (std::shared_ptr<Graphic> graphic : editor.get_current_scene()->_graphics) {
			Style& style_to_use = (selected_pointers.find(graphic) != selected_pointers.end()) ? selected_entry_style : entry_style;
			if (editor.gui->content("scene_panel_graphic_button" + i++, vec2(body_size.x, row_height), style_to_use, U"Graphic").is_hovering && editor.frame->get_mouse_state(Frame::CursorState::LeftReleased)) {
				if (!editor.frame->get_key_press(Frame::Key::LEFT_CONTROL)) selected_pointers.clear();
				selected_pointers.insert(std::pair(graphic, selectable_object_type::graphic));
				clicked_on_button = true;
			}
		}

		i = 0;
		for (std::shared_ptr<Light> light : editor.get_current_scene()->_lights) {
			std::u32string name = U"Light";
			selectable_object_type object_type;
			if (dynamic_cast<AmbiantLight*>(light.get())) { name = U"AmbiantLight"; object_type = selectable_object_type::ambiant_light; }
			if (dynamic_cast<DirectionalLight*>(light.get())) { name = U"DirectionalLight"; object_type = selectable_object_type::directional_light; };
			if (dynamic_cast<PointLight*>(light.get())) { name = U"PointLight"; object_type = selectable_object_type::point_light; };
			if (dynamic_cast<SpotLight*>(light.get())) { name = U"SpotLight"; object_type = selectable_object_type::spot_light; };
			
			Style& style_to_use = (selected_pointers.find(light) != selected_pointers.end()) ? selected_entry_style : entry_style;

			if (editor.gui->content("scene_panel_light_button" + i++, vec2(body_size.x, row_height), style_to_use, name).is_hovering && editor.frame->get_mouse_state(Frame::CursorState::LeftReleased)) {
				if (!editor.frame->get_key_press(Frame::Key::LEFT_CONTROL)) selected_pointers.clear();
				selected_pointers.insert(std::pair(light, object_type));
				clicked_on_button = true;
			}
		}
		
		i = 0;
		for (std::shared_ptr<Object> object : editor.get_current_scene()->_objects) {
			Style& style_to_use = (selected_pointers.find(object) != selected_pointers.end()) ? selected_entry_style : entry_style;
			if (editor.gui->content("scene_panel_object_button" + i++, vec2(body_size.x, row_height), style_to_use, U"Object").is_hovering && editor.frame->get_mouse_state(Frame::CursorState::LeftReleased)) {
				if (!editor.frame->get_key_press(Frame::Key::LEFT_CONTROL)) selected_pointers.clear();
				selected_pointers.insert(std::pair(object, selectable_object_type::object));
				clicked_on_button = true;
			}
		}

		i = 0;
		for (std::shared_ptr<Vehicle> vehicle : editor.get_current_scene()->_vehicles) {
			Style& style_to_use = (selected_pointers.find(vehicle) != selected_pointers.end()) ? selected_entry_style : entry_style;
			if (editor.gui->content("scene_panel_vehicle" + i++, vec2(body_size.x, row_height), style_to_use, U"Vehicle").is_hovering && editor.frame->get_mouse_state(Frame::CursorState::LeftReleased)) {
				if (!editor.frame->get_key_press(Frame::Key::LEFT_CONTROL)) selected_pointers.clear();
				selected_pointers.insert(std::pair(vehicle, selectable_object_type::vehicle));
				clicked_on_button = true;
			}
		}

		i = 0;
		for (std::shared_ptr<Text> text : editor.get_current_scene()->_texts) {
			Style& style_to_use = (selected_pointers.find(text) != selected_pointers.end()) ? selected_entry_style : entry_style;
			if (editor.gui->content("scene_panel_text_button" + i++, vec2(body_size.x, row_height), style_to_use, U"Text").is_hovering && editor.frame->get_mouse_state(Frame::CursorState::LeftReleased)) {
				if (!editor.frame->get_key_press(Frame::Key::LEFT_CONTROL)) selected_pointers.clear();
				selected_pointers.insert(std::pair(text, selectable_object_type::text));
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
				std::shared_ptr<Program> program = default_program::solid_program_s();
				std::shared_ptr<UnorderedMaterial> material = std::make_shared<UnorderedMaterial>(2);
				material->set_texture("Images/GoldBlock.png", 4, 0, UnorderedMaterial::COLOR);
				material->set_texture("Images/full_white.png", 4, 1, UnorderedMaterial::SPECULAR);

				std::shared_ptr<Graphic> graphic = std::make_shared<Graphic>(default_geometry::cube());
				graphic->load_program(program);
				graphic->load_material(material);
				editor.get_current_scene()->add(graphic);
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
			for (std::pair<std::shared_ptr<void>, selectable_object_type> selected_pointer : selected_pointers) {
				if (selected_pointer.second == graphic)				editor.get_current_scene()->remove(std::static_pointer_cast<Graphic>(selected_pointer.first));
				if (selected_pointer.second == ambiant_light)		editor.get_current_scene()->remove(std::static_pointer_cast<AmbiantLight>(selected_pointer.first));
				if (selected_pointer.second == directional_light)	editor.get_current_scene()->remove(std::static_pointer_cast<DirectionalLight>(selected_pointer.first));
				if (selected_pointer.second == point_light)			editor.get_current_scene()->remove(std::static_pointer_cast<PointLight>(selected_pointer.first));
				if (selected_pointer.second == spot_light)			editor.get_current_scene()->remove(std::static_pointer_cast<SpotLight>(selected_pointer.first));
				if (selected_pointer.second == object)				editor.get_current_scene()->remove(std::static_pointer_cast<Object>(selected_pointer.first));
				if (selected_pointer.second == vehicle)				editor.get_current_scene()->remove(std::static_pointer_cast<Vehicle>(selected_pointer.first));
				if (selected_pointer.second == text)				editor.get_current_scene()->remove(std::static_pointer_cast<Text>(selected_pointer.first));
			}
		}

		if (clicked_on_panel)
			add_menu_enabled = false;
		if (clicked_on_panel && !clicked_on_button)
			selected_pointers.clear();
	}

};