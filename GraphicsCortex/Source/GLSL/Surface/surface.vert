#<vertex shader>
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texture_coordinates;
layout(location = 2) in vec3 a_normal;

out VSOut{
	vec3 position;
	vec2 texcoord;
	vec3 normal;
} o;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	o.position = a_position;
	o.texcoord = a_texture_coordinates;
	o.normal = a_normal;
}