#<fragment shader>

#version 330 core 

layout (location = 0) out vec4 frag_color;

uniform vec2 rect_size;
uniform vec2 screen_position;
uniform vec4 rect_color;
uniform vec4 corner_rounding;
uniform vec4 border_color;
uniform vec4 border_thickness;

float edge_smoothing = 0.93f;
float inner_edge_smoothing = 0.05f;

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

void edit_cornders(vec2 position, vec2 rectangle_size, vec4 rounding_amount, bool discard_frag = true, vec4 rounding_displacement = vec4(0.0f), vec4 color = vec4(1)){

	vec2 tr_corner = vec2(position.x + rectangle_size.x, position.y);
	vec2 bl_corner = vec2(position.x, position.y - rectangle_size.y);
	vec2 br_corner = vec2(position.x + rectangle_size.x, position.y - rectangle_size.y);
	
	// top right
	if (is_inside(tr_corner - vec2(rounding_amount.x, 0), vec2(rounding_amount.x), gl_FragCoord.xy)){
		vec2 rounding_center = vec2(tr_corner.x - rounding_amount.x, tr_corner.y - rounding_amount.x);
		float dist = calculate_distance(gl_FragCoord.xy, rounding_center);
		if (dist > rounding_amount.x + rounding_displacement.x){
			if (discard_frag)
				discard;
			else{
				if(dist - (rounding_amount.x + rounding_displacement.x) > (-rounding_displacement.x) / 2){
					frag_color = vec4(color.xyz, 1 - smoothstep(edge_smoothing * (-rounding_displacement.x), 1 * (-rounding_displacement.x), dist - (rounding_amount.x + rounding_displacement.x)));
				}
				else{
					float mix_amount = 1 - smoothstep(0 * (-rounding_displacement.x), (inner_edge_smoothing) * (-rounding_displacement.x), dist - (rounding_amount.x + rounding_displacement.x));
					frag_color = vec4(mix(border_color.xyz, rect_color.xyz, mix_amount), 1);
				}
			}
		}
	}

	// top left
	else if (is_inside(position, vec2(rounding_amount.y), gl_FragCoord.xy)){
		vec2 rounding_center = vec2(position.x + rounding_amount.y, position.y - rounding_amount.y);
		float dist = calculate_distance(gl_FragCoord.xy, rounding_center);
		if (dist > rounding_amount.y + rounding_displacement.y){
			if (discard_frag)
				discard;
			else{
				if(dist - (rounding_amount.y + rounding_displacement.y) > (-rounding_displacement.y) / 2)
					frag_color = vec4(color.xyz, 1 - smoothstep(edge_smoothing * (-rounding_displacement.y), 1 * (-rounding_displacement.y), dist - (rounding_amount.y + rounding_displacement.y)));
				else{
					float mix_amount = 1 - smoothstep(0 * (-rounding_displacement.y), (inner_edge_smoothing) * (-rounding_displacement.y), dist - (rounding_amount.y + rounding_displacement.y));
					frag_color = vec4(mix(border_color.xyz, rect_color.xyz, mix_amount), 1);
				}
			}
		}
	}
	
	// bottom left
	else if (is_inside(bl_corner + vec2(0, rounding_amount.z), vec2(rounding_amount.z), gl_FragCoord.xy)){
		vec2 rounding_center = vec2(bl_corner.x + rounding_amount.z, bl_corner.y + rounding_amount.z);
		float dist = calculate_distance(gl_FragCoord.xy, rounding_center);
		if (dist > rounding_amount.z + rounding_displacement.z){
			if (discard_frag)
				discard;
			else{
				if(dist - (rounding_amount.z + rounding_displacement.z) > (-rounding_displacement.z) / 2)
					frag_color = vec4(color.xyz, 1 - smoothstep(edge_smoothing * (-rounding_displacement.z), 1 * (-rounding_displacement.z), dist - (rounding_amount.z + rounding_displacement.z)));
				else{
					float mix_amount = 1 - smoothstep(0 * (-rounding_displacement.z), (inner_edge_smoothing) * (-rounding_displacement.z), dist - (rounding_amount.z + rounding_displacement.z));
					frag_color = vec4(mix(border_color.xyz, rect_color.xyz, mix_amount), 1);
				}
			}
		}
	}

	// bottom right
	else if (is_inside(br_corner + vec2(-rounding_amount.w, rounding_amount.w), vec2(rounding_amount.w), gl_FragCoord.xy)){
		vec2 rounding_center = vec2(br_corner.x - rounding_amount.w, br_corner.y + rounding_amount.w);
		float dist = calculate_distance(gl_FragCoord.xy, rounding_center);
		if (dist > rounding_amount.w + rounding_displacement.w){
			if (discard_frag)
				discard;
			else{
				if(dist - (rounding_amount.w + rounding_displacement.w) > (-rounding_displacement.w) / 2)
					frag_color = vec4(color.xyz, 1 - smoothstep(edge_smoothing * (-rounding_displacement.w), 1 * (-rounding_displacement.w), dist - (rounding_amount.w + rounding_displacement.w)));
				else{
					float mix_amount = 1 - smoothstep(0 * (-rounding_displacement.w), (inner_edge_smoothing) * (-rounding_displacement.w), dist - (rounding_amount.w + rounding_displacement.w));
					frag_color = vec4(mix(border_color.xyz, rect_color.xyz, mix_amount), 1);
				}
			}
		}
	}
}

void edit_edges(vec2 position, vec2 rect_size, vec4 edge_thickness, bool discard_flag = false, vec4 color = vec4(1, 1, 1, 1)){
	// top
	if (is_inside(position, vec2(rect_size.x, edge_thickness.x), gl_FragCoord.xy)){
		if (discard_flag)
			discard;
		else{
			float dist = gl_FragCoord.y - (position.y - edge_thickness.x);
			if(dist > (edge_thickness.x) / 2)
				frag_color = frag_color = vec4(color.xyz, 1-smoothstep(edge_smoothing * edge_thickness.x, 1 * edge_thickness.x, dist));
			else{
				float mix_amount = 1 - smoothstep(0 * (edge_thickness.x), (inner_edge_smoothing) * (edge_thickness.x), dist);
				frag_color = vec4(mix(border_color.xyz, rect_color.xyz, mix_amount), 1);
			}
		}
	}
	// left
	if (is_inside(position, vec2(edge_thickness.y, rect_size.y), gl_FragCoord.xy)){
		if (discard_flag)
			discard;
		else{
			float dist = (position.x + edge_thickness.y) - gl_FragCoord.x ;
			if(dist > (edge_thickness.y) / 2)
				frag_color = frag_color = vec4(color.xyz, 1-smoothstep(edge_smoothing * edge_thickness.y, 1 * edge_thickness.y, dist));
			else{
				float mix_amount = 1 - smoothstep(0 * (edge_thickness.y), (inner_edge_smoothing) * (edge_thickness.y), dist);
				frag_color = vec4(mix(border_color.xyz, rect_color.xyz, mix_amount), 1);
			}
		}
	}
	// bottom
	if (is_inside(vec2(position.x, position.y - rect_size.y + edge_thickness.z), vec2(rect_size.x, edge_thickness.z), gl_FragCoord.xy)){
		if (discard_flag)
			discard;
		else{
			float dist = (position.y - rect_size.y + edge_thickness.z) - gl_FragCoord.y;
			if(dist > (edge_thickness.x) / 2)
				frag_color = frag_color = vec4(color.xyz, 1-smoothstep(edge_smoothing * edge_thickness.z, 1 * edge_thickness.z, dist));
			else{
				float mix_amount = 1 - smoothstep(0 * (edge_thickness.z), (inner_edge_smoothing) * (edge_thickness.z), dist);
				frag_color = vec4(mix(border_color.xyz, rect_color.xyz, mix_amount), 1);
			}
		}	
	}
	// right
	if (is_inside(vec2(position.x + rect_size.x - edge_thickness.w, position.y), vec2(edge_thickness.w, rect_size.y), gl_FragCoord.xy)){
		if (discard_flag)
			discard;
		else{
			float dist =  gl_FragCoord.x - (position.x + rect_size.x - edge_thickness.w);
			if(dist > (edge_thickness.x) / 2)
				frag_color = frag_color = vec4(color.xyz, 1-smoothstep(edge_smoothing * edge_thickness.w, 1 * edge_thickness.w, dist));
			else{
				float mix_amount = 1 - smoothstep(0 * (edge_thickness.w), (inner_edge_smoothing) * (edge_thickness.w), dist);
				frag_color = vec4(mix(border_color.xyz, rect_color.xyz, mix_amount), 1);
			}
		}	
	}

}


void main(){
	frag_color = rect_color;
	edit_cornders(screen_position, rect_size, corner_rounding, true);
	edit_edges(screen_position, rect_size, border_thickness, false, border_color);
	edit_cornders(screen_position, rect_size, corner_rounding, false, -1 * border_thickness, border_color);
}
