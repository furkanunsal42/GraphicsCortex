#<fragment shader>

#version 460 core 

layout (location = 0) out vec4 frag_color;

in vec3 v_position;
in vec2 v_texcoord;
in vec4 v_color;

void main(){
	frag_color = vec4(v_color.xyz, 1);
 }

