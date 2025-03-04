#<fragment shader>

#version 460 core
out vec4 fragment_color;

in vec3 texture_coordinates;

uniform samplerCube cubemap_texture;

void main()
{    
    vec3 color = texture(cubemap_texture, normalize(texture_coordinates)).rgb;

    // HDR tonemapping
    float exposure = 1.7;
    color = vec3(1) - exp(-color * exposure);
    
    // gamma correct
    color = pow(color, vec3(1.0/1.8)); 

    fragment_color = vec4(color, 1);
}