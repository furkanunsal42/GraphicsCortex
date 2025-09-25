#<geometry shader>

#version 460

layout(points) in;
layout(triangle_strip, max_vertices=6) out;

in vec4 v_position_size[];
in vec4 v_texcoord_begin_end[];
in vec4 v_color0[];
in vec4 v_color1[];
in vec4 v_color2[];
in vec4 v_gradient_direction_z_pad[];
in vec4 v_border_thickness[];
in vec4 v_border_rounding[];
in vec4 v_border_color0[];
in vec4 v_border_color1[];
in vec4 v_border_color2[];
in vec4 v_border_color3[];
in vec4 v_shadow_thickness[];
in vec4 v_shadow_color[];

out vec4 g_position_size;
out vec4 g_texcoord_begin_end;
out vec4 g_color0;
out vec4 g_color1;
out vec4 g_color2;
out vec4 g_gradient_direction_z_pad;
out vec4 g_border_thickness;
out vec4 g_border_rounding;
out vec4 g_border_color0;
out vec4 g_border_color1;
out vec4 g_border_color2;
out vec4 g_border_color3;
out vec4 g_shadow_thickness;
out vec4 g_shadow_color;

out vec2 texcoord;

uniform mat4 projection;

void main(){
	
	g_position_size				= v_position_size[0];
	g_texcoord_begin_end		= v_texcoord_begin_end[0];
	g_color0					= v_color0[0];
	g_color1					= v_color1[0];
	g_color2					= v_color2[0];
	g_gradient_direction_z_pad	= v_gradient_direction_z_pad[0];
	g_border_thickness			= v_border_thickness[0];
	g_border_rounding			= v_border_rounding[0];
	g_border_color0				= v_border_color0[0];
	g_border_color1				= v_border_color1[0];
	g_border_color2				= v_border_color2[0];
	g_border_color3				= v_border_color3[0];
	g_shadow_thickness			= v_shadow_thickness[0];
	g_shadow_color				= v_shadow_color[0];

	vec2 position;
	texcoord	= glm::vec2(v_texcoord_begin_end[0].x, v_texcoord_begin_end[0].w);
	position	= v_position_size[0].xy			+ vec2(0, 0) * vec2(v_position_size[0].z, v_position_size[0].w);
	gl_Position = projection * vec4(position, v_gradient_direction_z_pad[0].z, 1);
	EmitVertex();

	texcoord	= glm::vec2(v_texcoord_begin_end[0].x, v_texcoord_begin_end[0].y);
	position	= v_position_size[0].xy			+ vec2(0, 1) * vec2(v_position_size[0].z, v_position_size[0].w);
	gl_Position = projection * vec4(position, v_gradient_direction_z_pad[0].z, 1);
	EmitVertex();

	texcoord	= glm::vec2(v_texcoord_begin_end[0].z, v_texcoord_begin_end[0].y);
	position	= v_position_size[0].xy			+ vec2(1, 1) * vec2(v_position_size[0].z, v_position_size[0].w);
	gl_Position = projection * vec4(position, v_gradient_direction_z_pad[0].z, 1);
	EmitVertex();
	EndPrimitive();

	texcoord	= glm::vec2(v_texcoord_begin_end[0].x, v_texcoord_begin_end[0].w);
	position	= v_position_size[0].xy			+ vec2(0, 0) * vec2(v_position_size[0].z, v_position_size[0].w);
	gl_Position = projection * vec4(position, v_gradient_direction_z_pad[0].z, 1);
	EmitVertex();
	
	texcoord	= glm::vec2(v_texcoord_begin_end[0].z, v_texcoord_begin_end[0].y);
	position	= v_position_size[0].xy			+ vec2(1, 1) * vec2(v_position_size[0].z, v_position_size[0].w);
	gl_Position = projection * vec4(position, v_gradient_direction_z_pad[0].z, 1);
	EmitVertex();
	
	texcoord	= glm::vec2(v_texcoord_begin_end[0].z, v_texcoord_begin_end[0].w);
	position	= v_position_size[0].xy			+ vec2(1, 0) * vec2(v_position_size[0].z, v_position_size[0].w);
	gl_Position = projection * vec4(position, v_gradient_direction_z_pad[0].z, 1);
	EmitVertex();
	EndPrimitive();

}