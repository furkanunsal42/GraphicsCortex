#<fragment shader>

#version 330 core 

layout (location = 0) out vec4 frag_color;
in vec3 tex_coords;

uniform sampler2DArray texture_array;
void main(){
	frag_color = texture(texture_array, vec3(tex_coords.xyz));
 }
