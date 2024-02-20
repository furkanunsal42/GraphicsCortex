#<vertex shader>
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texture_coordinates;

out vec3 v_position;
out vec2 v_texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	v_position = a_position;
	v_texcoord = a_texture_coordinates;
	gl_Position = projection * model * vec4(a_position,  1.0f);
}