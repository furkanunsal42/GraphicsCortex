#<fragment shader>

#version 460 core 
#extension GL_ARB_bindless_texture : require
layout (location = 0) out vec4 frag_color;

in vec4 v_fill_color;
in vec4 v_line_color;
in float v_line_thickness;
in vec4 v_position;
in vec4 v_clip_area;
in vec4 v_uv;
in flat uvec4 v_texture_index;
in vec4 v_border_roundness;
in vec4 v_border_thickness;
in vec4 v_border_color0;
in vec4 v_border_color1;
in vec4 v_border_color2;
in vec4 v_border_color3;
in vec4 v_border_shadow_thickness;
in vec4 v_border_shadow_color;
in vec4 v_position_size;
	
/*
void main(){
	
	if (v_texture_index.xy != uvec2(0, 0))
		frag_color = vec4(texture(sampler2D(v_texture_index.xy), v_uv.xy)) * v_fill_color;
	else
		frag_color = v_fill_color;
 }

 */
 
bvec4 and(bvec4 a, bvec4 b){
	return bvec4(
		a.x && b.x,
		a.y && b.y,
		a.z && b.z,
		a.w && b.w
		);
}

const float cos45 = 0.70710678118654752440084436210485;

void main(){

	const vec2 position_local = v_position.xy - v_position_size.xy;
	const vec2 size = v_position_size.zw;
	
	vec4 border_distances;
	border_distances.x = position_local.x;
	border_distances.y = position_local.y;
	border_distances.z = size.x - position_local.x;
	border_distances.w = size.y - position_local.y;
	
	bvec4 inside_corner = and(lessThan(border_distances.xyzw, v_border_roundness), lessThan(border_distances.yzwx, v_border_roundness));
	vec2 corner0 = vec2(v_border_roundness.x,				v_border_roundness.y);
	vec2 corner1 = vec2(size.x - v_border_roundness.x,		v_border_roundness.y);
	vec2 corner2 = vec2(size.x - v_border_roundness.x,		size.y - v_border_roundness.y);
	vec2 corner3 = vec2(v_border_roundness.x,				size.y - v_border_roundness.y);
	
	if (inside_corner.x && length(position_local - corner0) > v_border_roundness.x) discard;
	if (inside_corner.y && length(position_local - corner1) > v_border_roundness.y) discard;
	if (inside_corner.z && length(position_local - corner2) > v_border_roundness.z) discard;
	if (inside_corner.w && length(position_local - corner3) > v_border_roundness.w) discard;
	
	vec4 active_color;
	
	if(!any(inside_corner)){
		const vec4 length_inside_border = border_distances - v_border_thickness;
	
		bvec4 borders = lessThan(length_inside_border, vec4(0));
		bvec4 mask = lessThanEqual(length_inside_border.xyzw, min(min(length_inside_border.yxxx, length_inside_border.zzyy), length_inside_border.wwwz));
		borders = and(mask, borders);
		
		active_color =	
			borders.x	? v_border_color0 :
			borders.y	? v_border_color1 :
			borders.z	? v_border_color2 :
			borders.w	? v_border_color3 :
			v_fill_color;
	} 
	else {
		
		vec4 border_colors[4] = {
			v_border_color0,
			v_border_color1,
			v_border_color2,
			v_border_color3
		};
	
		float border_thicknesses[4] = {
			v_border_thickness.x,
			v_border_thickness.y,
			v_border_thickness.z,
			v_border_thickness.w
		};
	
		vec4 corner_angles;
		corner_angles.x = dot(normalize(position_local - corner0), vec2(-1, 0));
		corner_angles.y = dot(normalize(position_local - corner1), vec2(0, -1));
		corner_angles.z = dot(normalize(position_local - corner2), vec2(+1, 0));
		corner_angles.w = dot(normalize(position_local - corner3), vec2(0,  1));
		
		int subcorner_index0 = corner_angles.x > cos45 ? 0 : 1;
		int subcorner_index1 = corner_angles.y > cos45 ? 1 : 2;
		int subcorner_index2 = corner_angles.z > cos45 ? 2 : 3;
		int subcorner_index3 = corner_angles.w > cos45 ? 3 : 0;
	
		active_color = 
			inside_corner.x && v_border_roundness.x - length(position_local - corner0) < border_thicknesses[subcorner_index0] ? border_colors[subcorner_index0] :
			inside_corner.y && v_border_roundness.y - length(position_local - corner1) < border_thicknesses[subcorner_index1] ? border_colors[subcorner_index1] :
			inside_corner.z && v_border_roundness.z - length(position_local - corner2) < border_thicknesses[subcorner_index2] ? border_colors[subcorner_index2] :
			inside_corner.w && v_border_roundness.w - length(position_local - corner3) < border_thicknesses[subcorner_index3] ? border_colors[subcorner_index3] :
			v_fill_color;
	
	}
	
	frag_color = active_color;
}
