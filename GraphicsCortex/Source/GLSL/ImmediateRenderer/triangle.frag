#<fragment shader>

#version 460 core 
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_color;

in vec4 v_fill_color;
in vec4 v_line_color;
in float v_line_thickness;
in vec4 v_position;
in vec4 v_clip_area;
in vec2 v_uv;
in flat uvec2 v_texture_index;
in vec4 v_border_roundness;
in vec4 v_border_thickness;
in vec4 v_border_color0;
in vec4 v_border_color1;
in vec4 v_border_color2;
in vec4 v_border_color3;
in vec4 v_border_shadow_thickness;
in vec4 v_border_shadow_color;
	
void main(){
	
	if (v_texture_index != uvec2(0, 0))
		frag_color = vec4(texture(sampler2D(v_texture_index), v_uv)) * v_border_color0; //v_fill_color;
	else
		frag_color = v_border_color0;
 }

