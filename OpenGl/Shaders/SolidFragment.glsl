
#<fragment shader>

#version 330 core
layout(location = 0) out vec4 frag_color;
in vec2 tex_coords;
in vec3 frag_normal;
in vec3 frag_space_coord;

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

uniform sampler2D texture_slot;

// lights
uniform ambiant_light a_lights[10];
uniform directional_light d_lights[10];
uniform point_light p_lights[10];

vec4 calculate_ambiant_light(vec3 ambiant){
	return texture(texture_slot, tex_coords) * vec4(ambiant, 1.0f);
}

vec4 calculate_directional_light(vec3 light_direction, vec3 light_color, vec3 normal){
	vec4 color = texture(texture_slot, tex_coords) * vec4(light_color, 1) * (max(dot(-light_direction, normal), 0));
	return color;
}

vec4 calculate_point_light_intensity(vec3 light_position, vec3 light_color, vec3 current_position, vec3 normal, vec3 light_attenuation){
	vec3 light_direction = current_position - light_position;
	float distance = length(light_direction);
	light_direction = normalize(light_direction);
	vec4 color = calculate_directional_light(light_direction, light_color, normalize(normal));
	float attenuation = light_attenuation.x + light_attenuation.y *distance + light_attenuation.z*pow(distance, 2);
	if (attenuation == 0)
		attenuation = 1;
	return vec4(vec3(color.x, color.y, color.z) / attenuation, color.w);
}
void main(){
	frag_color= calculate_ambiant_light(a_lights[0].color) +  
				calculate_directional_light(d_lights[0].direction, d_lights[0].color, frag_normal) + 
				calculate_point_light_intensity(p_lights[0].position, p_lights[0].color, frag_space_coord, frag_normal, vec3(p_lights[0].constant_term, p_lights[0].linear_term, p_lights[0].exponential_term));
}