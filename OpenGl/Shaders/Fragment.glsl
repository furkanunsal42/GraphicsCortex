
#<fragment shader>

#version 330 core
layout(location = 0) out vec4 frag_color;
in vec2 tex_coords;
in vec3 frag_normal;
in vec3 frag_light_sun;

uniform sampler2D texture_slot;
void main(){
	frag_color = texture(texture_slot, tex_coords) * dot(frag_light_sun, frag_normal);
}