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

void main(){
	
	if (v_texture_index != uvec2(0, 0))
		frag_color = vec4(texture(sampler2D(v_texture_index), v_uv).xyzw) * v_fill_color;
	else
		frag_color = v_fill_color;
 }

