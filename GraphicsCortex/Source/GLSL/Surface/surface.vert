#<vertex shader>
#version 460 core

const int vertex_slot = 0;
const int normal_slot = 1;
const int tangent_slot = 2;
const int uv0_slot = 3;
const int uv1_slot = 4;
const int vertex_color_slot = 5;
const int bone_indicies_slot = 6;
const int bone_weights_slot = 7;

layout(location = vertex_slot)  in vec3 a_position;
layout(location = normal_slot)  in vec3 a_normal;
layout(location = uv0_slot)		in vec2 a_texture_coordinates;

out vec3 v_position;
out vec2 v_texcoord;
out vec3 v_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	v_position = a_position;
	v_texcoord = a_texture_coordinates;
	v_normal = a_normal;
	
	gl_Position = projection * view * model * vec4(a_position, 1.0f);
}