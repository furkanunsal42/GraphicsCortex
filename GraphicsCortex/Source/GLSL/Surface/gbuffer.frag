#<fragment shader>

#version 460 core 
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_world_position;
layout (location = 1) out vec4 frag_texcoord;
layout (location = 2) out vec4 frag_world_normal;

layout (location = 3) out vec4 frag_albedo;
layout (location = 4) out vec4 frag_tbn_normal;

layout(std140) uniform textures{
	sampler2D bindless_textures[1024 * 4];
};

layout(std140) uniform pbr_material{
	bool use_albedo_map;
	bool use_normal_map;
	bool use_metalic_roughness_map;
	bool use_brdf_map;
	bool use_emmisive;
	bool use_ambient_occlusion;

	vec4 const_albedo;
	vec4 const_metalic_roughness;
	vec4 const_emmisive;
	
	sampler2D albedo;
	sampler2D normal;
	sampler2D metalic_roughness;
	sampler2D brdf;
	sampler2D emmisive;
	sampler2D ambient_occlusion;
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
