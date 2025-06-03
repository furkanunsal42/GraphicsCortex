#<fragment shader>

#version 460 core 
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_color;

uniform vec4 color;

void main(){
	frag_color = color;
 }

