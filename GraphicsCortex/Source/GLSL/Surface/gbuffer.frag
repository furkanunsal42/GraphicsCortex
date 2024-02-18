#<fragment shader>

#version 460 core 
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_world_position;
layout (location = 1) out vec4 frag_texcoord;
layout (location = 2) out vec4 frag_world_normal;

layout(std140) uniform textures{
	sampler2D bindless_textures[1024 * 4];
};

in vec3 v_position;
in vec2 v_texcoord;
in vec3 v_normal;
in mat3 frag_TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	frag_world_position = vec4(v_position, 1);
	frag_texcoord = vec4(v_texcoord, 0, 1);
	frag_world_normal = vec4(v_normal, 1);
}
