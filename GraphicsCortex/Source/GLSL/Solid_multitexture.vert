#<vertex shader>

#version 330 core
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec2 v_texture_coordinates;
layout(location = 2) in float v_map_index_color;
layout(location = 3) in float v_map_index_specular;
layout(location = 4) in float v_map_index_normal;
layout(location = 5) in vec3 v_normal;

out DATA
{
	vec2 tex_coords;
	vec3 frag_normal;
	vec3 frag_space_coord;
	mat4 projection;
	mat4 model;
	mat4 view;
	float map_index_color;
	float map_index_specular;
	float map_index_normal;
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
	data_out.map_index_color = v_map_index_color;
	data_out.map_index_specular = v_map_index_specular;
	data_out.map_index_normal = v_map_index_normal;

	gl_Position = model * vec4(v_position.x, v_position.y, v_position.z, 1);
}
