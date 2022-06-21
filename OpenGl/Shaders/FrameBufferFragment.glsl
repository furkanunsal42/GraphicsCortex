#<fragment shader>

#version 330 core 

layout (location = 0) out vec4 frag_color;
in vec2 frag_tex_coords;
uniform sampler2D texture_slot;

void main(){
	frag_color = texture(texture_slot, frag_tex_coords);
}
