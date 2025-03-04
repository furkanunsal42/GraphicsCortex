#<fragment shader>

#version 460 core
out vec4 fragment_color;

in vec3 texture_coordinates;

uniform samplerCube cubemap_texture;

void main()
{    
    fragment_color = texture(cubemap_texture, normalize(texture_coordinates));
}