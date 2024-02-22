#<fragment shader>

#version 460 core 
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_world_position;
layout (location = 1) out vec4 frag_texcoord;
layout (location = 2) out vec4 frag_world_normal;

layout (location = 3) out vec4 frag_albedo;
layout (location = 4) out vec4 frag_metalic_roughness_ao;
layout (location = 5) out vec4 frag_brdf;
layout (location = 6) out vec4 frag_emmisive;

layout(std140) uniform textures{
	sampler2D bindless_textures[1024 * 4];
};

layout(std140) uniform pbr_material{

	bool use_albedo_map;
	bool use_normal_map;
	bool use_metalic_map;
	bool use_roughness_map;
	bool use_brdf_map;
	bool use_emmisive_map;
	bool use_ambient_occlusion_map;
	
	vec4 const_albedo;
	vec4 const_metalic_roughness;
	vec4 const_emmisive;
	
	layout(bindless_sampler) sampler2D albedo;
	layout(bindless_sampler) sampler2D normal;
	layout(bindless_sampler) sampler2D metalic;
	layout(bindless_sampler) sampler2D roughness;
	layout(bindless_sampler) sampler2D brdf;
	layout(bindless_sampler) sampler2D emmisive;
	layout(bindless_sampler) sampler2D ambient_occlusion;
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

	frag_albedo					= use_albedo_map				? texture(albedo, v_texcoord)				: const_albedo;
	frag_metalic_roughness_ao.x = use_metalic_map				? texture(metalic, v_texcoord).x			: const_metalic_roughness.x;
	frag_metalic_roughness_ao.y = use_roughness_map				? texture(roughness, v_texcoord).x			: const_metalic_roughness.y;
	frag_metalic_roughness_ao.z = use_ambient_occlusion_map		? texture(ambient_occlusion, v_texcoord).x	: 0;
	frag_brdf 					= use_brdf_map					? texture(brdf, v_texcoord)					: vec4(0, 0, 0, 0);
	frag_emmisive				= use_emmisive_map				? texture(emmisive, v_texcoord)				: const_emmisive;

	frag_metalic_roughness_ao.a = 1;
	frag_brdf.a = 1;
	frag_emmisive.a = 1;
}
