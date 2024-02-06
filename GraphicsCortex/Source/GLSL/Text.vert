#<vertex shader>

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

out vec2 frag_uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	frag_uv = uv;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}