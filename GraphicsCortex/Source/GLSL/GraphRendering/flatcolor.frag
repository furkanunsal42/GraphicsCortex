#<fragment shader>

#version 460 core 
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_color;

uniform vec4 color = glm::vec4(1, 0, 1, 1);

void main(){
	frag_color = color;
 }

