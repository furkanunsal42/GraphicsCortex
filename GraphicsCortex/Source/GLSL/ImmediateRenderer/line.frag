#<fragment shader>

#version 460 core 

layout (location = 0) out vec4 frag_color;

in vec4 v_fill_color;
in vec4 v_line_color;
in float v_line_thickness;
in vec4 v_position;
in vec4 v_clip_area;
in vec2 v_uv;
in uvec2 v_texture_handle;

void main(){
	frag_color = v_line_color;
 }

