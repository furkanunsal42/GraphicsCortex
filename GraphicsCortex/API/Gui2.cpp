#include "Gui.h"
#include "Default_Programs.h"

// new implementation of gui

std::shared_ptr<Program> Gui2::gui_program;
std::shared_ptr<Font> Gui2::_font;

Gui2::Gui2(Frame& frame) : 
	frame_ref(frame)
{
	gui_program = default_program::gui_program_s().obj;
	_font = std::make_shared<Font>("Fonts\\Roboto-Thin.ttf", 32);
}

void Gui2::box(unsigned int id, vec2 position, vec2 size, Style style, std::u32string text) {
	AABB2 aabb(position, size);
	Graphic graphic;
	graphic.load_model(Mesh_s());

	graphic.mesh->load_model(aabb.generate_model());
	graphic.load_program(Gui2::gui_program);

	float current_z_index = 0;
	vec4 border_thickness(0, 0, 0, 0);
	vec3 border_color(0.5, 0.5, 0.5);
	vec4 corner_rounding(0, 0, 0, 0);
	vec3 color(0.5, 0.5, 0.5);

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

void Gui2::new_frame(Time frame_time) {
	camera.perspective = false;
	camera.projection_matrix = glm::ortho(0.0f, (float)frame_ref.window_width, 0.0f, (float)frame_ref.window_height, -100.0f, 100.0f);

}