#<vertex shader>

#version 460 core
layout (location = 0) in vec3 a_verticies;

out vec3 texture_coordinates;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    texture_coordinates = a_verticies;
    vec4 position = projection * mat4(mat3(view)) * vec4(a_verticies, 1.0);
    gl_Position = vec4(position.x, position.y, position.w, position.w);
}  