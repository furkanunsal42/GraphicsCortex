#include "Gui.h"
#include "Default_Programs.h"

// new implementation of gui

std::shared_ptr<Program> Gui2::gui_program;
std::shared_ptr<Font> Gui2::_font;

Gui2::Gui2(Frame& frame) : 
	frame_ref(frame)
{
	gui_program = default_program::gui_program_s().obj;
	_font = std::make_shared<Font>("Fonts\\Roboto-Regular.ttf", 16);
}

void Gui2::new_frame(Time frame_time) {
	camera.perspective = false;
	camera.projection_matrix = glm::ortho(0.0f, (float)frame_ref.window_width, 0.0f, (float)frame_ref.window_height, -100.0f, 100.0f);
}

void Gui2::box(unsigned int id, vec2 position, vec2 size, Style style, std::u32string text) {
	AABB2 aabb(position, size);
	Graphic graphic;
	graphic.load_model(Mesh_s());

	graphic.mesh->load_model(aabb.generate_model());
	graphic.load_program(Gui2::gui_program);

	float current_z_index = 0;
	vec4 border_thickness(1, 1, 1, 1);
	vec3 border_color(0, 0, 0);
	vec4 corner_rounding(0, 0, 0, 0);
	vec3 color = style.color.value;

	gui_program->update_uniform("screen_position", aabb.position.x, frame_ref.window_height - aabb.position.y);
	gui_program->update_uniform("rect_color", color.x, color.y, color.z, 1.0f);
	gui_program->update_uniform("rect_size", aabb.size.x, aabb.size.y);
	gui_program->update_uniform("corner_rounding", corner_rounding.x, corner_rounding.y, corner_rounding.z, corner_rounding.w);
	gui_program->update_uniform("border_color", border_color.x, border_color.y, border_color.z, 1.0f);
	gui_program->update_uniform("border_thickness", border_thickness.x, border_thickness.y, border_thickness.z, border_thickness.w);
	gui_program->update_uniform("z_index", current_z_index);

	camera.update_default_uniforms(*Gui2::gui_program);
	graphic.update_default_uniforms(*Gui2::gui_program);
	graphic.draw(false);
}

void Gui2::layout(unsigned int id, vec2 position, vec2 min_size, Style style, Layout::LayoutType layout_type) {
	if (current_layout != nullptr) {
		std::cout << "[GUI Error] another Gui::layout() is called without calling Gui::layout_end() for previous Gui::layout() call" << std::endl;
		return;
	}
	
	this->position = position;
	current_layout = std::make_shared<layout_node>(id, min_size, style, layout_type);
	
	if (layout_stack.size() > 0) {
		std::cout << "[GUI Error] Gui::layout() is called but layout_stack is not empty " << std::endl;
	}
	layout_stack.clear();
	layout_stack.push_back(current_layout);
}

void Gui2::content(unsigned int id, vec2 size, Style style, std::u32string text) {
	if (std::shared_ptr<layout_node> node = layout_stack.back().lock()) {
		node->contents.push_back(content_info(id, size, style, text));
		node->self_info.layout.add_widget(size);
	}
	else {
		std::cout << "[GUI Error] Gui::content() is called but no Gui::layout() or Gui::layout_content() was called before" << std::endl;
		return;
	}
}

void Gui2::layout_content(unsigned int id, vec2 min_size, Style style, Layout::LayoutType layout_type) {
	if (std::shared_ptr<layout_node> node = layout_stack.back().lock()) {
		node->childs.push_back(std::make_shared<layout_node>(id, min_size, style, layout_type));
	}
	else {
		std::cout << "[GUI Error] Gui::layout_content() is called but no Gui::layout() or Gui::layout_content() was called before" << std::endl;
		return;
	}
}

void Gui2::layout_content_end() {
	if (std::shared_ptr<layout_node> node = layout_stack.back().lock()){
		layout_stack.pop_back();
		if (std::shared_ptr<layout_node> parent = layout_stack.back().lock()) {
			parent->self_info.layout.add_widget(node->self_info.layout.window_size);
		}
		else {
			std::cout << "[GUI Error] Gui::layout_content_end() is called but no Gui::layout() was called before" << std::endl;
			return;
		}
	}
	else {
		std::cout << "[GUI Error] Gui::layout_content_end() is called but no Gui::layout() or Gui::layout_content() was called before" << std::endl;
		return;
	}
}

void Gui2::layout_end() {
	//if (current_layout == nullptr) {
	//	std::cout << "[GUI Error] Gui::layout_end() is called without corresponding Gui::layout()" << std::endl;
	//	return;
	//}
	//Layout major_layout = Layout(current_layout->layout_type, current_layout->position, vec2(0, 0));
	//
	//// determine positions for contents
	//std::vector<vec2> content_positions;
	//for (int i = 0; i < contents.size(); i++) {
	//	content_info& content = contents[i];
	//	content_positions.push_back(major_layout.get_widget_position());
	//	major_layout.add_widget(content.size);
	//}
	//
	//// force minimum size for layout
	//major_layout.window_size.x = std::max(major_layout.window_size.x, current_layout->min_size.x);
	//major_layout.window_size.y = std::max(major_layout.window_size.y, current_layout->min_size.y);
	//
	//// draw layout
	//box(current_layout->id, current_layout->position, major_layout.window_size, current_layout->style, U"");
	//
	//// draw contents
	//for (int i = 0; i < contents.size(); i++) {
	//	content_info& content = contents[i];
	//	box(content.id, content_positions[i], content.size, content.style, content.text);
	//}
	//contents.clear();
}
