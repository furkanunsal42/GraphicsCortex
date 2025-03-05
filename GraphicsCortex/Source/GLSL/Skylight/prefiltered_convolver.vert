#<vertex shader>

#version 460 core
layout (location = 0) in vec3 a_verticies;

out vec3 v_local_position;

void main()
{
    v_local_position = a_verticies;  
    gl_Position =  vec4(v_local_position, 1.0);
}