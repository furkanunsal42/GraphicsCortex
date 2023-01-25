#<fragment shader>

#version 330 core 

layout (location = 0) out vec4 frag_color;
in vec3 frag_tex_coords;

uniform samplerCube cubemap;
void main(){
	frag_color = texture(cubemap, frag_tex_coords);
 }
