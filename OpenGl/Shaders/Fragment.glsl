
#<fragment shader>

#version 330 core
layout(location = 0) out vec4 frag_color;
in vec2 tex_coords;
in vec3 frag_normal;
uniform vec3 light_sun;
uniform vec4 ambiant_light;
uniform sampler2D texture_slot;
void main(){
	frag_color = texture(texture_slot, tex_coords) * (ambiant_light + max(dot(-light_sun, frag_normal), 0));
}