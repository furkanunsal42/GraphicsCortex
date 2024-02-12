#<vertex shader>

#version 460 core
#extension GL_ARB_bindless_texture : require

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coords;
layout(location = 2) in float texture_index_color;
layout(location = 3) in float textrue_index_specular;
layout(location = 3) in float textrue_index_normal;
layout(location = 3) in vec3 normal;


out vec2 tex_coords;
out vec2 tex_indicies;
out float tex_index_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	tex_coords = texture_coords;
	tex_indicies = texture_coords;
	tex_index_color = int(texture_index_color);
	gl_Position = projection * view * model * vec4(position, 1.0f);
}