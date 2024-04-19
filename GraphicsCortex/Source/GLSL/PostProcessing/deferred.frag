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

// temp constant directional light
vec3 directional_light_color = vec3(1, 1, 1);
vec3 directional_light_dir = normalize(vec3(-0.2, -1, -0.2));

uniform vec3 camera_coords;

void main(){
	
	vec4 albedo_color = texture(albedo_texture, v_texcoord);
	vec3 world_position = texture(position_texture, v_texcoord).xyz; 
	vec3 world_normal = texture(normal_texture, v_texcoord).xyz;
	world_normal = normalize(world_normal);

	vec3 view_vector = normalize(world_position - camera_coords);

	vec3 directional_diffuse_light = dot(world_normal, -directional_light_dir) * directional_light_color;
	vec3 halfway_vector = normalize((-view_vector) + (-directional_light_dir));
	vec3 directional_specular_light = pow(dot(world_normal, halfway_vector), 32) * directional_light_color;

	vec3 light = directional_specular_light + directional_diffuse_light;
	vec3 color = light * albedo_color.xyz;

	frag_color = vec4(color, 1);
}
