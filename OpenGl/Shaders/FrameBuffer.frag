#<fragment shader>

#version 330 core 

layout (location = 0) out vec4 frag_color;
in vec2 frag_tex_coords;
uniform sampler2D texture_slot;
uniform int edge_detect;

float offset = 4/600.0f;
int kernel[] = int[](
	1, 0, 1,
	0,-4, 0,
	1, 0, 1
);

float linearize_depth(float d,float zNear,float zFar)
{
    float z_n = 2.0 * d - 1.0;
    return 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
}

void main(){
	
	/*
	vec4 color;
	vec4 color_avarage;
	if (bool(edge_detect)){
		float x, y;
		for (int i = 0; i < 9; i++){
			x = ((i % 3) - 1) * offset;
			y = ((i / 3) - 1) * offset;
			vec4 temp_color = texture(texture_slot, vec2(frag_tex_coords.x + x, frag_tex_coords.y + y));
			color += (kernel[i] * temp_color);
			color_avarage += temp_color/9;
		}
		if (length(color.xyz) < 0.6f)
			frag_color = texture(texture_slot, frag_tex_coords);
		else{
			frag_color = color_avarage;
			//frag_color = vec4(1.0f);
		} 
	}
	else{
		frag_color = texture(texture_slot, frag_tex_coords);
	}
	*/
	
	float near= 0.1f, far = 100.0f;
	float depth = texture(texture_slot, frag_tex_coords).x;
	
	frag_color = vec4(vec3(linearize_depth(depth, 0.1f, 100.0f)) / far, 1.0f);
}
