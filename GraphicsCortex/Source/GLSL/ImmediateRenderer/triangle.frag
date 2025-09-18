#<fragment shader>

#version 460 core 

layout (location = 0) out vec4 frag_color;

in vec4 v_fill_color;
in vec4 v_line_color;
in float v_line_thickness;
in vec4 v_position;

void main(){
	frag_color = v_fill_color;
 }

