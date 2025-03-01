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

layout (location = vertex_slot) in vec3 aPos;
layout (location = normal_slot) in vec3 aNormal;
layout (location = uv0_slot) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    Normal = transpose(inverse(mat3(model))) * aNormal;   

    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}