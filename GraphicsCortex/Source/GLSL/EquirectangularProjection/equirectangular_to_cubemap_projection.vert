#<vertex shader>

#version 460 core
layout (location = 0) in vec3 a_verticies;

out vec3 local_position;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    local_position = a_verticies;  
    gl_Position =  projection * mat4(mat3(view)) * vec4(local_position, 1.0);
}