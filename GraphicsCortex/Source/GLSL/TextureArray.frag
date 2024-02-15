#<fragment shader>

#version 460 core 
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_color;
in vec2 tex_coords;
in float tex_index_color;

layout(std140) uniform textures{
    sampler2D bindless_textures[1024*4];
};

void main(){
	vec4 color = texture(bindless_textures[int(round(tex_index_color))], tex_coords);
	if (color.a < 0.05) discard;
	frag_color = color;
}
