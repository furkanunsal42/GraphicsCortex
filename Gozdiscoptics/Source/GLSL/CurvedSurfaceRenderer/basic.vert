#<vertex shader>
#version 460 core

const int vertex_slot = 0;

layout(location = vertex_slot)	in vec3 a_position;

out vec3 v_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	v_position = a_position;
	
	gl_Position = projection * view * model * vec4(a_position, 1.0f);
}