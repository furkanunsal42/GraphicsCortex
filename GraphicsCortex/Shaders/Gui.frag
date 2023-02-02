#<fragment shader>

#version 330 core 

layout (location = 0) out vec4 frag_color;

uniform vec2 rect_size;
uniform vec2 screen_position;
uniform vec4 color;
uniform vec4 corner_rounding;

bool is_inside(vec2 rect_position, vec2 rect_size, vec2 point){
	if (rect_position.x > point.x || rect_position.x + rect_size.x <= point.x)
		return false;
	if (rect_position.y <= point.y || rect_position.y - rect_size.y > point.y)
		return false;
	return true;
}

float calculate_distance(vec2 point_a, vec2 point_b){
	return sqrt(pow(point_a.x-point_b.x, 2) + pow(point_a.y-point_b.y, 2));
}


void main(){
	frag_color = color;
	// top left
	if (is_inside(screen_position, vec2(corner_rounding.y), gl_FragCoord.xy)){
		vec2 rounding_center = vec2(screen_position.x + corner_rounding.y, screen_position.y - corner_rounding.y);
		float distance = calculate_distance(gl_FragCoord.xy, rounding_center);
		if (distance > corner_rounding.y)
			discard;
	}
	// top right
	vec2 corner = vec2(screen_position.x + rect_size.x, screen_position.y);
	if (is_inside(corner - vec2(corner_rounding.x, 0), vec2(corner_rounding.x), gl_FragCoord.xy)){
		vec2 rounding_center = vec2(corner.x - corner_rounding.x, corner.y - corner_rounding.x);
		float distance = calculate_distance(gl_FragCoord.xy, rounding_center);
		if (distance > corner_rounding.x)
			discard;
	}

	// bottom left
	corner = vec2(screen_position.x, screen_position.y - rect_size.y);
	if (is_inside(corner + vec2(0, corner_rounding.z), vec2(corner_rounding.z), gl_FragCoord.xy)){
		vec2 rounding_center = vec2(corner.x + corner_rounding.z, corner.y + corner_rounding.z);
		float distance = calculate_distance(gl_FragCoord.xy, rounding_center);
		if (distance > corner_rounding.z)
			discard;
	}

	// bottom right
	corner = vec2(screen_position.x + rect_size.x, screen_position.y - rect_size.y);
	if (is_inside(corner + vec2(-corner_rounding.w, corner_rounding.w), vec2(corner_rounding.w), gl_FragCoord.xy)){
		vec2 rounding_center = vec2(corner.x - corner_rounding.w, corner.y + corner_rounding.w);
		float distance = calculate_distance(gl_FragCoord.xy, rounding_center);
		if (distance > corner_rounding.w)
			discard;
	}
}
