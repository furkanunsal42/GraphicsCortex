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

void edit_cornders(vec2 position, vec2 rectangle_size, vec4 rounding_amount, bool discard_frag = true, float rounding_displacement = 0.0f, vec4 color = vec4(1)){

	// top left
	if (is_inside(position, vec2(rounding_amount.y), gl_FragCoord.xy)){
		vec2 rounding_center = vec2(position.x + rounding_amount.y, position.y - rounding_amount.y);
		float distance = calculate_distance(gl_FragCoord.xy, rounding_center);
		if (distance > rounding_amount.y + rounding_displacement){
			if (discard_frag)
				discard;
			else
				frag_color = color;
		}
	}
	// top right
	vec2 corner = vec2(position.x + rectangle_size.x, position.y);
	if (is_inside(corner - vec2(rounding_amount.x, 0), vec2(rounding_amount.x), gl_FragCoord.xy)){
		vec2 rounding_center = vec2(corner.x - rounding_amount.x, corner.y - rounding_amount.x);
		float distance = calculate_distance(gl_FragCoord.xy, rounding_center);
		if (distance > rounding_amount.x + rounding_displacement){
			if (discard_frag)
				discard;
			else
				frag_color = color;
		}
	}

	// bottom left
	corner = vec2(position.x, position.y - rectangle_size.y);
	if (is_inside(corner + vec2(0, rounding_amount.z), vec2(rounding_amount.z), gl_FragCoord.xy)){
		vec2 rounding_center = vec2(corner.x + rounding_amount.z, corner.y + rounding_amount.z);
		float distance = calculate_distance(gl_FragCoord.xy, rounding_center);
		if (distance > rounding_amount.z + rounding_displacement){
			if (discard_frag)
				discard;
			else
				frag_color = color;
		}
	}

	// bottom right
	corner = vec2(position.x + rectangle_size.x, position.y - rectangle_size.y);
	if (is_inside(corner + vec2(-rounding_amount.w, rounding_amount.w), vec2(rounding_amount.w), gl_FragCoord.xy)){
		vec2 rounding_center = vec2(corner.x - rounding_amount.w, corner.y + rounding_amount.w);
		float distance = calculate_distance(gl_FragCoord.xy, rounding_center);
		if (distance > rounding_amount.w + rounding_displacement){
			if (discard_frag)
				discard;
			else
				frag_color = color;
		}
	}
}

void main(){
	frag_color = color;
	edit_cornders(screen_position, rect_size, corner_rounding, true);
	float displacement = -2.0f;
	edit_cornders(vec2(screen_position.x, screen_position.y), rect_size, corner_rounding, false, displacement, vec4(1, 1, 1, 1));
}
