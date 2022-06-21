#<vertex shader>

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coords;
layout(location = 2) in vec3 normals;

out vec2 frag_tex_coords;
void main(){
	frag_tex_coords = texture_coords;
	gl_Position = vec4(position,  1.0f);
}