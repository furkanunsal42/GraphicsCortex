#<vertex shader>

#version 330 core

layout(location = 0) in vec2 vertex_position;

uniform vec2 screen_position;
uniform mat4 projection;

void main(){
	gl_Position = projection * vec4(screen_position + vertex_position, 0.0f, 1.0f);
}