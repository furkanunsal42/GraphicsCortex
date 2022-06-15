#<vertex shader>

#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 texture_coordinates;
out vec3 color;
out vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	color = vertex_color;
	tex_coords = texture_coordinates;
	gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1);
}