#<fragment shader>

#version 460 core 
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_color;

in vec4 v_fill_color;
in vec4 v_line_color;
in float v_line_thickness;
in vec4 v_position;
in vec4 v_clip_area;
in vec2 v_uv;

layout (std140) uniform buf {
	uvec2 texture_handle[1024];
};

void main(){
	
	if (v_uv == uvec2(2))
		frag_color = texture(sampler2D(texture_handle[0]), v_uv);
	else
		frag_color = v_fill_color;
 }

