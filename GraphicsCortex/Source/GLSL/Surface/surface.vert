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

layout (location = vertex_slot) in vec3 a_verticies;
layout (location = normal_slot) in vec3 a_normal;
layout (location = tangent_slot) in vec3 a_tangent;
layout (location = bitangent_slot) in vec3 a_bitangent;
layout (location = uv0_slot) in vec2 a_texture_coordinates;

out vec2 v_texture_coordinates;
out vec3 v_world_position;
out vec3 v_view_position;
out vec3 v_normal;
out vec3 v_tangent;
out vec3 v_bitangent;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    v_texture_coordinates = a_texture_coordinates;
    v_world_position = vec3(model * vec4(a_verticies, 1.0));
    v_normal = normalize(mat3(model) * a_normal);   
    v_tangent = normalize(mat3(model) * a_tangent);   
    v_bitangent = normalize(mat3(model) * a_bitangent);   
    v_view_position = vec3(view * model * vec4(a_verticies, 1.0));

    gl_Position =  projection * view * vec4(v_world_position, 1.0);
}