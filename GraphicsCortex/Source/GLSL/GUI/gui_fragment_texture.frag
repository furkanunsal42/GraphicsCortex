#<fragment shader>

#version 460 core 

layout (location = 0) out vec4 frag_color;

layout(binding = 0) uniform sampler2D source_texture;

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

void main(){
	frag_color = texture(source_texture, texcoord) * g_color0;
}

position_size;
texcoord_begin_end;
color0;
color1;
color2;
gradient_direction_z_pad;
border_thickness;
border_rounding;
border_color0;
border_color1;
border_color2;
border_color3;
shadow_thickness;
shadow_color;

