#<vertex shader>
#version 460 core
#extension GL_ARB_bindless_texture : require

const int fill_color_slot		= 0;
const int line_color_slot		= 1;
const int line_thickness_slot	= 2;
const int position_slot			= 3;
const int clip_area_slot		= 4;
const int uv_slot				= 5;
const int texture_index_slot	= 6;

layout (location = fill_color_slot)		in vec4 a_fill_color;
layout (location = line_color_slot)		in vec4 a_line_color;
layout (location = line_thickness_slot)	in vec4 a_line_thickness;
layout (location = position_slot)		in vec4 a_position;
layout (location = clip_area_slot)		in vec4 a_clip_area;
layout (location = uv_slot)				in vec2 a_uv;
layout (location = texture_index_slot)	in int a_texture_index;

out vec4 v_fill_color;
out vec4 v_line_color;
out float v_line_thickness;
out vec4 v_position;
out vec4 v_clip_area;
out vec2 v_uv;
out int v_texture_index;

uniform mat4 view;
uniform mat4 projection;

void main(){
	
	v_fill_color = a_fill_color;
	v_line_color = a_line_color;
	v_line_thickness = a_line_thickness.x;
	v_position = a_position;
	v_clip_area = a_clip_area;
	v_uv = a_uv;
	v_texture_index = a_texture_index;

	gl_Position = projection * view * a_position;
}