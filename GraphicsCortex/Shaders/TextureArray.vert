#<vertex shader>

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coords;
layout(location = 2) in vec3 texture_indicies;

out vec2 tex_coords;
out vec3 tex_indicies;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	tex_coords = texture_coords;
	tex_indicies = texture_indicies;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}