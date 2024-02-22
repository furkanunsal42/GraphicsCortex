#<fragment shader>

#version 460 core 
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_color;
in vec2 tex_coords;
in float tex_index_color;

layout(std140) uniform textures{
    sampler2D bindless_textures[1024*4];
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

//uniform samplerCube cubemap;

void main(){
	vec4 color = texture(bindless_textures[int(0)], tex_coords);
	if (color.a < 0.05) discard;
	
	//color = texture(cubemap, vec3(tex_coords, 0));
	//if (color.a < 0.05) discard;
	
	frag_color = color;
}
