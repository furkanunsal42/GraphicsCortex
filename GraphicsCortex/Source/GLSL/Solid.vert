#<vertex shader>

#version 330 core
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec2 v_texture_coordinates;
layout(location = 2) in vec3 v_normal;

out DATA
{
	vec2 tex_coords;
	vec3 frag_normal;
	vec3 frag_space_coord;
	mat4 projection;
	mat4 model;
	mat4 view;
} data_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	data_out.frag_normal = vec3(model * vec4(v_normal, 0.0f));
	data_out.tex_coords = v_texture_coordinates;
	data_out.frag_space_coord = vec3( model * vec4(v_position, 1.0f));
	data_out.projection = projection;
	data_out.model = model;
	data_out.view = view;

	gl_Position = model * vec4(v_position.x, v_position.y, v_position.z, 1);
}
