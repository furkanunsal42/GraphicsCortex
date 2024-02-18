#<fragment shader>

#version 460 core 
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_color;

layout(std140) uniform textures{
	sampler2D bindless_textures[1];
};

in vec2 v_texcoord;

void main(){
	frag_color = vec4(v_texcoord, 0, 1);
 }

