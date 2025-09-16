#<vertex shader>
#version 460 core

const int fill_color_slot		= 0;
const int line_color_slot		= 1;
const int line_thickness_slot	= 2;
const int position_slot			= 3;

layout (location = fill_color_slot)		in vec4 a_fill_color;
layout (location = line_color_slot)		in vec4 a_line_color;
layout (location = line_thickness_slot)	in vec4 a_line_thickness;
layout (location = position_slot)		in vec4 a_position;

out vec4 v_fill_color;
out vec4 v_line_color;
out float v_line_thickness;
out vec4 v_position;

void main(){
	
	v_fill_color = a_fill_color;
	v_line_color = a_line_color;
	v_line_thickness = a_line_thickness.x;
	v_position = a_position;

	gl_Position = a_position;
}