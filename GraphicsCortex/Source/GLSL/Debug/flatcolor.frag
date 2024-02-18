#<fragment shader>

#version 460 core 
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_color;

layout(std140) uniform textures{
	sampler2D bindless_textures[1];
};

uniform vec4 color;

void main(){
	frag_color = color;
	if (color == vec4(0, 0, 0, 0))
		frag_color = vec4(1, 0, 1, 1);
 }

