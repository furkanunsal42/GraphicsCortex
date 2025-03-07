#<vertex shader>

#version 460 core
layout (location = 0) in vec3 a_verticies;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 v_local_position;

void main()
{
    v_local_position = a_verticies;  
    gl_Position =  projection * view * model * vec4(v_local_position, 1);
}