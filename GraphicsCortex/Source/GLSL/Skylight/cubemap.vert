#<vertex shader>

#version 460 core
layout (location = 0) in vec3 a_verticies;

out vec3 texture_coordinates;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    texture_coordinates = a_verticies;
    gl_Position = projection * mat4(mat3(view)) * vec4(a_verticies, 1.0);
}  