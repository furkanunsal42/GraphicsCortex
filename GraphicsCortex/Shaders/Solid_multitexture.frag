
#<fragment shader>

#version 330 core
layout(location = 0) out vec4 frag_color;
in vec2 tex_coords;
in vec3 frag_normal;
in vec3 frag_space_coord;
in mat3 frag_TBN;
in float map_index_color;
in float map_index_specular;
in float map_index_normal;

// temp

struct ambiant_light{
	vec3 color;
};

struct directional_light{
	vec3 direction;
	vec3 color;
	// temp
	mat4 view_matrix;
	mat4 projection_matrix;
	sampler2D shadow_map;
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

//maps 
uniform sampler2DArray texture_array_slot;
uniform samplerCube cube_map;

// flags
uniform int use_cube_map_reflection = 0;
uniform float cube_map_reflection_strength = 0.85;

// light flags
uniform int a_lights_count;
uniform int d_lights_count;
uniform int p_lights_count;
uniform int s_lights_count;

// lights
uniform ambiant_light a_lights[10];
uniform directional_light d_lights[10];
uniform point_light p_lights[10];
uniform spot_light s_lights[10];

//other
uniform vec3 camera_coords;


vec3 calculate_ambiant_light(vec3 ambiant){
	return ambiant;
}

vec3 calculate_cube_map_reflection(vec3 current_position, vec3 camera_position,  vec3 normal){
	float reflection_strength = .8;
	normal = normal;
	vec3 camera_looking_direction = normalize(camera_position - current_position);
	camera_looking_direction.y *= -1;
	camera_looking_direction.z *= -1;
	vec3 reflected_light = normalize(reflect(camera_looking_direction, normal));
	vec4 reflection_color = texture(cube_map, reflected_light);
	reflection_color *= reflection_strength;
	return reflection_color.xyz;
}

vec3 calculate_specular_light(vec3 light_direction, vec3 current_position, vec3 camera_position, vec3 light_color, vec3 normal){
	
	float strenght = 0.8f;
	if (bool(use_cube_map_reflection))
		strenght = 2.0f;
	
	float cos_angle = 0.9f;
	if (bool(use_cube_map_reflection))
		cos_angle = 0.6;

	vec3 specular_map_color;
	if (map_index_specular > -0.5 && false)
		specular_map_color = texture(texture_array_slot, vec3(tex_coords, map_index_specular)).rgb;
	else
		specular_map_color = vec3(1.0);
	
	float specular_map_total = (specular_map_color.x + specular_map_color.y + specular_map_color.z)/3; // may change
	if (specular_map_total == 0)
		return vec3(0.0);
	vec3 camera_looking_direction = normalize(camera_position - current_position);
	vec3 reflected_light = normalize(reflect(light_direction, normal));
	float current_cos_angle = dot(reflected_light, camera_looking_direction);
	
	// if camera isn't in a suitable angle
	if(current_cos_angle < cos_angle)
		return vec3(0.0f);

	// if texture isn't lit direcly from the front
	if (dot(normal, light_direction) > 0)
		return vec3(0.0f);

	//vec3 color = light_color * (max(dot(-light_direction, normal), 0));
	float power = 100.0f;
	if (bool(use_cube_map_reflection))
		power = 30.0f;
	vec3 light = vec3(pow(current_cos_angle, power) * strenght);
	return light * specular_map_total;
}

vec3 calculate_directional_light(vec3 light_direction, vec3 light_color, vec3 normal){
	// temp
	vec4 position_by_light = (d_lights[0].projection_matrix * d_lights[0].view_matrix * vec4(frag_space_coord, 1));
	position_by_light = position_by_light / position_by_light.w;
	position_by_light = position_by_light * 0.5 + 0.5;

	float closest_distance_to_light = texture(d_lights[0].shadow_map, position_by_light.xy).r;
	float fragment_distance_to_light = position_by_light.z;
	float shadow_acne_bias = 0.005;
	
	if(closest_distance_to_light + shadow_acne_bias < fragment_distance_to_light)
		return(vec3(0));	// in shadow
	// temp end

	vec3 color = light_color * (max(dot(-light_direction, normal), 0));
	color += calculate_specular_light(light_direction, frag_space_coord, camera_coords, light_color, normal);
	return color;
}

vec3 calculate_point_light_intensity(vec3 light_position, vec3 light_color, vec3 current_position, vec3 normal, vec3 light_attenuation){
	vec3 light_direction = current_position - light_position;
	float light_length = length(light_direction);
	light_direction = normalize(light_direction);
	vec3 light = calculate_directional_light(light_direction, light_color, normalize(normal));
	float attenuation = light_attenuation.x + light_attenuation.y *light_length+ light_attenuation.z*pow(light_length, 2);
	if (attenuation == 0)
		attenuation = 1;
	
	return light.xyz / attenuation;
}

vec3 calculate_spot_light_intensity(vec3 light_position, vec3 direction, vec3 light_color, vec3 current_position, vec3 normal, vec3 light_attenuation, float cos_angle){
	vec3 light_direction = current_position - light_position;
	float light_length = length(light_direction);
	light_direction = normalize(light_direction);
	
	float current_cos_angle = dot(light_direction, direction) / (length(light_direction) * length(direction));
	if (current_cos_angle < cos_angle)
		return vec3(0.0f);
	
	vec3 light = calculate_directional_light(light_direction, light_color, normalize(normal));
	float attenuation = light_attenuation.x + light_attenuation.y *light_length+ light_attenuation.z*pow(light_length, 2);
	if (attenuation == 0)
		attenuation = 1;
	float light_intensity = max(1.0 - (1.0-current_cos_angle)/(1.0-cos_angle), 0);
	return light_intensity * light.xyz / attenuation;
}

vec3 calculate_total_light(vec3 normal, vec3 space_coords){
	vec3 total_light = vec3(0.0f);
	
	for(int i = 0; i < a_lights_count; i++)
		total_light += calculate_ambiant_light(a_lights[i].color);
	
	for(int i = 0; i < d_lights_count; i++)
		total_light += calculate_directional_light(d_lights[i].direction, d_lights[i].color, normal);
	
	for(int i = 0; i < p_lights_count; i++)
		total_light += calculate_point_light_intensity(p_lights[i].position, p_lights[i].color, space_coords, normal, vec3(p_lights[i].constant_term, p_lights[i].linear_term, p_lights[i].exponential_term));
	
	for(int i = 0; i < s_lights_count; i++)
		total_light += calculate_spot_light_intensity(s_lights[i].position, s_lights[i].direction, s_lights[i].color, space_coords, normal, vec3(s_lights[i].constant_term, s_lights[i].linear_term, s_lights[i].exponential_term), s_lights[i].cos_angle);
	
	//return clamp(total_light, 0 , 1);
	return total_light;
}

void main(){
	
	vec3 normal;
	if(bool(map_index_normal > -0.5) && false)
		normal = normalize(frag_TBN * ((texture(texture_array_slot, vec3(tex_coords, map_index_normal)) * 2).xyz - 1));
	else
		normal = frag_normal;

	vec3 total_light = calculate_total_light(normal, frag_space_coord);
	vec4 color;
	if (bool(map_index_color > -0.5))
		color = texture(texture_array_slot, vec3(tex_coords, map_index_color));
	else
		color = vec4(1.0);

	frag_color = vec4(total_light, 1) * color;

	if(bool(use_cube_map_reflection))
		frag_color = mix(frag_color, vec4(calculate_cube_map_reflection(frag_space_coord, camera_coords, normalize(normal)),1), cube_map_reflection_strength);

	if(frag_color.a < 0.1f)
		discard;

	
}