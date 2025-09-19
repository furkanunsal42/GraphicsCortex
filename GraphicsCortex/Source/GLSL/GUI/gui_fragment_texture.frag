a#<fragment shader>

#version 460 core 

layout (location = 0) out vec4 frag_color;

layout(binding = 0) uniform sampler2D source_texture;

in vec3 v_position;
in vec2 v_texcoord;
in vec4 v_color;

void main(){
	frag_color = texture(source_texture, v_texcoord);
 }

