#<fragment shader>

#version 460 core 
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_color;

in vec3 v_position;
in vec2 v_texcoord;

uniform mat4 projection;

layout (bindless_sampler) uniform sampler2D albedo_texture;
layout (bindless_sampler) uniform sampler2D metalic_roughness_ao_texture;
layout (bindless_sampler) uniform sampler2D brdf_texture;
layout (bindless_sampler) uniform sampler2D emmisive_texture;
layout (bindless_sampler) uniform sampler2D position_texture;
layout (bindless_sampler) uniform sampler2D texcoord_texture;
layout (bindless_sampler) uniform sampler2D normal_texture;

void main(){
	vec4 albedo_color = texture(albedo_texture, v_texcoord);
	frag_color = vec4(albedo_color.xyz, 1);

}
