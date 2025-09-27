#<fragment shader>

#version 460 core 

layout (location = 0) out vec4 frag_color;

in vec4 g_position_size;
in vec4 g_texcoord_begin_end;
in vec4 g_color0;
in vec4 g_color1;
in vec4 g_color2;
in vec4 g_gradient_direction_z_pad;
in vec4 g_border_thickness;
in vec4 g_border_rounding;
in vec4 g_border_color0;
in vec4 g_border_color1;
in vec4 g_border_color2;
in vec4 g_border_color3;
in vec4 g_shadow_thickness;
in vec4 g_shadow_color;

in vec2 texcoord;
in vec2 position;

bvec4 and(bvec4 a, bvec4 b){
	return bvec4(
		a.x && b.x,
		a.y && b.y,
		a.z && b.z,
		a.w && b.w
		);
}

void main(){

	const vec2 position_local = position - g_position_size.xy;
	const vec2 size = g_position_size.zw;
	
	vec4 border_distances;
	border_distances.x = position_local.x;
	border_distances.y = position_local.y;
	border_distances.z = size.x - position_local.x;
	border_distances.w = size.y - position_local.y;

	bvec4 inside_corner = and(lessThan(border_distances.xyzw, g_border_rounding), lessThan(border_distances.yzwx, g_border_rounding));
	vec2 corner0 = g_position_size.xy + vec2(g_border_rounding.x,				g_border_rounding.y);
	vec2 corner1 = g_position_size.xy + vec2(size.x - g_border_rounding.x,		g_border_rounding.y);
	vec2 corner2 = g_position_size.xy + vec2(size.x - g_border_rounding.x,		size.y - g_border_rounding.y);
	vec2 corner3 = g_position_size.xy + vec2(g_border_rounding.x,				size.y - g_border_rounding.y);



	if (inside_corner.x && length(position_local - corner0) > g_border_rounding.x) discard;
	if (inside_corner.y && length(position_local - corner1) > g_border_rounding.y) discard;
	if (inside_corner.z && length(position_local - corner2) > g_border_rounding.z) discard;
	if (inside_corner.w && length(position_local - corner3) > g_border_rounding.w) discard;

	const vec4 length_inside_border = border_distances - g_border_thickness;

	bvec4 borders = lessThan(length_inside_border, vec4(0));
	bvec4 mask = lessThanEqual(length_inside_border.xyzw, min(min(length_inside_border.yxxx, length_inside_border.zzyy), length_inside_border.wwwz));
	borders = and(mask, borders);

	const vec4 active_color =	
		borders.x	? g_border_color0 :
		borders.y	? g_border_color1 :
		borders.z	? g_border_color2 :
		borders.w	? g_border_color3 :
		g_color0;

	frag_color = active_color;
}
