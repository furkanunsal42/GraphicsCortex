
#<fragment shader>

#version 330 core
layout(location = 0) out vec4 frag_color;
in vec2 tex_coords;
in vec3 frag_normal;
in vec3 frag_space_coord;

uniform sampler2D texture_slot;

// directional light
//uniform vec3 light_sun;
//uniform vec4 ambiant_light;

uniform vec3 point_light_position;
uniform vec4 point_light_color;

vec4 calculate_base_light(vec4 ambiant, vec3 light_direction, vec4 light_color, vec3 normal){
	vec4 color = texture(texture_slot, tex_coords) * light_color * (ambiant + max(dot(-light_direction, normal), 0));
	return color;
}

vec4 calculate_point_light_intensity(vec3 light_position, vec4 light_color, vec3 current_position, vec3 normal){
	vec3 light_direction = current_position - light_position;
	float distance = length(light_direction);
	light_direction = normalize(light_direction);
	
	vec4 color = calculate_base_light(vec4(0.0f, 0.0f, 0.0f, 0.0f), light_direction, light_color, normalize(normal));
	//					constant		linear		exponential
	float attenuation = 1.0f + 0.10 *distance + 0.10*pow(distance, 2);

	return vec4(vec3(color.x, color.y, color.z) / attenuation, color.w);
}
void main(){
	frag_color = calculate_point_light_intensity(point_light_position, point_light_color, frag_space_coord, frag_normal);
}