#<fragment shader>

#version 330 core
layout(location = 0) out vec4 frag_color;
in vec2 tex_coords;
in vec3 frag_normal;
in vec3 frag_space_coord;
in mat3 frag_TBN;

struct ambiant_light{
	vec3 color;
};

struct directional_light{
	vec3 direction;
	vec3 color;
};

struct point_light{
	vec3 position;
	vec3 color;
	float constant_term;
	float linear_term;
	float exponential_term;
};

struct spot_light{
	vec3 position;
	vec3 direction;
	float cos_angle;
	vec3 color;
	float constant_term;
	float linear_term;
	float exponential_term;
};


uniform sampler2D color_map_slot;
uniform sampler2D specular_map_slot;
uniform sampler2D normal_map_slot;

uniform int use_color_map;
uniform int use_specular_map;
uniform int use_normal_map;

uniform vec3 camera_coords;
// lights
uniform ambiant_light a_lights[10];
uniform directional_light d_lights[10];
uniform point_light p_lights[10];
uniform spot_light s_lights[10];

uniform int a_lights_count;
uniform int d_lights_count;
uniform int p_lights_count;
uniform int s_lights_count;

void main(){

	vec3 normal;
	if(bool(use_normal_map))
		normal = normalize(frag_TBN * ((texture(normal_map_slot, tex_coords) * 2).xyz - 1));
	else
		normal = frag_normal;
	frag_color = vec4(normal/2 + 0.5 ,1);
}