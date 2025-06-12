#<fragment shader>

#version 460 core

layout (location = 0) out vec4 frag_color;

layout(binding = 0) uniform sampler3D volume;

in vec3 v_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 inverse_model;
uniform mat4 inverse_view;
uniform int sample_count;

vec2 ray_intersect_aabb(vec3 bounds_min, vec3 bounds_max, vec3 ray_origin, vec3 ray_direction) {
	vec3 inverse_ray_direction = 1.0f / ray_direction;

	vec3 t0 = (bounds_min - ray_origin) * inverse_ray_direction;
	vec3 t1 = (bounds_max - ray_origin) * inverse_ray_direction;
	vec3 tmin = min(t0, t1);
	vec3 tmax = max(t0, t1);

	float distnace_a = max(max(tmin.x, tmin.y), tmin.z);
	float distance_b = min(tmax.x, min(tmax.y, tmax.z));

	float distance_to_box = max(0, distnace_a);
	float distnace_in_box = max(0, distance_b - distance_to_box);

	return vec2(distance_to_box, distnace_in_box);
}

void main(){

	vec3 world_space_camera_pos = (inverse_view * vec4(0, 0, 0, 1)).xyz;
	vec3 world_space_view_direction = normalize(v_position.xyz - world_space_camera_pos);

	vec3 bound_min = vec3(model * vec4(-0.5, -0.5, -0.5, 1)).xyz;
	vec3 bound_max = vec3(model * vec4(+0.5, +0.5, +0.5, 1)).xyz;

	vec2 ray_info = ray_intersect_aabb(bound_min, bound_max, world_space_camera_pos, world_space_view_direction);

	vec3 world_space_begin = world_space_camera_pos + ray_info.x * world_space_view_direction;
	vec3 world_space_end = world_space_begin + ray_info.y * world_space_view_direction;

	vec3 object_space_begin = (inverse_model * vec4(world_space_begin, 1)).xyz;
	vec3 object_space_end = (inverse_model * vec4(world_space_end, 1)).xyz;

	vec3 object_space_direction = normalize(object_space_end - object_space_begin);
	float step_length = 1.0 / sample_count;
	vec3 current_position = object_space_begin;

	float total_density = 0;
	float current_depth = 0;

	while(current_depth < ray_info.y) {
		float density = texture(volume, current_position + 0.5).x;
		total_density += density * step_length;
		current_depth += step_length;
		current_position += object_space_direction * step_length;
	}
	
	frag_color = vec4(1, 1, 1, min(total_density, 1));
}