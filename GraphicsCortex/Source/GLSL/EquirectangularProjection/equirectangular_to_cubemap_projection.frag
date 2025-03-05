#<fragment shader>

#version 460 core
out vec4 FragColor;
in vec3 g_local_position;

uniform sampler2D equirectangular_texture;

const vec2 inv_atan = vec2(0.1591, 0.3183);
vec2 cube_to_equirectangular(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= inv_atan;
    uv += 0.5;
    return uv;
}

void main()
{		
    vec2 uv = cube_to_equirectangular(normalize(g_local_position)); // make sure to normalize localPos
    vec3 color = min(texture(equirectangular_texture, uv).rgb, vec3(10000));
    
    FragColor = vec4(color, 1.0);
}