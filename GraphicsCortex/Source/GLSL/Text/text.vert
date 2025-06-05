#<vertex shader>
#version 460 core

const int vertex_slot = 0;
const int normal_slot = 1;
const int tangent_slot = 2;
const int bitangent_slot = 3;
const int uv0_slot = 4;
const int uv1_slot = 5;
const int vertex_color_slot = 6;
const int bone_indicies_slot = 7;
const int bone_weights_slot = 8;

layout(location = vertex_slot)	in vec3 a_position;
layout(location = uv0_slot)		in vec2 a_texture_coordinates;

out vec3 v_position;
out vec2 v_texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	v_position = a_position;
	v_texcoord = a_texture_coordinates;

	gl_Position = projection * view * model * vec4(v_position, 1.0f);
}