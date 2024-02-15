#<vertex shader>

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in float texture_index_color;

out float tex_index_color;
out vec2 frag_uv;

uniform mat4 model;

layout(std140) uniform camera_matricies{
	mat4 view;
	mat4 projection;
};

void main(){
	tex_index_color = int(round(texture_index_color));
	frag_uv = uv;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}