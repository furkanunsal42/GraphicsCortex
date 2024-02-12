#<fragment shader>

#version 460 core 
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_color;
in vec2 tex_coords;
in float tex_index_color;

uniform sampler2D textures[128];

void main(){
	vec4 color = texture(textures[int(round(tex_index_color))], tex_coords);

	frag_color = color;
 }
