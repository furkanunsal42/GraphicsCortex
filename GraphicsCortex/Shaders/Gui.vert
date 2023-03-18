#<vertex shader>

#version 330 core

layout(location = 0) in vec2 vertex_position;

uniform vec2 screen_position;
uniform mat4 projection;
uniform float z_index = 0;

void main(){
	gl_Position = projection * vec4(screen_position + vertex_position, z_index, 1.0f);
}