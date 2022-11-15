#<vertex shader>

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coords;

out vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	tex_coords = texture_coords;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}