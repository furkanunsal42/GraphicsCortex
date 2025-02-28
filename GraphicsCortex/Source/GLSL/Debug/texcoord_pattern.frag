#<fragment shader>

#version 460 core 
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_color;

layout(std140) uniform textures{
	sampler2D bindless_textures[1];
};

in vec2 v_texcoord;

const vec4 color_odd  = vec4(vec3(0.9), 1);
const vec4 color_even = vec4(vec3(0.5), 1);

void main(){
	
	ivec2 idx = ivec2(v_texcoord * 32);
	vec4 color = (idx.x + idx.y) % 2 == 0 ? color_even : color_odd;
	frag_color = color;

 }

