#<vertex shader>

#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texture_coords;

out vec2 frag_tex_coords;
void main(){
	frag_tex_coords = texture_coords;
	gl_Position = vec4(position, 0.0f,  1.0f);
}