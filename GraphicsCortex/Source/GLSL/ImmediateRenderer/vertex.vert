#<vertex shader>
#version 460 core
#extension GL_ARB_bindless_texture : require

const int fill_color_slot				= 0;
const int line_color_slot				= 1;
const int line_thickness_slot			= 2;
const int position_slot					= 3;
const int clip_area_slot				= 4;
const int uv_slot						= 5;
const int texture_index_slot			= 6;
const int border_roundness_slot			= 7;
const int border_thickness_slot			= 8;
const int border_color0_slot			= 9;
const int border_color1_slot			= 10;
const int border_color2_slot			= 11;
const int border_color3_slot			= 12;
const int border_shadow_thickness_slot	= 13;
const int border_shadow_color_slot		= 14;
	
layout (location = fill_color_slot)					in vec4 a_fill_color;
layout (location = line_color_slot)					in vec4 a_line_color;
layout (location = line_thickness_slot)				in vec4 a_line_thickness;
layout (location = position_slot)					in vec4 a_position;
layout (location = clip_area_slot)					in vec4 a_clip_area;
layout (location = uv_slot)							in vec2 a_uv;
layout (location = texture_index_slot)				in uvec2 a_texture_index;
layout (location = border_roundness_slot)			in vec4 a_border_roundness;
layout (location = border_thickness_slot)			in vec4 a_border_thickness;
layout (location = border_color0_slot)				in vec4 a_border_color0;
layout (location = border_color1_slot)				in vec4 a_border_color1;
layout (location = border_color2_slot)				in vec4 a_border_color2;
layout (location = border_color3_slot)				in vec4 a_border_color3;
layout (location = border_shadow_thickness_slot)	in vec4 a_border_shadow_thickness;
layout (location = border_shadow_color_slot)		in vec4 a_border_shadow_color;

out vec4 v_fill_color;
out vec4 v_line_color;
out float v_line_thickness;
out vec4 v_position;
out vec4 v_clip_area;
out vec2 v_uv;
out flat uvec2 v_texture_index;
out vec4 v_border_roundness;
out vec4 v_border_thickness;
out vec4 v_border_color0;
out vec4 v_border_color1;
out vec4 v_border_color2;
out vec4 v_border_color3;
out vec4 v_border_shadow_thickness;
out vec4 v_border_shadow_color;
	
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
	v_border_roundness = a_border_roundness;
	v_border_thickness = a_border_thickness;
	v_border_color0 = a_border_color0;
	v_border_color1 = a_border_color1;
	v_border_color2 = a_border_color2;
	v_border_color3 = a_border_color3;
	v_border_shadow_thickness = a_border_shadow_thickness;
	v_border_shadow_color = a_border_shadow_color;

	gl_Position = projection * view * a_position;
}