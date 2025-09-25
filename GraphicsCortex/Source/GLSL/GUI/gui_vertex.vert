#<vertex shader>
#version 460 core

layout (location = 0)		in vec4 a_position_size;
layout (location = 1)		in vec4 a_texcoord_begin_end;
layout (location = 2)		in vec4 a_color0;
layout (location = 3)		in vec4 a_color1;
layout (location = 4)		in vec4 a_color2;
layout (location = 5)		in vec4 a_gradient_direction_z_pad;
layout (location = 6)		in vec4 a_border_thickness;
layout (location = 7)		in vec4 a_border_rounding;
layout (location = 8)		in vec4 a_border_color0;
layout (location = 9)		in vec4 a_border_color1;
layout (location = 10)		in vec4 a_border_color2;
layout (location = 11)		in vec4 a_border_color3;
layout (location = 12)		in vec4 a_shadow_thickness;
layout (location = 13)		in vec4 a_shadow_color;

out vec4 v_position_size;
out vec4 v_texcoord_begin_end;
out vec4 v_color0;
out vec4 v_color1;
out vec4 v_color2;
out vec4 v_gradient_direction_z_pad;
out vec4 v_border_thickness;
out vec4 v_border_rounding;
out vec4 v_border_color0;
out vec4 v_border_color1;
out vec4 v_border_color2;
out vec4 v_border_color3;
out vec4 v_shadow_thickness;
out vec4 v_shadow_color;

void main(){
	
	v_position_size				= a_position_size;
	v_texcoord_begin_end		= a_texcoord_begin_end;
	v_color0					= a_color0;
	v_color1					= a_color1;
	v_color2					= a_color2;
	v_gradient_direction_z_pad	= a_gradient_direction_z_pad;
	v_border_thickness			= a_border_thickness;
	v_border_rounding			= a_border_rounding;
	v_border_color0				= a_border_color0;
	v_border_color1				= a_border_color1;
	v_border_color2				= a_border_color2;
	v_border_color3				= a_border_color3;
	v_shadow_thickness			= a_shadow_thickness;
	v_shadow_color				= a_shadow_color;
}