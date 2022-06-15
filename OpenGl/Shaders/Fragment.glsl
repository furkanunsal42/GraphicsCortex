
#<fragment shader>

#version 330 core
layout(location = 0) out vec4 frag_color;
in vec3 color;
in vec2 tex_coords;

uniform vec4 u_color;
uniform sampler2D texture_slot;
void main(){
	frag_color = texture(texture_slot, tex_coords) + vec4(color.x, color.y, color.z, 1.0f) + u_color;
}