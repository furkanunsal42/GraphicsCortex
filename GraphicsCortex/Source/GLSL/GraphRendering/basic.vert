#<vertex shader>
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texture_coordinates;
layout(location = 2) in vec3 a_normal;

out vec3 v_position;
out vec2 v_texcoord;
out vec3 v_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	v_position = a_position;
	v_texcoord = a_texture_coordinates;
	v_normal = a_normal;
	
	gl_Position = projection * view * model * vec4(a_position, 1.0f);
}