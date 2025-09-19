#<vertex shader>
#version 460 core

layout (location = 0)		in vec3 a_position;
layout (location = 1)		in vec2 a_texcoord;
layout (location = 2)		in vec4 a_color;

out vec3 v_position;
out vec2 v_texcoord;
out vec4 v_color;

uniform mat4 projection;

void main(){
	
	v_position = a_position;
	v_texcoord = a_texcoord;
	v_color = a_color;

	gl_Position = projection * vec4(a_position, 1);
}